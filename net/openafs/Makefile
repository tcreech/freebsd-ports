# $FreeBSD$

PORTNAME=	openafs
DISTVERSION=	${AFS_DISTVERSION}.${DBVERSION:S/-//g}
CATEGORIES=	net
MASTER_SITES=	http://dl.central.org/dl/openafs/${AFS_DISTVERSION}/:openafs \
		http://dl.openafs.org/dl/openafs/${AFS_DISTVERSION}/:openafs \
		http://dl.central.org/dl/cellservdb/:gco \
		http://grand.central.org/dl/cellservdb/:gco
DISTFILES=	${PORTNAME}-${AFS_DISTVERSION}-src.tar.bz2:openafs \
		${PORTNAME}-${AFS_DISTVERSION}-doc.tar.bz2:openafs \
		CellServDB.${DBVERSION}:gco
EXTRACT_ONLY=	${PORTNAME}-${AFS_DISTVERSION}-src.tar.bz2 \
		${PORTNAME}-${AFS_DISTVERSION}-doc.tar.bz2

MAINTAINER=	bjk@FreeBSD.org
COMMENT=	AFS implementation from openafs.org
BROKEN_FreeBSD_15=    Needs updates for FreeBSD 15

LICENSE=	IPLv1
LICENSE_NAME=	IBM Public License Version 1.0
LICENSE_FILE=	${WRKSRC}/doc/LICENSE
LICENSE_PERMS=	dist-mirror dist-sell pkg-mirror pkg-sell auto-accept

# NOTE: upstream has marked net/openafs as deprecated
#BROKEN=		fails to build
#DEPRECATED=	Broken for more than 6 months
#EXPIRATION_DATE=	2019-05-08
ONLY_FOR_ARCHS=	i386 amd64 aarch64

AFS_DISTVERSION=	1.8.13.2
DBVERSION=	2023-10-31

USES=		cpe kmod tar:bzip2 uidfix
USE_LDCONFIG=	yes
HAS_CONFIGURE=	yes
CONFIGURE_ENV=	_XCFLAGS="${CFLAGS} -fPIC" \
		_PAM_CFLAGS="${CFLAGS} -fPIC" \
		_MT_LIBS="-lpthread"
CONFIGURE_ARGS=	--prefix=${PREFIX} \
		--localstatedir=/var \
		--mandir=${PREFIX}/share/man \
		--with-bsd-kernel-headers=${SRC_BASE}/sys \
		--with-bsd-kernel-build=${WRKDIR}/conf \
		--enable-debug \
		--enable-debug-kernel \
		--enable-debug-lwp \
		--includedir=${PREFIX}/include/openafs \
		${CONFIGURE_TARGET}
WRKSRC=		${WRKDIR}/${PORTNAME}-${AFS_DISTVERSION}
MAKE_ENV=	COMPILER_TYPE=${COMPILER_TYPE} ARCH=${ARCH} MACHINE_ARCH=${MACHINE_ARCH}
USE_RC_SUBR=	${AFSMNTDIR}server

CPE_VERSION=	${AFS_DISTVERSION}

CONFLICTS=	coda-server-6.[0-9]* heimdal-[0-9]*
CONFLICTS_INSTALL=	openafs16-[0-9]* openafs-devel-.* openafs-gerrit-.*

AFSMNTDIR?=	/afs
AFSSTATEDIR?=	/var/${PORTNAME}
AFSCACHEDIR?=	${AFSSTATEDIR}/cache
PLIST_SUB+=	AFSMNTDIR=${AFSMNTDIR:C,^/,,} \
		AFSSTATEDIR=${AFSSTATEDIR:C,^/,,} \
		AFSCACHEDIR=${AFSCACHEDIR:C,^/,,}

# clang (the base system compiler in __FreeBSD_version 1000024 and later)
# is pickier about stack alignment than gcc; OpenAFS uses its own
# lightweight process "threading" library which will produce a misaligned
# stack under clang; we need to pass -mstackrealign to avoid SIGBUS at
# runtime.  gcc does not understand this flag, so we use compiler.mk to
# figure out what type of compiler is in use.
USES+=		compiler

OPTIONS_DEFINE=	FUSE
FUSE_USES=	fuse pkgconfig
OPTIONS_SUB=	yes
FUSE_CONFIGURE_OFF=	--disable-fuse-client

.include <bsd.port.pre.mk>

.if ${COMPILER_TYPE} == clang
CFLAGS+=	-mstackrealign -Wno-shift-negative-value -Wno-pointer-sign
.endif

.if ${OSREL:R} >= 13
# FreeBSD 13 no long has an "as" tool as part of base.
# OpenAFS configure script will insist on finding one, even if it may not be
# used. Arrange for it to use cc to find an assembler instead.
# (In the common case, Clang has an integrated assembler.)
CONFIGURE_ARGS+=	AS=$(CC)
.endif

.if ${OSREL:R} >= 15
# Upstream does not support the client on 15 and newer
CONFIGURE_ARGS += --disable-kernel-module
PLIST_SUB+=	KMOD="@comment "

post-install:
	@${RMDIR} ${STAGEDIR}${PREFIX}/lib/openafs
.else
CONFIGURE_ARGS += --enable-kernel-module
PLIST_SUB+=	KMOD=
USE_RC_SUBR+= ${AFSMNTDIR}d

KERNCONF?=	GENERIC
KERNCONFDIR?=	${SRC_BASE}/sys/${MACHINE}/conf

# Use the first component of ${KERNCONF}.
.for K in ${KERNCONF}
_KERNCONF?=$K
.endfor

.if !exists(${KERNCONFDIR}/${_KERNCONF})
IGNORE=	requires kernel configuration file ${_KERNCONF} to build
.endif

# Do not show users the reminder about cleandepend and depend
post-extract:
	${MKDIR} ${WRKDIR}/conf
	cd ${KERNCONFDIR} && \
	    /usr/sbin/config -d ${WRKDIR}/conf ${_KERNCONF} >/dev/null

create-sample-files:
	@${ECHO_CMD} openafs.org > ${WRKDIR}/ThisCell
	@${ECHO_CMD} "${AFSMNTDIR}:${AFSCACHEDIR}:150000" > ${WRKDIR}/cacheinfo
	${INSTALL_DATA} ${DISTDIR}/CellServDB.${DBVERSION} ${WRKDIR}/CellServDB

post-install: create-sample-files
	@${MKDIR} ${STAGEDIR}${ETCDIR}
.for F in CellServDB ThisCell cacheinfo
	${INSTALL_DATA} ${WRKDIR}/${F} ${STAGEDIR}${ETCDIR}/${F}.sample
.endfor
	@${RM} ${STAGEDIR}${PREFIX}/lib/openafs/libafs.ko
	@${RMDIR} ${STAGEDIR}${PREFIX}/lib/openafs
	@${MKDIR} ${STAGEDIR}${AFSMNTDIR} \
	    ${STAGEDIR}${AFSSTATEDIR} ${STAGEDIR}${AFSCACHEDIR}
.endif # ${OSREL} blacklist

.include <bsd.port.post.mk>
