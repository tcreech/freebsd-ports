PORTNAME=	pyprind
PORTVERSION=	2.11.3
PORTREVISION=	1
CATEGORIES=	misc python
MASTER_SITES=	PYPI
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}
DISTNAME=	PyPrind-${PORTVERSION}

MAINTAINER=	sunpoet@FreeBSD.org
COMMENT=	Python Progress Bar and Percent Indicator Utility
WWW=		https://github.com/rasbt/pyprind

LICENSE=	BSD3CLAUSE
LICENSE_FILE=	${WRKSRC}/LICENSE

BUILD_DEPENDS=	${PYTHON_PKGNAMEPREFIX}setuptools>=0:devel/py-setuptools@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}wheel>=0:devel/py-wheel@${PY_FLAVOR}
TEST_DEPENDS=	${PYTHON_PKGNAMEPREFIX}psutil>=3.2.0:sysutils/py-psutil@${PY_FLAVOR}

USES=		python
USE_PYTHON=	autoplist concurrent pep517 nose

NO_ARCH=	yes

OPTIONS_DEFINE=	PSUTIL
PSUTIL_DESC=	Support monitoring CPU and memory usage

PSUTIL_RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}psutil>=3.2.0:sysutils/py-psutil@${PY_FLAVOR}

.include <bsd.port.mk>
