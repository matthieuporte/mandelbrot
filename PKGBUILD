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
sha256sums=("ba55debba3e672516adb6e1b5fcf2fca47af9a9fe97852e31f37a8325843d1cb")
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
