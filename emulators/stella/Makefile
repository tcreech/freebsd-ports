PORTNAME=	stella
PORTVERSION=	7.0
CATEGORIES=	emulators

MAINTAINER=	acm@FreeBSD.org
COMMENT=	Multi-platform Atari 2600 VCS emulator
WWW=		https://stella-emu.github.io/

LICENSE=	GPLv2+
LICENSE_FILE=	${WRKSRC}/License.txt

LIB_DEPENDS=	libpng.so:graphics/png

USES=		compiler:c++11-lib gmake localbase:ldflags sdl tar:xz
USE_SDL=	sdl2
USE_GITHUB=	yes
GH_ACCOUNT=	stella-emu

HAS_CONFIGURE=	yes
CONFLICTS_INSTALL=	rubygem-stella

OPTIONS_DEFINE=	DOCS

do-install:
	${INSTALL_PROGRAM} ${WRKSRC}/stella ${STAGEDIR}${PREFIX}/bin
	@${MKDIR} ${STAGEDIR}${DATADIR}
	${INSTALL_DATA} ${WRKSRC}/src/emucore/stella.pro ${STAGEDIR}${DATADIR}

do-install-DOCS-on:
	@${MKDIR} ${STAGEDIR}${DOCSDIR}
	@cd ${WRKSRC}/docs && ${COPYTREE_SHARE} . ${STAGEDIR}${DOCSDIR}

.include <bsd.port.mk>
