PORTNAME=	nethack
DISTVERSION=	3.4.3
PORTREVISION=	9
CATEGORIES=	japanese games
MASTER_SITES=	SF
DISTNAME=	${PORTNAME}-${DISTVERSION:S/.//g}-src

PATCH_SITES=	https://ftp.iij.ad.jp/pub/osdn.jp/jnethack/58545/ \
		https://ftp.jaist.ac.jp/pub/sourceforge.jp/jnethack/58545/
PATCHFILES=	jnethack-${DISTVERSION}-0.11.diff.gz
PATCH_DIST_STRIP=	-p1

MAINTAINER=	CQG00620@nifty.ne.jp
COMMENT=	Japanized NetHack
WWW=		https://jnethack.github.io/

LICENSE=	nethack
LICENSE_NAME=	NETHACK GENERAL PUBLIC LICENSE
LICENSE_FILE=	${WRKSRC}/dat/license
LICENSE_PERMS=	dist-mirror dist-sell pkg-mirror pkg-sell auto-accept

USES=		cpe gmake ncurses tar:tgz
MAKE_ENV=	GRAPHICS="${GRAPHICS}"

MAKE_JOBS_UNSAFE=	yes

# Configure these variables as you want them to be.
HACKNAME?=	jnethack
HACKDIR?=	share/${HACKNAME}

DATADIR=	${PREFIX}/${HACKDIR}
DOCSDIR=	${PREFIX}/share/doc/${HACKNAME}
SUB_FILES=	pkg-deinstall
SUB_LIST=	HACKDIR=${HACKDIR}

WRKSRC=		${WRKDIR}/${PORTNAME}-${DISTVERSION}

PLIST_SUB=	HACKNAME="${HACKNAME}"

OPTIONS_DEFINE=		DOCS SHELL X11
OPTIONS_DEFAULT=	SHELL X11

SHELL_DESC=	Shell support

X11_USES=	xorg
X11_USE=	XORG=ice,sm,x11,xaw,xext,xmu,xpm,xt
X11_VARS=	GRAPHICS=X11_GRAPHICS

.include <bsd.port.options.mk>

.if ${OPSYS} == FreeBSD
CFLAGS+=	-Wno-error=incompatible-function-pointer-types
.endif

post-patch:
	@${REINPLACE_CMD} -e 's|%%HACKNAME%%|${HACKNAME}|g' \
		-e 's|%%HACKDIR%%|${PREFIX}/${HACKDIR}|g' \
		${WRKSRC}/include/config.h \
		${WRKSRC}/sys/unix/Makefile.doc
	@${REINPLACE_CMD} -e 's|<malloc.h>|<stdlib.h>|g' \
		${WRKSRC}/win/Qt/qt_win.cpp

post-patch-SHELL-off:
	@${REINPLACE_CMD} -e 's|#define SHELL|/* #define SHELL|' \
		${WRKSRC}/include/unixconf.h

post-patch-X11-off:
	@${REINPLACE_CMD} -e 's|# define XI18N|/* # define XI18N|' \
		${WRKSRC}/include/config.h

do-configure:
	(cd ${WRKSRC}/sys/unix && ${SH} setup.sh)

post-install:
	(cd ${WRKSRC}/doc && \
		${SETENV} ${MAKE_ENV} ${MAKE} ${MAKE_ARGS} manpages)
	${INSTALL_DATA} ${WRKSRC}/win/X11/JNetHack.ad \
		${STAGEDIR}${PREFIX}/lib/X11/app-defaults/JNetHack

post-install-DOCS-on:
	@${MKDIR} ${STAGEDIR}${DOCSDIR}
	(cd ${WRKSRC} && ${INSTALL_DATA} README.j ChangeLog.j INSTALL.j \
		doc/Guidebook.txt doc/jGuidebook.txt ${STAGEDIR}${DOCSDIR})

.include <bsd.port.mk>
