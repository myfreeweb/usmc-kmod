#include <sys/cdefs.h>

#include <sys/param.h>
#include <sys/types.h>
#include <sys/bus.h>
#include <sys/kernel.h>
#include <sys/kobj.h>
#include <sys/systm.h>
#include <sys/conf.h>
#include <sys/uio.h>
#include <sys/malloc.h>
#include <sys/module.h>

typedef struct {
	uint64_t reg0;
	uint64_t reg1;
	uint64_t reg2;
	uint64_t reg3;
} smc_regs_t;

int usmc_smc_call(register_t, register_t, register_t, register_t, smc_regs_t *);

static d_open_t      usmc_open;
static d_close_t     usmc_close;
static d_read_t      usmc_read;
static d_write_t     usmc_write;

static struct cdevsw usmc_cdevsw = {
	.d_version = D_VERSION,
	.d_open = usmc_open,
	.d_close = usmc_close,
	.d_read = usmc_read,
	.d_write = usmc_write,
	.d_name = "usmc",
};
static struct cdev *usmc_dev;
static smc_regs_t cur_result;

static int
usmc_loader(struct module *m __unused, int what, void *arg __unused)
{
	int error = 0;

	switch (what) {
		case MOD_LOAD:
			error = make_dev_p(MAKEDEV_CHECKNAME | MAKEDEV_WAITOK,
					&usmc_dev, &usmc_cdevsw,
					0, UID_ROOT, GID_WHEEL, 0600, "usmc");
			break;
		case MOD_UNLOAD:
			destroy_dev(usmc_dev);
			break;
		default:
			error = EOPNOTSUPP;
			break;
	}
	return (error);
}

static int
usmc_open(struct cdev *dev __unused, int oflags __unused, int devtype __unused,
		struct thread *td __unused)
{

	return (0);
}

static int
usmc_close(struct cdev *dev __unused, int fflag __unused, int devtype __unused,
		struct thread *td __unused)
{

	return (0);
}

static int
usmc_read(struct cdev *dev __unused, struct uio *uio, int ioflag __unused)
{
	int error;

	if ((error = uiomove(&cur_result, sizeof(smc_regs_t), uio)) != 0)
		uprintf("uiomove\n");

	return (error);
}

static int
usmc_write(struct cdev *dev __unused, struct uio *uio, int ioflag __unused)
{
	int error;

	if (uio->uio_offset != 0)
		return (EINVAL);

	error = uiomove(&cur_result, sizeof(smc_regs_t), uio);
	if (error != 0)
		uprintf("uiomove\n");

	printf("USMC before: %lx %lx %lx %lx\n", cur_result.reg0, cur_result.reg1, cur_result.reg2, cur_result.reg3);
	usmc_smc_call(cur_result.reg0, cur_result.reg1, cur_result.reg2, cur_result.reg3, &cur_result);
	printf("USMC after: %lx %lx %lx %lx\n", cur_result.reg0, cur_result.reg1, cur_result.reg2, cur_result.reg3);
	return (error);
}

DEV_MODULE(usmc, usmc_loader, NULL);
