pkgname=mandelbrot-explorer
pkgver=1.0
pkgrel=1
pkgdesc="Mandelbrot Explorer written in C with GTK"
arch=('x86_64')
url="https://github.com/matthieuporte/mandelbrot"
license=('MIT')
depends=('gtk3')

source=("https://github.com/matthieuporte/mandelbrot/archive/v$pkgver.tar.gz")

build() {
  cd "$srcdir/$pkgname-$pkgver"
  make
}

package() {
  cd "$srcdir/$pkgname-$pkgver"
  make DESTDIR="$pkgdir/" install
}
