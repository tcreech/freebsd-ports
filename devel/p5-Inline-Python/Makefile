PORTNAME=	Inline-Python
PORTVERSION=	0.58
CATEGORIES=	devel perl5
MASTER_SITES=	CPAN
PKGNAMEPREFIX=	p5-

MAINTAINER=	perl@FreeBSD.org
COMMENT=	Write Perl Subroutines in Python
WWW=		https://metacpan.org/release/Inline-Python

LICENSE=	ART10 GPLv1+
LICENSE_COMB=	dual

BUILD_DEPENDS=	${RUN_DEPENDS}
RUN_DEPENDS=	p5-Inline>=0.46:devel/p5-Inline
TEST_DEPENDS=	p5-Proc-ProcessTable>=0.53:devel/p5-Proc-ProcessTable \
		p5-Test-Deep>=0:devel/p5-Test-Deep \
		p5-Test-Number-Delta>=0:devel/p5-Test-Number-Delta

USES=		perl5 python
USE_PERL5=	configure

CONFIGURE_ENV=	INLINE_PYTHON_EXECUTABLE="${PYTHON_CMD}"

post-install:
	${STRIP_CMD} ${STAGEDIR}${PREFIX}/${SITE_ARCH_REL}/auto/Inline/Python/Python.so

.include <bsd.port.mk>
