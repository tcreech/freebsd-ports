PORTNAME=	pam-afs-session
PORTVERSION=	2.6
CATEGORIES=	security
MASTER_SITES=	http://archives.eyrie.org/software/afs/

MAINTAINER=	tcreech@umd.edu
COMMENT=	A PAM module for getting AFS tokens

USES=	libtool

BUILD_DEPENDS=	${LOCALBASE}/include/openafs/afs/param.h:${PORTSDIR}/net/openafs \
			${LOCALBASE}/bin/aklog:${PORTSDIR}/net/openafs
HAS_CONFIGURE=	yes
CONFIGURE_ARGS+=	--mandir=${LOCALBASE}/man
CPPFLAGS+=	-I${LOCALBASE}/include/openafs
LDFLAGS+=	-L${LOCALBASE}/lib

.include <bsd.port.mk>

