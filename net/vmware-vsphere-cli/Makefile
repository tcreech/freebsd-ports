PORTNAME=		vmware-vsphere-cli
PORTVERSION=		6.7.0
CATEGORIES=		net perl5
MASTER_SITES=		#http://communities.vmware.com/community/vmtn/developer/downloads
DISTNAME=		VMware-vSphere-Perl-SDK-${PORTVERSION}-${DISTREL}.i386

MAINTAINER=		zi@FreeBSD.org
COMMENT=		VMware vSphere SDK for Perl with CLI tools
WWW=		http://communities.vmware.com/community/vmtn/server/vsphere/automationtools/vsphere_cli

# Converted from RESTRICTED
LICENSE=	VMware
LICENSE_NAME=	vmware
LICENSE_TEXT=	May not be redistributed.  Must accept license to download.
LICENSE_PERMS=	auto-accept

BUILD_DEPENDS=		p5-Class-MethodMaker>=0:devel/p5-Class-MethodMaker \
			p5-Crypt-SSLeay>=0:security/p5-Crypt-SSLeay \
			p5-XML-LibXML>=0:textproc/p5-XML-LibXML \
			p5-UUID>=0:devel/p5-UUID \
			p5-Archive-Zip>=0:archivers/p5-Archive-Zip \
			p5-SOAP-Lite>=0:net/p5-SOAP-Lite
RUN_DEPENDS=		p5-Class-MethodMaker>=0:devel/p5-Class-MethodMaker \
			p5-Crypt-SSLeay>=0:security/p5-Crypt-SSLeay \
			p5-XML-LibXML>=0:textproc/p5-XML-LibXML \
			p5-UUID>=0:devel/p5-UUID \
			p5-Archive-Zip>=0:archivers/p5-Archive-Zip \
			p5-SOAP-Lite>=0:net/p5-SOAP-Lite \
			p5-Text-Template>0:textproc/p5-Text-Template \
			p5-Time-Piece>0:devel/p5-Time-Piece \
			p5-Path-Class>0:devel/p5-Path-Class \
			p5-version>0:devel/p5-version \
			p5-Data-Dumper>0:devel/p5-Data-Dumper \
			p5-HTML-Parser>0:www/p5-HTML-Parser \
			p5-XML-SAX>0:textproc/p5-XML-SAX \
			p5-XML-NamespaceSupport>0:textproc/p5-XML-NamespaceSupport \
			p5-XML-LibXML>0:textproc/p5-XML-LibXML \
			p5-libwww>0:www/p5-libwww \
			p5-LWP-Protocol-https>0:www/p5-LWP-Protocol-https \
			p5-Socket6>0:net/p5-Socket6 \
			p5-IO-Socket-INET6>0:net/p5-IO-Socket-INET6 \
			p5-Net-INET6Glue>0:net/p5-Net-INET6Glue \
			p5-Net-HTTP>0:net/p5-Net-HTTP

USES=			perl5 shebangfix
USE_PERL5=		configure
NO_PACKAGE=		May not be redistributed.  Must accept license to download.
SHEBANG_FILES=		apps/general/* apps/host/* apps/performance/* apps/session/* apps/vm/* bin/*

DISTREL=		8156551

.include <bsd.port.pre.mk>

.if (!exists(${DISTDIR}/${DISTNAME}${EXTRACT_SUFX}))
DISTFILE_INSTRUCTIONS=\n\
Due to licensing restrictions, certain files must be fetched manually.\n\n\
Please visit https://developer.broadcom.com/sdks/vsphere-perl-sdk/${PORTVERSION:C/^([0-9]+\.[0-9]).*/\1/}\n\
and choose Download.  Then, login and accept the license agreement.\n\
Choose the download for VMware-vSphere-Perl-SDK-${PORTVERSION}-${DISTREL}.i386.tar.gz (even if you are not on i386)\n\n\
After the download has completed, please place ${DISTNAME}${EXTRACT_SUFX} in\n\
${DISTDIR}, then restart the build.\n\n
.endif

.if defined(DISTFILE_INSTRUCTIONS)
.if defined(PACKAGE_BUILDING)
IGNORE=		requires manual fetch
.else
pre-fetch:
	@${PRINTF} "${DISTFILE_INSTRUCTIONS}"
	@exit 1
.endif
.endif

post-extract:
	@${MV} ${WRKDIR}/${PORTNAME}${PKGNAMESUFFIX}-distrib ${WRKSRC}
	@${RM} ${WRKSRC}/vmware-install.pl
	@${RM} -r ${WRKSRC}/lib/VMware/share/VMware/pyexe

post-patch:
	@${REINPLACE_CMD} -e 's|%%EXAMPLESDIR%%|${STAGEDIR}${EXAMPLESDIR}|g' ${WRKSRC}/Makefile.PL
	@${FIND} ${WRKSRC} -name \*.orig -delete

post-install:
	${MKDIR} ${STAGEDIR}${PREFIX}/${SITE_PERL_REL}/AppUtil
	(cd ${WRKSRC}/apps/AppUtil && ${COPYTREE_SHARE} . ${STAGEDIR}${PREFIX}/${SITE_PERL_REL}/AppUtil)
	${INSTALL_SCRIPT} ${WRKSRC}/bin/vicfg-* ${STAGEDIR}${PREFIX}/bin
	${INSTALL_SCRIPT} ${WRKSRC}/bin/svmotion ${STAGEDIR}${PREFIX}/bin
	${INSTALL_SCRIPT} ${WRKSRC}/bin/vmware-cmd ${STAGEDIR}${PREFIX}/bin
	${INSTALL_SCRIPT} ${WRKSRC}/bin/vmkfstools ${STAGEDIR}${PREFIX}/bin
	${INSTALL_SCRIPT} ${WRKSRC}/bin/vifs ${STAGEDIR}${PREFIX}/bin
	${INSTALL_SCRIPT} ${WRKSRC}/bin/vihostupdate ${STAGEDIR}${PREFIX}/bin
	${INSTALL_SCRIPT} ${WRKSRC}/bin/vihostupdate35 ${STAGEDIR}${PREFIX}/bin
	${INSTALL_SCRIPT} ${WRKSRC}/bin/viperl-support ${STAGEDIR}${PREFIX}/bin
	${INSTALL_SCRIPT} ${WRKSRC}/apps/host/* ${STAGEDIR}${PREFIX}/bin
	${INSTALL_SCRIPT} ${WRKSRC}/apps/general/* ${STAGEDIR}${PREFIX}/bin
	${INSTALL_SCRIPT} ${WRKSRC}/apps/vm/* ${STAGEDIR}${PREFIX}/bin

.include <bsd.port.post.mk>
