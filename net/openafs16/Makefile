# $FreeBSD$

PORTNAME=	openafs16
AFS_NAME=	${PORTNAME:C/[0-9][0-9]//g}
DISTVERSION=	${AFS_DISTVERSION}.${DBVERSION:S/-//g}
CATEGORIES=	net
MASTER_SITES=	http://dl.central.org/dl/openafs/${AFS_DISTVERSION}/:openafs \
		http://dl.openafs.org/dl/openafs/${AFS_DISTVERSION}/:openafs \
		http://dl.central.org/dl/cellservdb/:gco \
		http://grand.central.org/dl/cellservdb/:gco
DISTFILES=	${AFS_NAME}-${AFS_DISTVERSION}-src.tar.bz2:openafs \
		${AFS_NAME}-${AFS_DISTVERSION}-doc.tar.bz2:openafs \
		CellServDB.${DBVERSION}:gco
EXTRACT_ONLY=	${AFS_NAME}-${AFS_DISTVERSION}-src.tar.bz2 \
		${AFS_NAME}-${AFS_DISTVERSION}-doc.tar.bz2

MAINTAINER=	bjk@FreeBSD.org
COMMENT=	AFS implementation from openafs.org
IGNORE_FreeBSD_13=    Needs updates for FreeBSD 13

LICENSE=	IPLv1
LICENSE_NAME=	IBM Public License Version 1.0
LICENSE_FILE=	${WRKSRC}/doc/LICENSE
LICENSE_PERMS=	dist-mirror dist-sell pkg-mirror pkg-sell auto-accept

#BROKEN=		fails to build
ONLY_FOR_ARCHS=	i386 amd64

AFS_DISTVERSION=	1.6.24
DBVERSION=	2018-05-14

USES=		cpe kmod tar:bzip2
USE_LDCONFIG=	yes
HAS_CONFIGURE=	yes
CONFIGURE_ENV=	_XCFLAGS="${CFLAGS} -fPIC" \
		_PAM_CFLAGS="${CFLAGS} -fPIC" \
		_MT_LIBS="-lpthread"
CONFIGURE_ARGS=	--prefix=${PREFIX} \
		--localstatedir=/var \
		--mandir=${MANPREFIX}/man \
		--with-bsd-kernel-headers=${SRC_BASE}/sys \
		--with-bsd-kernel-build=${WRKDIR}/conf \
		--enable-debug \
		--enable-debug-kernel \
		--enable-debug-lwp \
		--includedir=${PREFIX}/include/openafs \
		${CONFIGURE_TARGET:S/12.2/12.1/}
WRKSRC=		${WRKDIR}/${AFS_NAME}-${AFS_DISTVERSION}
MAKE_ENV=	COMPILER_TYPE=${COMPILER_TYPE}
USE_RC_SUBR=	afsserver

CPE_VERSION=	${AFS_DISTVERSION}

CONFLICTS=	coda-server-6.[0-9]* heimdal-[0-9]*
CONFLICTS_INSTALL=	${AFS_NAME}-[0-9]* openafs-[0-9]* openafs-devel-.* openafs-gerrit-.*

ETCDIR?=	${PREFIX}/etc/${AFS_NAME}
DATADIR?=	${PREFIX}/share/${AFS_NAME}

AFSMNTDIR?=	/afs
AFSSTATEDIR?=	/var/${AFS_NAME}
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
FUSE_CONFIGURE_ON=	--enable-fuse-client

.include <bsd.port.pre.mk>

.if ${COMPILER_TYPE} == clang
CFLAGS+=	-mstackrealign
.endif

.if ${OSREL} == "13.0"
# Upstream does not support the client on 13 and newer
CONFIGURE_ARGS += --disable-kernel-module
PLIST_SUB+=	KMOD="@comment "

post-install:
	@${RMDIR} ${STAGEDIR}${PREFIX}/lib/openafs
.else
CONFIGURE_ARGS += --enable-kernel-module
PLIST_SUB+=	KMOD=
USE_RC_SUBR+= afsd

KERNCONF?=	GENERIC
KERNCONFDIR?=	${SRC_BASE}/sys/${ARCH}/conf

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
	${INSTALL_KLD} ${STAGEDIR}${PREFIX}/lib/openafs/libafs.ko \
	    ${STAGEDIR}${KMODDIR}
	@${RM} ${STAGEDIR}${PREFIX}/lib/openafs/libafs.ko
	@${RMDIR} ${STAGEDIR}${PREFIX}/lib/openafs
	@${MKDIR} ${STAGEDIR}${AFSMNTDIR} \
	    ${STAGEDIR}${AFSSTATEDIR} ${STAGEDIR}${AFSCACHEDIR}
.endif # ${OSREL} blacklist

.include <bsd.port.post.mk>
