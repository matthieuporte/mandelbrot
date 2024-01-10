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
sha256sums=('4ae95df5b9d28dbc9a7b03f77d4ec87b0e5e004abd4c9001f27e6dd4063e5ad8')

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
