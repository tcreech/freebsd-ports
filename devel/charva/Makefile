PORTNAME=	charva
PORTVERSION=	1.1.4
PORTREVISION=	7
CATEGORIES=	devel java
MASTER_SITES=	SF/${PORTNAME}/${PORTNAME}/${PORTNAME}-${PORTVERSION}
DISTNAME=	${PORTNAME}

MAINTAINER=	ale@FreeBSD.org
COMMENT=	Java Windowing Toolkit for Text Terminals
WWW=		http://www.pitman.co.za/projects/charva/

BUILD_DEPENDS=	gmake:devel/gmake
RUN_DEPENDS=	${JAVALIBDIR}/commons-logging.jar:java/apache-commons-logging

BROKEN_armv6=		fails to compile: build.xml: compilation failed
BROKEN_armv7=		fails to compile: build.xml: compilation failed

OPTIONS_DEFINE=	DOCS

USES=		dos2unix java:ant,run zip
DOS2UNIX_GLOB=	build.xml Makefile.freebsd.txt *.c *.java

ALL_TARGET=	clean makeDLL dist

PLIST_FILES=	${JAVAJARDIR}/${PORTNAME}.jar \
		${JAVA_HOME}/jre/lib/${MACHINE_ARCH}/libTerminal.so
PORTDOCS=	*

DOCS_ALL_TARGET=	javadoc

do-install:
	@${ECHO_MSG} -n ">> Installing JAR file as ${JAVAJARDIR}/${PORTNAME}.jar..."
	@${INSTALL_DATA} ${WRKSRC}/java/dist/lib/${PORTNAME}.jar \
		${STAGEDIR}${JAVAJARDIR}/${PORTNAME}.jar
	@${ECHO_MSG} " [DONE]"
	@${ECHO_MSG} -n ">> Installing JNI library as ${JAVA_HOME}/jre/lib/${MACHINE_ARCH}/libTerminal.so..."
	-@${MKDIR} ${STAGEDIR}${JAVA_HOME}/jre/lib/${MACHINE_ARCH}
	@${INSTALL_PROGRAM} ${WRKSRC}/c/lib/libTerminal.so \
		${STAGEDIR}${JAVA_HOME}/jre/lib/${MACHINE_ARCH}/libTerminal.so
	@${ECHO_MSG} " [DONE]"

do-install-DOCS-on:
	@${ECHO_MSG} -n ">> Installing documentation..."
	@${MKDIR} ${STAGEDIR}${DOCSDIR}
	@cd ${WRKSRC}/docs && ${FIND} * \
	  | ${CPIO} -pdmu -R ${SHAREOWN}:${SHAREGRP} ${STAGEDIR}${DOCSDIR} > /dev/null 2>&1
	@${ECHO_MSG} " [DONE]"

.include <bsd.port.mk>
