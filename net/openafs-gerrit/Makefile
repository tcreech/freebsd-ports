# $FreeBSD$

PORTNAME=	openafs-gerrit
GERRIT_CHANGE=14206
GERRIT_PS=2
DISTVERSION=	gerrit.${GERRIT_CHANGE}.${GERRIT_PS}.${DBVERSION:S/-//g}
CATEGORIES=	net
DISTFILES=	CellServDB.${DBVERSION}:gco
GERRIT_REV=11f67d98d7d6a23a6470b4bd88ba3fe6eb851337
MASTER_SITES= \
		http://dl.central.org/dl/cellservdb/:gco \
		http://grand.central.org/dl/cellservdb/:gco

GERRIT_URL= https://gerrit.openafs.org/changes/${GERRIT_CHANGE}/revisions/${GERRIT_REV}/archive?format=tgz

DBVERSION=	2018-05-14
EXTRACT_ONLY=
NO_WRKSUBDIR=	yes

COMMENT=	AFS implementation from openafs.org
#LICENSE=	IPLv1
#LICENSE_NAME=	IBM Public License Version 1.0

BROKEN_FreeBSD_13=	Needs updates for FreeBSD 13
ONLY_FOR_ARCHS=	i386 amd64

PORTNAME_BASE=	openafs
DATADIR=		${PREFIX}/share/${PORTNAME_BASE}
DOCSDIR=		${PREFIX}/share/doc/${PORTNAME_BASE}
ETCDIR=			${PREFIX}/etc/${PORTNAME_BASE}
EXAMPLESDIR=	${PREFIX}/share/examples/${PORTNAME_BASE}
WWWDIR=			${PREFIX}/www/${PORTNAME_BASE}

USES=		cpe kmod tar:bzip2 fakeroot autoreconf:build libtool:build gssapi
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
		${CONFIGURE_TARGET:S/amd64/x86_64/:S/12.2/12.1/}
#WRKSRC=		${WRKDIR}/${PORTNAME}-${AFS_DISTVERSION}
MAKE_ENV=	COMPILER_TYPE=${COMPILER_TYPE}
USE_RC_SUBR=	${AFSMNTDIR}server

CPE_VERSION=	${AFS_DISTVERSION}

CONFLICTS=	coda-server-6.[0-9]* heimdal-[0-9]*
CONFLICTS_INSTALL=	openafs-[0-9]* openafs16-[0-9]* openafs-devel-.*

AFSMNTDIR?=	/afs
AFSSTATEDIR?=	/var/${PORTNAME_BASE}
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

.if ${OSREL} == "13.0"
# Upstream does not support the client on 13 and newer
CONFIGURE_ARGS += --disable-kernel-module
PLIST_SUB+=	KMOD="@comment "

post-install:
	@${RMDIR} ${STAGEDIR}${PREFIX}/lib/openafs
.else
CONFIGURE_ARGS += --enable-kernel-module
PLIST_SUB+=	KMOD=
USE_RC_SUBR+= ${AFSMNTDIR}d

KERNCONF?=	GENERIC
KERNCONFDIR?=	${SRC_BASE}/sys/${ARCH}/conf

# Use the first component of ${KERNCONF}.
.for K in ${KERNCONF}
_KERNCONF?=$K
.endfor

.if !exists(${KERNCONFDIR}/${_KERNCONF})
IGNORE=	requires kernel configuration file ${_KERNCONF} to build
.endif

post-fetch:
	test -e ${DISTDIR}/11f67d9-openafs.tgz || ${FETCH_CMD} --no-verify-peer -o ${DISTDIR}/11f67d9-openafs.tgz ${GERRIT_URL}

pre-extract:
	if ! (cd ${EXTRACT_WRKDIR} && ${EXTRACT_CMD} ${EXTRACT_BEFORE_ARGS} ${DISTDIR}/11f67d9-openafs.tgz ${EXTRACT_AFTER_ARGS});\
		then \
		exit 1; \
		fi

# Do not show users the reminder about cleandepend and depend
post-extract:
	${MKDIR} ${WRKDIR}/conf
	cd ${KERNCONFDIR} && \
	    /usr/sbin/config -d ${WRKDIR}/conf ${_KERNCONF} >/dev/null

pre-configure:
	@cd ${WRKSRC} && ${SETENV} ${CONFIGURE_ENV} ./regen.sh

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

# Not sure why!
MAKE_JOBS_UNSAFE=yes

.include <bsd.port.post.mk>
