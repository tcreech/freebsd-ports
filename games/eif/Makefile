PORTNAME=	eif
PORTVERSION=	1.3.4
PORTREVISION=	5
CATEGORIES=	games
MASTER_SITES=	http://www.stack.nl/~marcolz/empire/ \
		ftp://ftp.stack.nl/pub/users/johans/eif/

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Empire text client
WWW=		https://www.stack.nl/~marcolz/empire/

GNU_CONFIGURE=	yes
GNU_CONFIGURE_MANPREFIX=${PREFIX}/share
USES=		ncurses readline shebangfix
SHEBANG_FILES=	contrib/*.pl

.include <bsd.port.pre.mk>

post-patch:
	@${REINPLACE_CMD} -e '/curout_isapipe/d' ${WRKSRC}/src/data.h
	@${REINPLACE_CMD} -e '/cd .(pkglibdir)/s/cd /&$$(DESTDIR)/' \
		${WRKSRC}/contrib/Makefile.in
	@${REINPLACE_CMD} -e 's@lcurses@lncurses@g' ${WRKSRC}/configure

.include <bsd.port.post.mk>
