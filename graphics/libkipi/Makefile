PORTNAME=	libkipi
DISTVERSION=	${KDE_APPLICATIONS_VERSION}
CATEGORIES=	graphics kde kde-applications

MAINTAINER=	kde@FreeBSD.org
COMMENT=	KDE Image Plugin Interface
WWW=		https://www.digikam.org/

IGNORE=		No longer shipped in KDE Release Service 24.08

LICENSE=	GPLv2+

USES=		cmake compiler:c++11-lang gettext kde:5 qt:5 tar:xz
USE_KDE=	auth codecs config configwidgets coreaddons i18n libkdcraw \
		libkexiv2 service widgetsaddons xmlgui \
		ecm:build
USE_QT=		core dbus gui widgets xml \
		buildtools:build qmake:build
USE_LDCONFIG=	yes

.include <bsd.port.mk>
