PORTNAME=	bwping
DISTVERSION=	2.6
CATEGORIES=	net
MASTER_SITES=	SF

MAINTAINER=	oleg.derevenetz@gmail.com
COMMENT=	Tool to measure bandwidth and RTT between two hosts using ICMP
WWW=		https://bwping.sourceforge.io/

LICENSE=	BSD2CLAUSE
LICENSE_FILE=	${WRKSRC}/COPYING

GNU_CONFIGURE=	yes
INSTALL_TARGET=	install-strip

PLIST_FILES=	sbin/bwping sbin/bwping6 share/man/man8/bwping.8.gz share/man/man8/bwping6.8.gz

.include <bsd.port.mk>
