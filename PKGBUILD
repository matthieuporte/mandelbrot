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
sha256sums=('908cc1cfebc23fc3b9f492f69c1a4cf36bc80a420aa1a21ae9c5997c9533b9b2')

build() {
    cd "$srcdir/$pkgname-$pkgver"

    make
}

package() {
    cd "$srcdir/$pkgname-$pkgver"

    make DESTDIR="$pkgdir/" install

    cd "$srcdir/.."
    pwd
    mkdir -p "usr/share/applications"
    cp mandelbrot-explorer.desktop "usr/share/applications"
}
