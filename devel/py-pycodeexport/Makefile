PORTNAME=	pycodeexport
DISTVERSION=	0.1.3
PORTREVISION=	2
CATEGORIES=	devel python
MASTER_SITES=	PYPI
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	yuri@FreeBSD.org
COMMENT=	Python package for code generation
WWW=		https://github.com/bjodah/pycodeexport

LICENSE=	BSD2CLAUSE
LICENSE_FILE=	${WRKSRC}/LICENSE

RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}mako>=1.0.0:textproc/py-mako@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}pycompilation>=0.4.0:devel/py-pycompilation@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}sympy>=0.7.5:math/py-sympy@${PY_FLAVOR}

USES=		python
USE_PYTHON=	distutils cython_run autoplist
NO_ARCH=	yes

.include <bsd.port.mk>
