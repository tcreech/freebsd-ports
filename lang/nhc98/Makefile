PORTNAME=	nhc98
PORTVERSION=	1.22
PORTREVISION=	10
CATEGORIES=	lang haskell
MASTER_SITES=	http://haskell.org/${PORTNAME}/
DISTFILES=	${PORTNAME}src-${PORTVERSION}${EXTRACT_SUFX}
DIST_SUBDIR=	nhc98

MAINTAINER=	haskell@FreeBSD.org
COMMENT=	Fully-fledged compiler for Haskell 98
WWW=		https://www.haskell.org/nhc98/

DEPRECATED=			only for i386, latest release in 2010, does not build with GCC 14
EXPIRATION_DATE=	2025-02-21

ONLY_FOR_ARCHS=	i386

USES=		gmake
USE_GCC=	13

HAS_CONFIGURE=		yes
CONFIGURE_ARGS+=	--prefix=${PREFIX} --heap=4M --buildwith=gcc
MAKE_ARGS+=		ARCH= MANDIR=${PREFIX}/share/man/man1
MAKE_JOBS_UNSAFE=	yes

OPTIONS_DEFINE=		DOCS

DOCS_CONFIGURE_ON=	--docdir=${DOCSDIR} +docs
DOCS_CONFIGURE_OFF=	-docs

PORTDOCS=	*
CONFLICTS=	hs-cpphs

post-patch:
	@${REINPLACE_CMD} -e 's|-O3|${CFLAGS}|' ${WRKSRC}/Makefile.inc
# The Makefile defines target names based on $(CC) which is
# problematic when CC=gcc5.  The following patch does not affect the
# compiler used during the build, but makes sure that targets have
# the correct suffix e.g. 'basic-gcc' instead of 'basic-gcc5':
	@${REINPLACE_CMD} -e 's|$$(CC)|gcc|g' ${WRKSRC}/Makefile

post-configure:
	@${ECHO} "LIBDIR=${PREFIX}/lib/nhc98" \
		>> ${WRKSRC}/targets/ix86-FreeBSD/config.cache

.include <bsd.port.mk>
