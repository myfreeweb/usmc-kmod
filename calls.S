#include <machine/asm.h>

ENTRY(usmc_smc_call)
	str	x4, [sp, #-8]
	smc	#0
	ldr	x8, [sp, #-8]
	stp	x0, x1, [x8]
	stp	x2, x3, [x8, #16]
	ret
END(usmc_smc_call)
