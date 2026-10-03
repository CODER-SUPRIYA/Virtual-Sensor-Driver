#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x155e60e3, "misc_deregister" },
	{ 0x85acaba2, "cancel_delayed_work_sync" },
	{ 0x26772f20, "remove_proc_entry" },
	{ 0xe8213e80, "_printk" },
	{ 0xbd03ed67, "__ref_stack_chk_guard" },
	{ 0x7851be11, "__SCT__might_resched" },
	{ 0x092a35a2, "_copy_to_user" },
	{ 0x7a5ffe84, "init_wait_entry" },
	{ 0xd272d446, "schedule" },
	{ 0xc281f1fb, "prepare_to_wait_event" },
	{ 0xb730487b, "finish_wait" },
	{ 0xd272d446, "__stack_chk_fail" },
	{ 0xe4de56b4, "__ubsan_handle_load_invalid_value" },
	{ 0x5a844b26, "__x86_indirect_thunk_rax" },
	{ 0x9aa6980d, "mutex_init_generic" },
	{ 0xe804603d, "__init_waitqueue_head" },
	{ 0x71798f7e, "delayed_work_timer_fn" },
	{ 0x02f9bbf0, "timer_init_key" },
	{ 0xe98a2697, "misc_register" },
	{ 0xaf25c7d9, "proc_create_single_data" },
	{ 0x534ed5f3, "__msecs_to_jiffies" },
	{ 0xaef1f20d, "system_percpu_wq" },
	{ 0x8ce83585, "queue_delayed_work_on" },
	{ 0xf10582b6, "get_random_u16" },
	{ 0x12ca6142, "ktime_get_with_offset" },
	{ 0x68a1b6c6, "__wake_up" },
	{ 0xd272d446, "__fentry__" },
	{ 0x9aa6980d, "mutex_lock" },
	{ 0x5fe49b2b, "seq_printf" },
	{ 0x9aa6980d, "mutex_unlock" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0x7851be11, "__get_user_4" },
	{ 0xd272d446, "__put_user_4" },
	{ 0xe9196a28, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0x155e60e3,
	0x85acaba2,
	0x26772f20,
	0xe8213e80,
	0xbd03ed67,
	0x7851be11,
	0x092a35a2,
	0x7a5ffe84,
	0xd272d446,
	0xc281f1fb,
	0xb730487b,
	0xd272d446,
	0xe4de56b4,
	0x5a844b26,
	0x9aa6980d,
	0xe804603d,
	0x71798f7e,
	0x02f9bbf0,
	0xe98a2697,
	0xaf25c7d9,
	0x534ed5f3,
	0xaef1f20d,
	0x8ce83585,
	0xf10582b6,
	0x12ca6142,
	0x68a1b6c6,
	0xd272d446,
	0x9aa6980d,
	0x5fe49b2b,
	0x9aa6980d,
	0xd272d446,
	0x7851be11,
	0xd272d446,
	0xe9196a28,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"misc_deregister\0"
	"cancel_delayed_work_sync\0"
	"remove_proc_entry\0"
	"_printk\0"
	"__ref_stack_chk_guard\0"
	"__SCT__might_resched\0"
	"_copy_to_user\0"
	"init_wait_entry\0"
	"schedule\0"
	"prepare_to_wait_event\0"
	"finish_wait\0"
	"__stack_chk_fail\0"
	"__ubsan_handle_load_invalid_value\0"
	"__x86_indirect_thunk_rax\0"
	"mutex_init_generic\0"
	"__init_waitqueue_head\0"
	"delayed_work_timer_fn\0"
	"timer_init_key\0"
	"misc_register\0"
	"proc_create_single_data\0"
	"__msecs_to_jiffies\0"
	"system_percpu_wq\0"
	"queue_delayed_work_on\0"
	"get_random_u16\0"
	"ktime_get_with_offset\0"
	"__wake_up\0"
	"__fentry__\0"
	"mutex_lock\0"
	"seq_printf\0"
	"mutex_unlock\0"
	"__x86_return_thunk\0"
	"__get_user_4\0"
	"__put_user_4\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "43F895DDAA55C178763401D");
