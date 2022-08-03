PORTNAME=	snapcast
DISTVERSIONPREFIX=	v
DISTVERSION=	0.26.0
CATEGORIES=	multimedia audio

MAINTAINER=	yuri@FreeBSD.org
COMMENT=	Multiroom client-server audio player

LICENSE=	GPLv3
LICENSE_FILE=	${WRKSRC}/LICENSE

BUILD_DEPENDS=	${LOCALBASE}/include/boost/asio.hpp:devel/boost-libs
LIB_DEPENDS=	libavahi-common.so:net/avahi-app \
		libexpat.so:textproc/expat2 \
		libFLAC.so:audio/flac \
		libogg.so:audio/libogg \
		libopus.so:audio/opus \
		libvorbis.so:audio/libvorbis \
		libsoxr.so:audio/libsoxr

USES=		cmake compiler:c++17-lang pkgconfig python shebangfix

USE_GITHUB=	yes
GH_ACCOUNT=	badaix

SHEBANG_FILES=	server/etc/plug-ins/meta_mpd.py

OPTIONS_DEFINE=		CLIENT SNDIO PULSEAUDIO
OPTIONS_DEFAULT=	CLIENT SNDIO
OPTIONS_SUB=		yes

CLIENT_DESC=		Build and install client in addition to server
CLIENT_CMAKE_BOOL=	BUILD_CLIENT

PULSEAUDIO_DESC=	Build with pulseaudio backend support
PULSEAUDIO_LIB_DEPENDS=	libpulse.so:audio/pulseaudio

SNDIO_DESC=		Build with sndio(7) backend support
SNDIO_LIB_DEPENDS=	libsndio.so:audio/sndio

post-install: # workaround for https://github.com/badaix/snapcast/issues/1021
	@${CHMOD} +r ${STAGEDIR}${PREFIX}/share/snapserver/plug-ins/meta_mpd.py

.include <bsd.port.mk>
