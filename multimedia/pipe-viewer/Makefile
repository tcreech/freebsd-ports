# Created by: tcreech

PORTNAME=	pipe-viewer
PORTVERSION=	0.1.7
CATEGORIES=	multimedia perl5
MASTER_SITES= https://github.com/trizen/${PORTNAME}/archive/refs/tags/${DISTVERSION}${EXTRACT_SUFX}?dummy=/

MAINTAINER=	tcreech@tcreech.com
COMMENT=	A lightweight YouTube client for Linux

LICENSE=	ART20
LICENSE_FILE=	${WRKSRC}/LICENSE

BUILD_DEPENDS=	${RUN_DEPENDS}
RUN_DEPENDS=	p5-Data-Dump>=0:devel/p5-Data-Dump \
		p5-LWP-Protocol-https>=0:www/p5-LWP-Protocol-https \
		p5-JSON>=0:converters/p5-JSON \
		p5-libwww>=0:www/p5-libwww \
		p5-Term-ReadLine-Gnu>=0:devel/p5-Term-ReadLine-Gnu \
		p5-Unicode-LineBreak>=0:textproc/p5-Unicode-LineBreak \
		p5-JSON-XS>=0:converters/p5-JSON-XS

USES=		perl5
USE_PERL5=	configure

NO_ARCH=	yes

.include <bsd.port.mk>
