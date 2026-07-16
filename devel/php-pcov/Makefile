PORTNAME=	pcov
DISTVERSIONPREFIX=	v
DISTVERSION=	1.0.12
CATEGORIES=	devel
PKGNAMEPREFIX=	${PHP_PKGNAMEPREFIX}

MAINTAINER=	einar@isnic.is
COMMENT=	Self contained php-code-coverage compatible driver for PHP
WWW=		https://pecl.php.net/package/pcov \
		https://github.com/krakjoe/pcov

LICENSE=	PHP301
LICENSE_FILE=	${WRKSRC}/LICENSE

LIB_DEPENDS=	libpcre2-8.so:devel/pcre2

USES=		php:ext localbase pkgconfig
USE_GITHUB=	yes
GH_ACCOUNT=	krakjoe
IGNORE_WITH_PHP=	86

CONFIGURE_ARGS=	--enable-pcov

#CFLAGS+=	-I${PREFIX}/include

.include <bsd.port.mk>
