pkgname="mandelbrot-explorer"
pkgver=1
pkgrel=1
epoch=
pkgdesc="A simple mandelbrot explorer written in C with gtk"
arch=('x86_64')
url="https://github.com/matthieuporte/mandelbrot"
license=('GPL')
groups=()
depends=('gtk3')
makedepends=()
checkdepends=()
optdepends=()
provides=()
conflicts=()
replaces=()
backup=()
options=()
install=
changelog=
source=("$pkgname-$pkgver.tar.gz")
noextract=()
sha256sums=("b67ef8fa26e7753ad71d8f00debcddcbcd574603beee7f13142353f0b3da1976")
validpgpkeys=()

prepare() {
	cd "$pkgname-$pkgver"
	#patch -p1 -i "$srcdir/$pkgname-$pkgver.patch"
}

build() {
	cd "$pkgname-$pkgver"
	./configure --prefix=/usr
	make
}

package() {
	cd "$pkgname-$pkgver"
	make DESTDIR="$pkgdir/" install
}
