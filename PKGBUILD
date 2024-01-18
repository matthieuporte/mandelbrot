pkgname="mandelbrot-explorer"
pkgver="1.0"
pkgrel="1"
pkgdesc="Mandelbrot Explorer written in C with GTK"
arch=("x86_64")
url="https://github.com/matthieuporte/mandelbrot"
license=("GPL")
depends=("gtk3")

source=("$pkgname-$pkgver.tar.gz")
#below is generated with a makepkg -g
sha256sums=('28fcea9e6d216f03c6d7d1d3c193275fd6585702a4daa7767a14c60d8320273e')

build() {
    cd "$srcdir/$pkgname-$pkgver"

    make
}

package() {
    cd "$srcdir/$pkgname-$pkgver"

    make DESTDIR="$pkgdir/" install

    pwd
    mkdir -p "$pkgdir/usr/share/applications"
    cp "../../mandelbrot-explorer.desktop" "$pkgdir/usr/share/applications"
}
