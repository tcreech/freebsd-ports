PORTNAME=	liquibase
PORTVERSION=	4.3.5
CATEGORIES=	databases java
MASTER_SITES=	https://github.com/liquibase/liquibase/releases/download/v${PORTVERSION}/

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Database Change Management
WWW=		https://www.liquibase.org/

LICENSE=	APACHE20

USES=		cpe java

NO_ARCH=	yes
NO_BUILD=	yes
NO_WRKSUBDIR=	yes

do-install:
	${INSTALL_SCRIPT} ${WRKSRC}/liquibase ${STAGEDIR}${PREFIX}/bin
	@${MKDIR} ${STAGEDIR}${DATADIR}
	${INSTALL_DATA} ${WRKSRC}/lib/*.jar ${WRKSRC}/liquibase.jar \
		${STAGEDIR}${DATADIR}

.include <bsd.port.mk>
