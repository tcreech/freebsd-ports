PORTNAME=	yapps2
PORTVERSION=	2.1.1
PORTREVISION=	2
CATEGORIES=	devel python
MASTER_SITES=	http://theory.stanford.edu/~amitp/yapps/
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}
DISTNAME=	yapps${PORTVERSION}

MAINTAINER=	xenophon+fbsdports@irtnog.org
COMMENT=	Easy-to-use parser generator that generates Python code
WWW=		https://theory.stanford.edu/~amitp/yapps/

PORTSCOUT=	skipv:2-2.1.1-17.1
WRKSRC=		${WRKDIR}/Yapps-${PORTVERSION}
USES=		python
USE_PYTHON=	distutils autoplist

.include <bsd.port.mk>
