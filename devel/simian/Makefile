PORTNAME=	simian
PORTVERSION=	2.5.10
CATEGORIES=	devel java
MASTER_SITES=	http://www.harukizaemon.com/simian/

MAINTAINER=	tota@FreeBSD.org
COMMENT=	Similarity analyser for source and other text files
WWW=		https://www.harukizaemon.com/simian/

LICENSE=	SIMIAN
LICENSE_NAME=	SIMIAN SOFTWARE LICENSE
LICENSE_FILE=	${WRKSRC}/license.pdf
# Have to agree to license before download
LICENSE_PERMS=	no-dist-mirror no-dist-sell no-pkg-mirror no-pkg-sell no-auto-accept

NO_BUILD=	yes
NO_WRKSUBDIR=	yes

SUB_FILES=	simian
SUB_LIST+=	VERSION=${PORTVERSION}
PLIST_SUB+=	VERSION=${PORTVERSION}

PORTDOCS=	changes.html features.html \
		get_dotnet.png get_it_now.html \
		get_java_blue-button.gif index.html installation.html \
		simian.dtd simian.jpg simian.xsl \
		javadoc

OPTIONS_DEFINE=	DOCS
OPTIONS_SINGLE=	LANG
OPTIONS_SINGLE_LANG=	JAVA MONO
OPTIONS_SUB=	yes

OPTIONS_DEFAULT=	JAVA

JAVA_USES=	java
MONO_RUN_DEPENDS=	mono:lang/mono${MONO_DEFAULT}

.include <bsd.port.pre.mk>

pre-fetch:
.if !exists(${DISTDIR}/${DISTNAME}${EXTRACT_SUFX})
.if !defined(PACKAGE_BUILDING) && (!defined(SIMIAN_AGREE_LICENSE) || ${SIMIAN_AGREE_LICENSE} != "yes")
	@${ECHO}	By downloading this version, you agree to the terms
	@${ECHO}	and conditions set out in the license agreement and
	@${ECHO}	you agree that, should you wish to continue using
	@${ECHO}	Simian for other than open source projects, non-
	@${ECHO}	commercial projects or evaluation purposes for a
	@${ECHO}	period greater than 15 days, you will purchase a
	@${ECHO}	license as described following URL.
	@${ECHO}
	@${ECHO}	http://www.harukizaemon.com/simian/license.pdf
	@${ECHO}
	@${ECHO}	Build with SIMIAN_AGREE_LICENSE=yes to continue.
	@${ECHO}
	@${FALSE}
.endif
.endif

do-install:
	@${MKDIR} ${STAGEDIR}${PREFIX}/share/xsl/${PORTNAME}
	${INSTALL_DATA} ${WRKSRC}/simian.xsl ${STAGEDIR}${PREFIX}/share/xsl/${PORTNAME}
	@${MKDIR} ${STAGEDIR}${PREFIX}/simian/bin
	${INSTALL_SCRIPT} ${WRKDIR}/simian ${STAGEDIR}${PREFIX}/bin/simian

do-install-JAVA-on:
	${INSTALL_DATA} ${WRKSRC}/bin/simian-${PORTVERSION}.jar ${STAGEDIR}${PREFIX}/simian/bin

do-install-MONO-on:
	${INSTALL_SCRIPT} ${WRKSRC}/bin/*.dll ${STAGEDIR}${PREFIX}/simian/bin
	${INSTALL_SCRIPT} ${WRKSRC}/bin/simian-${PORTVERSION}.exe ${STAGEDIR}${PREFIX}/simian/bin

do-install-DOCS-on:
	cd ${WRKSRC} && ${COPYTREE_SHARE} "${PORTDOCS}" ${STAGEDIR}${DOCSDIR}

.include <bsd.port.post.mk>
