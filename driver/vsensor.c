// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/workqueue.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/mutex.h>
#include <linux/ktime.h>
#include <linux/random.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include "../include/vsensor_ioctl.h"

struct vsensor_dev {
    struct miscdevice misc;
    struct delayed_work dwork;
    struct mutex lock;
    wait_queue_head_t wq;
    struct vsensor_reading last;
    bool fresh;
    u32 interval_ms;
    s32 high_thr, low_thr, temp;
    u64 samples;
};

static struct vsensor_dev vs;

static void vsensor_work(struct work_struct *w)
{
    struct vsensor_dev *d = container_of(w, struct vsensor_dev, dwork.work);
    u32 interval;

    mutex_lock(&d->lock);
    d->temp += (s32)get_random_u32_below(1001) - 500;   /* +-0.5 C walk */
    if (d->temp < 15000) d->temp = 15000;
    if (d->temp > 60000) d->temp = 60000;

    d->last.temp_mC = d->temp;
    d->last.timestamp_ns = ktime_get_real_ns();
    d->last.status = 0;
    if (d->temp > d->high_thr) d->last.status |= VSENSOR_STATUS_HIGH;
    if (d->temp < d->low_thr)  d->last.status |= VSENSOR_STATUS_LOW;
    d->fresh = true;
    d->samples++;
    interval = d->interval_ms;
    mutex_unlock(&d->lock);

    wake_up_interruptible(&d->wq);
    schedule_delayed_work(&d->dwork, msecs_to_jiffies(interval));
}

static ssize_t vsensor_read(struct file *f, char __user *buf,
                            size_t len, loff_t *off)
{
    struct vsensor_reading r;

    if (len < sizeof(r))
        return -EINVAL;

    if (!READ_ONCE(vs.fresh)) {
        if (f->f_flags & O_NONBLOCK)
            return -EAGAIN;
        if (wait_event_interruptible(vs.wq, READ_ONCE(vs.fresh)))
            return -ERESTARTSYS;
    }

    mutex_lock(&vs.lock);
    r = vs.last;
    vs.fresh = false;
    mutex_unlock(&vs.lock);

    if (copy_to_user(buf, &r, sizeof(r)))
        return -EFAULT;
    return sizeof(r);
}

static __poll_t vsensor_poll(struct file *f, poll_table *wait)
{
    poll_wait(f, &vs.wq, wait);
    return READ_ONCE(vs.fresh) ? (EPOLLIN | EPOLLRDNORM) : 0;
}

static long vsensor_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    u32 u;
    s32 s;

    switch (cmd) {
    case VSENSOR_SET_INTERVAL:
        if (get_user(u, (u32 __user *)arg)) return -EFAULT;
        if (u < 10 || u > 10000) return -EINVAL;
        mutex_lock(&vs.lock); vs.interval_ms = u; mutex_unlock(&vs.lock);
        return 0;
    case VSENSOR_SET_HIGH_THR:
        if (get_user(s, (s32 __user *)arg)) return -EFAULT;
        mutex_lock(&vs.lock); vs.high_thr = s; mutex_unlock(&vs.lock);
        return 0;
    case VSENSOR_SET_LOW_THR:
        if (get_user(s, (s32 __user *)arg)) return -EFAULT;
        mutex_lock(&vs.lock); vs.low_thr = s; mutex_unlock(&vs.lock);
        return 0;
    case VSENSOR_GET_STATUS:
        mutex_lock(&vs.lock); u = vs.last.status; mutex_unlock(&vs.lock);
        return put_user(u, (u32 __user *)arg);
    default:
        return -ENOTTY;
    }
}

static int vsensor_proc_show(struct seq_file *m, void *v)
{
    mutex_lock(&vs.lock);
    seq_printf(m, "temp_mC:     %d\ninterval_ms: %u\nhigh_thr:    %d\n"
                  "low_thr:     %d\nsamples:     %llu\nstatus:      0x%x\n",
               vs.temp, vs.interval_ms, vs.high_thr, vs.low_thr,
               vs.samples, vs.last.status);
    mutex_unlock(&vs.lock);
    return 0;
}

static const struct file_operations vsensor_fops = {
    .owner          = THIS_MODULE,
    .read           = vsensor_read,
    .poll           = vsensor_poll,
    .unlocked_ioctl = vsensor_ioctl,
};

static int __init vsensor_init(void)
{
    int ret;

    mutex_init(&vs.lock);
    init_waitqueue_head(&vs.wq);
    INIT_DELAYED_WORK(&vs.dwork, vsensor_work);
    vs.interval_ms = 500;
    vs.high_thr = 35000;
    vs.low_thr = 20000;
    vs.temp = 25000;

    vs.misc.minor = MISC_DYNAMIC_MINOR;
    vs.misc.name  = "vsensor0";
    vs.misc.fops  = &vsensor_fops;
    vs.misc.mode  = 0666;
    ret = misc_register(&vs.misc);
    if (ret)
        return ret;

    proc_create_single("vsensor", 0444, NULL, vsensor_proc_show);
    schedule_delayed_work(&vs.dwork, msecs_to_jiffies(vs.interval_ms));
    pr_info("vsensor: loaded, /dev/vsensor0 ready\n");
    return 0;
}

static void __exit vsensor_exit(void)
{
    misc_deregister(&vs.misc);
    cancel_delayed_work_sync(&vs.dwork);
    remove_proc_entry("vsensor", NULL);
    pr_info("vsensor: unloaded\n");
}

module_init(vsensor_init);
module_exit(vsensor_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Supriya");
MODULE_DESCRIPTION("Virtual Sensor Driver");
