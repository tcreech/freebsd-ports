PORTNAME=	travis
PORTVERSION=	1.14.0
PORTREVISION=	1
CATEGORIES=	devel rubygems
MASTER_SITES=	RG

MAINTAINER=	ruby@FreeBSD.org
COMMENT=	CLI and Ruby client library for Travis CI
WWW=		https://github.com/travis-ci/travis.rb

LICENSE=	MIT
LICENSE_FILE=	${WRKSRC}/LICENSE

RUN_DEPENDS=	rubygem-faraday>=2.7.10<3:www/rubygem-faraday \
		rubygem-faraday-rack>=2<3:www/rubygem-faraday-rack \
		rubygem-highline2>=2.1.0<2.2:devel/rubygem-highline2 \
		rubygem-json_pure>=2.6.3<3:devel/rubygem-json_pure \
		rubygem-launchy2>=2.5.2<2.6:devel/rubygem-launchy2 \
		rubygem-pusher-client>=0.6.2<0.7:www/rubygem-pusher-client \
		rubygem-rack-test>=2.1.0<3:www/rubygem-rack-test \
		rubygem-travis-gh>=0.21.0<0.22:devel/rubygem-travis-gh

USES=		gem

NO_ARCH=	yes

PLIST_FILES=	bin/travis

.include <bsd.port.mk>
