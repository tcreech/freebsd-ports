PORTNAME=	libdnsres
PORTVERSION=	0.1a
PORTREVISION=	11
CATEGORIES=	devel
MASTER_SITES=	http://monkey.org/~provos/

MAINTAINER=	oz@nixil.net
COMMENT=	Provides a non-blocking thread-safe API for resolving DNS names
WWW=		https://www.monkey.org/~provos/libdnsres/

LICENSE=	BSD4CLAUSE
LICENSE_FILE=	${WRKSRC}/LICENSE

DEPRECATED=	No consumers in tree, last release in 2005 by upstream
EXPIRATION_DATE=2025-06-30

BUILD_DEPENDS=	${LOCALBASE}/lib/libevent.a:devel/libevent

USES=		libtool
GNU_CONFIGURE=	yes
GNU_CONFIGURE_MANPREFIX=${PREFIX}/share
USE_LDCONFIG=	yes
CONFIGURE_ARGS=	--with-libevent=${LOCALBASE}
MAKE_JOBS_UNSAFE=	yes

post-patch:
	@${REINPLACE_CMD} -e 's,CFLAGS =,CFLAGS +=, ; s|top_buildir|top_builddir|g' \
		${WRKSRC}/Makefile.in

.include <bsd.port.mk>
