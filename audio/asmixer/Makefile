PORTNAME=	asmixer
PORTVERSION=	0.5
PORTREVISION=	4
CATEGORIES=	audio afterstep
MASTER_SITES=	http://tigr.net/afterstep/download/asmixer/

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Mixer control for AfterStep window manager
WWW=		http://www.tigr.net/afterstep/

LICENSE=	GPLv2

USES=		xorg
USE_XORG=	xpm x11
GNU_CONFIGURE=	yes
CONFIGURE_ARGS=	--with-xpm \
		--with-xpm-includes=${LOCALBASE}/include \
		--with-xpm-library=${LOCALBASE}/lib

PLIST_FILES=	bin/asmixer share/man/man1/asmixer.1.gz

post-patch:
.for i in asmixer.c configure autoconf/config.h.in
	@${REINPLACE_CMD} -e \
		's|machine/soundcard.h|sys/soundcard.h| ; \
		 s|MACHINE_SOUNDCARD_H|SYS_SOUNDCARD_H| ; \
		 /sys\/syslimits.h/d' ${WRKSRC}/${i}
.endfor

do-install:
	${INSTALL_PROGRAM} ${WRKSRC}/asmixer ${STAGEDIR}${PREFIX}/bin
	${INSTALL_MAN} ${WRKSRC}/asmixer.man \
		${STAGEDIR}${PREFIX}/share/man/man1/asmixer.1

.include <bsd.port.mk>
