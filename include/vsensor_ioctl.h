#ifndef VSENSOR_IOCTL_H
#define VSENSOR_IOCTL_H

#include <linux/types.h>
#include <linux/ioctl.h>

#define VSENSOR_MAGIC 'v'

#define VSENSOR_STATUS_HIGH 0x1   /* temp above high threshold */
#define VSENSOR_STATUS_LOW  0x2   /* temp below low threshold  */

struct vsensor_reading {
    __s32 temp_mC;          /* milli-degrees Celsius (no floats in kernel) */
    __u32 status;           /* VSENSOR_STATUS_* bit flags */
    __u64 timestamp_ns;
};

#define VSENSOR_SET_INTERVAL _IOW(VSENSOR_MAGIC, 1, __u32) /* ms, 10..10000 */
#define VSENSOR_SET_HIGH_THR _IOW(VSENSOR_MAGIC, 2, __s32) /* mC */
#define VSENSOR_SET_LOW_THR  _IOW(VSENSOR_MAGIC, 3, __s32) /* mC */
#define VSENSOR_GET_STATUS   _IOR(VSENSOR_MAGIC, 4, __u32)

#endif
