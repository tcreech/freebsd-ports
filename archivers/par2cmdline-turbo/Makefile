PORTNAME=	par2cmdline-turbo
PORTVERSION=	1.3.0
DISTVERSIONPREFIX=	v
CATEGORIES=	archivers

MAINTAINER=	martymac@FreeBSD.org
COMMENT=	Tools for using Parity v2 Archive files using ParPar backend
WWW=		https://github.com/animetosho/par2cmdline-turbo

LICENSE=	GPLv2

ONLY_FOR_ARCHS=	aarch64 amd64 i386
ONLY_FOR_ARCHS_REASON=	untested on other archs

USES=		autoreconf compiler:c++11-lang
GNU_CONFIGURE=	yes
GNU_CONFIGURE_MANPREFIX=${PREFIX}/share
LDFLAGS+=	-lpthread

USE_GITHUB=	yes
GH_ACCOUNT=	animetosho

PORTDOCS=	README.md
CONFLICTS=	par2cmdline

PLIST_FILES=	bin/par2 bin/par2create bin/par2repair bin/par2verify \
	share/man/man1/par2.1.gz

OPTIONS_DEFINE=	DOCS

TEST_TARGET=	check

post-install-DOCS-on:
	${MKDIR} ${STAGEDIR}${DOCSDIR} && \
		${INSTALL_DATA} ${PORTDOCS:S|^|${WRKSRC}/|} ${STAGEDIR}${DOCSDIR}

.include <bsd.port.mk>
