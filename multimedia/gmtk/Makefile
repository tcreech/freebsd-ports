PORTNAME=	gmtk
PORTVERSION=	1.0.9
PORTREVISION=	4
CATEGORIES=	multimedia gnome devel
MASTER_SITES=	https://pkgs.rpmfusion.org/repo/pkgs/free/gmtk/gmtk-1.0.9.tar.gz/9184658014ca7b325b569ac7417e4860/

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Library for gnome-mplayer and gecko-mediaplayer
WWW=		http://code.google.com/p/gmtk/

LICENSE=	GPLv2

DEPRECATED=	Support library for deprecated port multimedia/gnome-mplayer
EXPIRATION_DATE=2025-03-31

BUILD_DEPENDS=	p5-XML-Parser>0:textproc/p5-XML-Parser

USES=		gettext gmake gnome libtool pathfix perl5 pkgconfig xorg
USE_XORG=	x11
USE_GNOME=	glib20 intltool
USE_PERL5=	build
GNU_CONFIGURE=	yes
CONFIGURE_ARGS=	--disable-gconf
INSTALL_TARGET=	install-strip
USE_LDCONFIG=	yes

CPPFLAGS+=	-I${LOCALBASE}/include
LDFLAGS+=	-L${LOCALBASE}/lib

OPTIONS_DEFINE=		ALSA DOCS GIO GTK3 PULSEAUDIO

ALSA_LIB_DEPENDS=	libasound.so:audio/alsa-lib
ALSA_CONFIGURE_WITH=	alsa
GIO_CONFIGURE_WITH=	gio
GTK3_USE=		gnome=gtk30
GTK3_CONFIGURE_ENABLE=	gtk3
PULSEAUDIO_LIB_DEPENDS=	libpulse.so:audio/pulseaudio
PULSEAUDIO_CONFIGURE_WITH=	pulseaudio

.include <bsd.port.options.mk>

.if !${PORT_OPTIONS:MGTK3}
USE_GNOME+=	gtk20
.endif

post-patch:
	@${REINPLACE_CMD} -e \
		'/COPYING\\/d ; \
		 /INSTALL\\/d' ${WRKSRC}/Makefile.in

.include <bsd.port.mk>
