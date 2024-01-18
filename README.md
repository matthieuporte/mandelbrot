# mandelbrot-explorer

This is a simple mandelbrot explorer made in c with gtk.
It allows for exploration of the mandelbrot set, with customisations features
such as theming and control over the depth of rendering.

I've done other versions, not quite as good :
 - A non-interactive version [in ocaml](https://github.com/matthieuporte/mandelbrot-ocaml)
 - A version using [sdl](https://github.com/matthieuporte/mandelbrot-sdl)


## Tryout the app without installing

```shell
cd mandelbrot-explorer-1.0
make
./mandelbrot-explorer
``` 

## Install as a package on arch

run 
```shell
makepkg
sudo pacman -U mandelbrot-explorer-1.0-1-x84_64.pkg.tar.zst
```

Then you should be able to run the program in a terminal.

If you want to make it a desktop application, please copy `mandelbrot-explorer.desktop`
in `/usr/share/applications`.


## Install as flatpak (for other distros)

---

Enjoy !


[img1]: images/mandelbrot1.png
[img2]: images/mandelbrot2.png
[img3]: images/mandelbrot3.png

![what mandelbrot can look like when you zoom in][img1]
![what mandelbrot can look like when you zoom in][img2]
![what mandelbrot can look like when you zoom in][img3]
