# mandelbrot
One of our first tp at EPITA was to draw some fractals in ocaml. One I'm proud of is mandelbrot. The mandelbrot function allows you to really zoom in the fractal which makes it much more interesting. It works like this : 
```ocaml
mandelbrot [int : degree of prescision] 
[float * float : x and y coords of the bottom left corner]
[float : size (the smaller the size the bigger the zoom)]
```
It is the last function of the file fractals.ml

Enjoy !

[mandelbrot]: "https://matthieuporte.github.io/assets/images/mandelbrot.png"

![what mandelbrot can look like when you zoom in][mandelbrot]
