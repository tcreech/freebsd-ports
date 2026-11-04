PORTNAME=	tcreech-containers
DISTVERSION=	1
PORTREVISION=	1
CATEGORIES=	sysutils
MASTER_SITES=	# no distfiles: every payload lives in files/
DISTFILES=	# empty on purpose

MAINTAINER=	tcreech@tcreech.com
COMMENT=	Site OCI hooks and container host policy for podman/ocijail
WWW=		https://www.tcreech.com/

# Dependency patterns must be ABSOLUTE paths.  Mk/Scripts/do-depends.sh
# classifies them: "/*" is tested with [ -e ], anything else goes to
# "which -s", so a relative pattern such as bin/podman can never be
# satisfied and the framework then builds that dependency from source.
# Ask "are my dependencies satisfied" with -DSTRICT_DEPENDS, which checks
# without building (or -DUSE_PACKAGE_DEPENDS_ONLY to refuse source builds).

# Every hook except the helper is a python script, and nothing in podman,
# ocijail or cni-epair provides one -- podman's own port uses USES=python:build
# only, and python has been present on our hosts by luck, as a side effect of
# py3x-podman-compose.  USES=python:run supplies the dependency and, with
# USES=shebangfix, rewrites the source shebangs to the versioned interpreter:
# the hooks must not depend on $PATH, because ocijail replaces the hook
# environment whenever the hook JSON sets "env".
RUN_DEPENDS=	${LOCALBASE}/bin/podman:sysutils/podman \
		${LOCALBASE}/bin/ocijail:sysutils/ocijail
# cni-epair is main-only (new ports are not merged to quarterly), and naming a
# missing origin makes poudriere abort bulk and pkgclean for the whole set.
.if exists(${.CURDIR}/../cni-epair/Makefile)
RUN_DEPENDS+=	${LOCALBASE}/libexec/cni/epair:sysutils/cni-epair
.endif

USES=		python:run shebangfix
# Rewritten in ${WRKSRC}, then installed from there; the extensionless apt
# hook is listed explicitly because it has no .py suffix.
SHEBANG_FILES=	hooks.d/apt-cache-start
SHEBANG_GLOB=	*.py

NO_ARCH=	yes
NO_BUILD=	yes
PORTSCOUT=	ignore:1

# Everything this site ships in ${PREFIX}/etc/containers/hooks.d.  These are
# production files, not prototypes: zfs.{py,json} are byte-identical to the
# copies on the host that runs them in production apart from the shebang, and
# children/linprocfs come from a host that needed those jail tunables.  All
# four ship everywhere, because an annotation-gated hook is inert on a host
# that never sets the annotation.
#
# Shebangs are pinned to ${PREFIX}/bin/python3 rather than /usr/bin/env
# because ocijail replaces the hook environment when the hook JSON sets
# "env", which can leave $PATH without ${PREFIX}/bin.
HOOK_SCRIPTS=	apt-cache-start children.py linprocfs.py zfs.py
HOOK_JSONS=	apt-cache-start.json children.json linprocfs.json zfs.json

# Bridge the epair network puts containers on.  It has to be an existing
# bridge(4) with the LAN NIC as an untagged member: the epair plugin runs
# "ifconfig <bridge> addm <epair>" and refuses to start a container when the
# bridge is missing, so a plain NIC name is never valid.  This package does
# not create it -- rc.conf should, which also fixes the boot order, since
# podman starts before jail and a bridge that only exists because some jail's
# exec.prestart made it is not there when restart=always containers come up:
#
#	cloned_interfaces="igb0int0"
#	ifconfig_igb0int0="addm igb0 up"
#
# Override per host in make.conf; hosts that run no LAN-addressed containers
# can leave the generated lan.conflist uninstalled.
BRIDGE?=	igb0int0

# No distfiles, so give the patch stages somewhere to work: the hook payloads
# are copied out of ${FILESDIR} so that shebangfix can rewrite them in place.
WRKSRC=		${WRKDIR}/src

post-extract:
	${MKDIR} ${WRKSRC}
	${CP} -R ${FILESDIR}/hooks.d ${WRKSRC}/hooks.d

SUB_FILES=	pkg-message tcreech-enable-hooks-dir lan.conflist
SUB_LIST=	ENABLE=${PREFIX}/libexec/podman/tcreech-enable-hooks-dir \
		CONFC=${PREFIX}/etc/containers/containers.conf \
		HOOKSDIR=${PREFIX}/etc/containers/hooks.d \
		BRIDGE=${BRIDGE}

do-install:
	${MKDIR} ${STAGEDIR}${PREFIX}/libexec/podman \
		${STAGEDIR}${PREFIX}/etc/containers/hooks.d \
		${STAGEDIR}${PREFIX}/etc/containers/registries.conf.d \
		${STAGEDIR}${PREFIX}/etc/cni/net.d
# helper: containers.conf has no drop-in directory, so hooks_dir must be set
# in the live file; this does it idempotently and shows what it changed
	${INSTALL_SCRIPT} ${WRKDIR}/tcreech-enable-hooks-dir \
		${STAGEDIR}${PREFIX}/libexec/podman/
# OCI hooks; each script sits next to its JSON (non-*.json names are skipped
# by the hook reader, so co-locating the payload is safe)
.for h in ${HOOK_SCRIPTS}
	${INSTALL_SCRIPT} ${WRKSRC}/hooks.d/${h} \
		${STAGEDIR}${PREFIX}/etc/containers/hooks.d/
.endfor
.for h in ${HOOK_JSONS}
	${INSTALL_DATA} ${WRKSRC}/hooks.d/${h} \
		${STAGEDIR}${PREFIX}/etc/containers/hooks.d/
.endfor
# LAN registry trust; registries.conf.d IS a real drop-in directory
	${INSTALL_DATA} ${FILESDIR}/registries.conf.d/tutti.conf \
		${STAGEDIR}${PREFIX}/etc/containers/registries.conf.d/tutti.conf.sample
# the LAN container network
	${INSTALL_DATA} ${WRKDIR}/lan.conflist \
		${STAGEDIR}${PREFIX}/etc/cni/net.d/lan.conflist.sample

.include <bsd.port.mk>
