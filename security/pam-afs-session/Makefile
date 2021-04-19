PORTNAME=	pam-afs-session
PORTVERSION=	2.6
CATEGORIES=	security
MASTER_SITES=	http://archives.eyrie.org/software/afs/

MAINTAINER=	tcreech@umd.edu
COMMENT=	A PAM module for getting AFS tokens

USES=	libtool

FLAVORS=	openafs openafs16 openafsdevel
default_CONFLICTS_INSTALL+=	pam-afs-session-openafs16
default_CONFLICTS_INSTALL+=	pam-afs-session-openafsdevel

openafs16_PKGNAMESUFFIX= -openafs16
openafsdevel_PKGNAMESUFFIX= -openafsdevel

.if ${FLAVOR:U} == openafsdevel
BUILD_DEPENDS=	${LOCALBASE}/include/openafs/afs/param.h:${PORTSDIR}/net/openafs-devel \
			${LOCALBASE}/bin/aklog:${PORTSDIR}/net/openafs-devel
.elif ${FLAVOR:U} == openafs16
BUILD_DEPENDS=	${LOCALBASE}/include/openafs/afs/param.h:${PORTSDIR}/net/openafs16 \
			${LOCALBASE}/bin/aklog:${PORTSDIR}/net/openafs16
.else
BUILD_DEPENDS=	${LOCALBASE}/include/openafs/afs/param.h:${PORTSDIR}/net/openafs \
			${LOCALBASE}/bin/aklog:${PORTSDIR}/net/openafs
.endif

HAS_CONFIGURE=	yes
CONFIGURE_ARGS+=	--mandir=${LOCALBASE}/man
CPPFLAGS+=	-I${LOCALBASE}/include/openafs
LDFLAGS+=	-L${LOCALBASE}/lib

.include <bsd.port.mk>

