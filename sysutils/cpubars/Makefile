PORTNAME=	cpubars
DISTVERSION=	g20260720
CATEGORIES= sysutils
MAINTAINER= tcreech@tcreech.com

USE_GITHUB=	yes
GH_ACCOUNT=	aclements
GH_PROJECT=	cpubars
GH_TAGNAME=	ea5b9b4

USES=	gmake ncurses
ALL_TARGET= cpubars
CPPFLAGS+= -D__STDC_ISO_10646__

PLIST_FILES=	bin/cpubars

do-install:
	${INSTALL_SCRIPT} ${WRKSRC}/cpubars ${STAGEDIR}${PREFIX}/bin/cpubars

.include <bsd.port.mk>
