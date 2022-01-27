PORTNAME=	pam-afs-session
PORTVERSION=	2.6
CATEGORIES=	security
MASTER_SITES=	http://archives.eyrie.org/software/afs/

MAINTAINER=	tcreech@umd.edu
COMMENT=	A PAM module for getting AFS tokens

USES=	libtool

FLAVORS=	openafs openafsdevel
default_CONFLICTS_INSTALL+=	pam-afs-session-openafsdevel

openafsdevel_PKGNAMESUFFIX= -openafsdevel

.if ${FLAVOR:U} == openafsdevel
BUILD_DEPENDS=	${LOCALBASE}/include/openafs/afs/param.h:${PORTSDIR}/net/openafs-devel \
			${LOCALBASE}/bin/aklog:${PORTSDIR}/net/openafs-devel
.else
BUILD_DEPENDS=	${LOCALBASE}/include/openafs/afs/param.h:${PORTSDIR}/net/openafs \
			${LOCALBASE}/bin/aklog:${PORTSDIR}/net/openafs
.endif

HAS_CONFIGURE=	yes
CONFIGURE_ARGS+=	--mandir=${LOCALBASE}/man
CPPFLAGS+=	-I${LOCALBASE}/include/openafs
LDFLAGS+=	-L${LOCALBASE}/lib

.include <bsd.port.mk>

