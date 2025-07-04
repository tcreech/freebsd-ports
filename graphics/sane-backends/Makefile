PORTNAME=	sane-backends
DISTVERSION=	1.4.0
PORTREVISION=	1
CATEGORIES=	graphics
MASTER_SITES=	https://gitlab.com/-/project/429008/uploads/843c156420e211859e974f78f64c3ea3/

MAINTAINER=	vvd@FreeBSD.org
COMMENT=	API for accessing scanners, digital cameras, frame grabbers, etc
WWW=		http://www.sane-project.org/

LICENSE=	GPLv2+
LICENSE_FILE=	${WRKSRC}/COPYING

LIB_DEPENDS=	libpng.so:graphics/png \
		libtiff.so:graphics/tiff

USES=		compiler:c++11-lang gmake gnome jpeg libtool localbase:ldflags \
		pathfix pkgconfig python:build
USE_GNOME=	libxml2
USE_LDCONFIG=	yes
USE_RC_SUBR=	saned

GNU_CONFIGURE=	yes
CONFIGURE_ARGS=	--disable-locking \
		--enable-pthread \
		--without-systemd
INSTALL_TARGET=	install-strip

SUB_FILES=	pkg-message

USERS=		saned
GROUPS=		saned

PLIST_SUB=	SOVERSION=${DISTVERSION}

OPTIONS_DEFINE=		AVAHI DOCS GPHOTO IEEE1284 IPV6 NLS SNMP USB V4L
OPTIONS_DEFAULT=	AVAHI GPHOTO USB
OPTIONS_SUB=		yes

IEEE1284_DESC=		Parallel port support via libieee1284 (for i386 only)
USB_DESC=		USB support

AVAHI_LIB_DEPENDS=		libavahi-client.so:net/avahi-app \
				libcurl.so:ftp/curl \
				libpoppler-glib.so:graphics/poppler-glib
AVAHI_USE=			GNOME=cairo,glib20
AVAHI_CONFIGURE_WITH=		avahi libcurl poppler-glib
GPHOTO_LIB_DEPENDS=		libgphoto2.so:graphics/libgphoto2
GPHOTO_CONFIGURE_WITH=		gphoto2
IEEE1284_LIB_DEPENDS=		libieee1284.so:sysutils/libieee1284
IEEE1284_CONFIGURE_ENV_OFF=	sane_cv_use_libieee1284=no
IPV6_CONFIGURE_ENABLE=		ipv6
NLS_USES=			gettext-tools
NLS_CONFIGURE_ENABLE=		nls
SNMP_LIB_DEPENDS=		libnetsnmp.so:net-mgmt/net-snmp
SNMP_CONFIGURE_WITH=		snmp
USB_CONFIGURE_WITH=		usb
V4L_LIB_DEPENDS=		libv4l1.so:multimedia/libv4l
V4L_CONFIGURE_WITH=		v4l

.include <bsd.port.mk>
