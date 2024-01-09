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
sha256sums=("ec8fc62abfbb1396d3a701aee53e195e63a0875660dd3dfb65ce71db66a577cb")
validpgpkeys=()

prepare() {
	cd "$pkgname-$pkgver"
	#patch -p1 -i "$srcdr/$pkgname-$pkgver.patch"
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
