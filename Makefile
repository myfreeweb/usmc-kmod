SRCDIR=	${.CURDIR}

.PATH:	${SRCDIR}

KMOD=	usmc

SRCS=	usmc.c calls.S device_if.h bus_if.h

CLEANFILES+= ${KMOD}.ko.full ${KMOD}.ko.debug

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'

.include <bsd.kmod.mk>
