PORTNAME=	train-winrm
PORTVERSION=	0.2.13
CATEGORIES=	net rubygems
MASTER_SITES=	RG

MAINTAINER=	sunpoet@FreeBSD.org
COMMENT=	Allow applictaions using Train to speak to Windows using Remote Management
WWW=		https://github.com/inspec/train-winrm

LICENSE=	APACHE20
LICENSE_FILE=	${WRKSRC}/LICENSE

RUN_DEPENDS=	rubygem-winrm>=2.3.6<3.0:sysutils/rubygem-winrm \
		rubygem-winrm-elevated>=1.2.2<1.3:sysutils/rubygem-winrm-elevated \
		rubygem-winrm-fs>=1.0<2:filesystems/rubygem-winrm-fs

USES=		gem

NO_ARCH=	yes

.include <bsd.port.mk>
