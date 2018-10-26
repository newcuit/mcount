#include <stdio.h>

#define GNU_MAX_CALL_COUNT 100
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
typedef int (*mcount_cb_t)(int called_address);
typedef int (*mcount_show_t)(int called_address);

static int call_idx = 0;
static int mcount_save = 1;
static mcount_cb_t m_callback = NULL;
static int save_address[GNU_MAX_CALL_COUNT];

void __attribute__((naked)) __gnu_mcount_nc(void)
{
	int lr;

	__asm__ __volatile__("push {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, lr}");
	__asm__ __volatile__("mov %0, lr":"=r"(lr)::);

	if(likely(mcount_save)) {
		save_address[call_idx++%GNU_MAX_CALL_COUNT] = lr;
	}

	if(unlikely(m_callback)) {
		m_callback(lr);
	}

	__asm__ __volatile__("pop {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, ip, lr}");
	__asm__ __volatile__("bx ip");
}

int mcount_show(mcount_show_t cb)
{
	int cnt = 0;
	int i = call_idx%GNU_MAX_CALL_COUNT;

	for (; cnt < GNU_MAX_CALL_COUNT;cnt++) {
		if(likely(cb != NULL)) {
			cb(save_address[i++%GNU_MAX_CALL_COUNT]);
		} else {
			printf("Function called by %08x\n", 
					save_address[i++%GNU_MAX_CALL_COUNT]);
		}
	}
	return 0;
}

int mcount_init(int save, mcount_cb_t cb)
{
	m_callback = cb;
	mcount_save = save;
	return 0;
}

int mcount_reinit(int save, mcount_cb_t cb)
{
	return mcount_init(save, cb);
}
