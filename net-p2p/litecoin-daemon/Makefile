MASTERDIR=	${.CURDIR}/../litecoin
PORTREVISION=	1
PKGNAMESUFFIX=	-daemon

COMMENT=	Virtual Peer-to-Peer Currency (Daemon)

SLAVE_PORT=	yes

OPTIONS_DEFINE=		DEBUG HARDENING TESTS UPNP ZMQ
OPTIONS_DEFAULT=	HARDENING UPNP WALLET_BDBMODERN ZMQ

USES+=		cpe
CPE_VERSION=	${PORTVERSION:R}

USERS=		litecoin
GROUPS=		litecoin

CONFIGURE_ARGS=	--with-daemon \
		--without-gui \
		--without-libs \
		--without-qrencode \
		--without-utils

PLIST_FILES=	bin/litecoind
PLIST_FILES+=	share/man/man1/litecoind.1.gz
PLIST_FILES+=	"@sample etc/litecoin.conf.sample"

USE_RC_SUBR=	litecoind

post-install:
	${INSTALL_DATA} ${WRKSRC}/share/examples/litecoin.conf ${STAGEDIR}${PREFIX}/etc/litecoin.conf.sample

.include "${MASTERDIR}/Makefile"
