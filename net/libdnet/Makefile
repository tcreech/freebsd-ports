PORTNAME=	libdnet
PORTVERSION=	1.13
DISTVERSIONPREFIX=	v
PORTREVISION?=	5
CATEGORIES?=	net

MAINTAINER?=	onatan@gmail.com
COMMENT=	Simple interface to low level networking routines
WWW=		https://github.com/5u623l20/libdnet/

LICENSE=	BSD3CLAUSE
LICENSE_FILE=	${WRKSRC}/LICENSE

LIB_DEPENDS=	libpfctl.so:net/libpfctl

USES?=		autoreconf libtool localbase:ldflags

LDFLAGS+=	-lpfctl

USE_GITHUB=	yes
GH_ACCOUNT=	5u623l20

GNU_CONFIGURE=		yes
GNU_CONFIGURE_MANPREFIX=${PREFIX}/share
CONFIGURE_ARGS+=	--without-check

.if !defined(LIBDNET_SLAVE)
CONFIGURE_ARGS+=	--without-python
USE_LDCONFIG=	yes
.endif

.include <bsd.port.mk>
