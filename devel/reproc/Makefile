PORTNAME=	reproc
DISTVERSIONPREFIX=	v
DISTVERSION=	14.2.5
CATEGORIES=	devel

MAINTAINER=	yuri@FreeBSD.org
COMMENT=	C/C++ library to simplify running external programs
WWW=		https://github.com/DaanDeMeyer/reproc

LICENSE=	MIT
LICENSE_FILE=	${WRKSRC}/LICENSE

USES=		cmake compiler:c++11-lang
USE_LDCONFIG=	yes

USE_GITHUB=	yes
GH_ACCOUNT=	DaanDeMeyer

CMAKE_ON=	REPROC++ BUILD_SHARED_LIBS

do-test:
	@cd ${BUILD_WRKSRC} && \
		${SETENV} ${CONFIGURE_ENV} ${CMAKE_BIN} ${CMAKE_ARGS} -DREPROC_TEST:BOOL=ON ${CMAKE_SOURCE_PATH} && \
		${SETENV} ${MAKE_ENV} ${MAKE_CMD} ${MAKE_ARGS} ${ALL_TARGET} && \
		${SETENV} ${MAKE_ENV} ${MAKE_CMD} ${MAKE_ARGS} test

.include <bsd.port.mk>
