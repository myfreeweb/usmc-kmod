#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

typedef struct {
	uint64_t reg0;
	uint64_t reg1;
	uint64_t reg2;
	uint64_t reg3;
} smc_regs_t;

int main(int argv, char **argc) {
	if (argv != 5) {
		fprintf(stderr, "usage: smctool 0x.. 0x.. 0x.. 0x..\n");
		return 1;
	}

	int fd = openat(AT_FDCWD, "/dev/usmc", O_RDWR | O_DIRECT);
	if (fd < 1) {
		fprintf(stderr, "could not open: %d\n", errno);
		return 2;
	}

	smc_regs_t regs;
	regs.reg0 = strtoll(argc[1], NULL, 16);
	regs.reg1 = strtoll(argc[2], NULL, 16);
	regs.reg2 = strtoll(argc[3], NULL, 16);
	regs.reg3 = strtoll(argc[4], NULL, 16);

	if (write(fd, &regs, sizeof(smc_regs_t)) != sizeof(smc_regs_t)) {
		fprintf(stderr, "could not write: %d\n", errno);
		return 3;
	}

	if (read(fd, &regs, sizeof(smc_regs_t)) != sizeof(smc_regs_t)) {
		fprintf(stderr, "could not read: %d\n", errno);
		return 4;
	}

	printf("%lx\n%lx\n%lx\n%lx\n", regs.reg0, regs.reg1, regs.reg2, regs.reg3);
	return 0;
}
