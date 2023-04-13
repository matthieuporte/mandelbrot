#load "graphics.cma";;
#use "topfind";;
#require "graphics";;

open Graphics;;
open Random;;
self_init;;
open_graph " 1200x800";;

(* montains *)

let mountain n (x1,y1) (x2,y2) =
  clear_graph ();
  set_color black;
  let rec rec_m n (x1,y1) (x2,y2) =
      let (midx,midy) = ((x1+x2)/2,(y1+y2)/2 + int((30 * n+1))) in
    if n > 0 then begin
      rec_m (n-1) (x1,y1) (midx,midy);
      rec_m (n-1) (midx,midy) (x2,y2) end
    else begin
      moveto x1 y1;
      lineto midx midy;
      lineto x2 y2 end in
  rec_m n (x1,y1) (x2,y2);;


let m n = mountain n (0,0) (950,0);;

m (int(4)+4);;
m (int(4)+4);;
m (int(4)+4);;
m (int(4)+4);;
m (int(4)+4);;

(* dragon *)

let dragon n (x1,y1) (x2,y2) =
  clear_graph ();
  set_color red;
  let rec rec_d n (x1,y1) (x2,y2) =
      let (midx,midy) = ((x1+x2)/2 + (y2-y1)/2,(y1 + y2)/2 - (x2 - x1)/2) in
    if n > 0 then begin
      rec_d (n-1) (x1,y1) (midx,midy);
      rec_d (n-1) (x2,y2) (midx,midy) end
    else begin
      moveto x1 y1;
      lineto midx midy;
      lineto x2 y2 end in
  rec_d n (x1,y1) (x2,y2);;


dragon 18 (150,150) (350,350);;

(* Carpet *)

let carpet n (x,y) =
  clear_graph ();
  if n < 0 then
    invalid_arg "carpet : n cannot be inferior to 0"
  else
    let drawSquare x y s =
      let x = int_of_float x and
          y = int_of_float y and
          midx = int_of_float (x+.s/.3.) and
          midy = int_of_float (y+.s/.3.) and
          s = int_of_float s and
          ss = int_of_float (s/.3.) in
    set_color black;
    fill_rect x y s s;
    set_color white;
    fill_rect midx midy ss ss  in
  
  let rec rec_c n (x,y) = (* n (x,y) *)
    let ss (* small size *) = n/.3. in
    if n <= 9. then drawSquare x y n else begin
        drawSquare x y n;
        let ss2 = ss*.2. in
        let x0 = x and x1 = x+.ss and x2 = x+.ss2 and
            y0 = y and y1 = y+.ss and y2 = y+.ss2 in
      rec_c (n/.3.) (x0,y0);
      rec_c (n/.3.) (x0,y1);
      rec_c (n/.3.) (x0,y2);
      rec_c (n/.3.) (x1,y0);
      rec_c (n/.3.) (x1,y2);
      rec_c (n/.3.) (x2,y0);
      rec_c (n/.3.) (x2,y1);
      rec_c (n/.3.) (x2,y2); end in
  
  rec_c (float_of_int(n)) ((float_of_int x),(float_of_int y)) ;;

carpet 600 (10,10);;
carpet 1200 (10,10);;
carpet 729 (10,10);;
carpet 243 (10,10);;


(* triangle *)



let sierpinski n (x,y) s =
  clear_graph ();
  if n < 0 then
    invalid_arg "triangle : n cannot be inferior to 0"
  else
    (* height of the equilateral triangle *)
    let intH y s = (int_of_float(float_of_int(y)+.(sqrt(3.)/.2.*.float_of_int(s)))) and
        floatH y s = y+.(sqrt(3.)/.2.*.s) in
    let drawTriangle x y s =
      let x = int_of_float x and
          y = int_of_float y and
          s = int_of_float s in
      set_color black;
      moveto x y;
      lineto (x+s) y;
      lineto ((x+s+x)/2) (intH y s);
      lineto x y in
    let rec rec_t n x y s =
      if n = 0 then drawTriangle x y s else begin
          (* drawTriangle x y s;  (* can be added *)*)
        rec_t (n-1) x y (s/.2.);
        rec_t (n-1) (x+.s/.2.) y (s/.2.);
        rec_t (n-1) (x+.s/.4.) (floatH y (s/.2.)) (s/.2.) end in
    rec_t n (float_of_int(x)) (float_of_int(y)) (float_of_int(s));;


sierpinski 2 (10,10) 500;;
sierpinski 4 (10,10) 500;;
sierpinski 6 (10,10) 500;;
sierpinski 8 (10,10) 500;;
sierpinski 12 (10,10) 500;;


(* cercles *)

let four_circles r (x,y) limit =
  clear_graph ();
  set_color black;
  let rec rec_c r x y n =
    if n = 0 then draw_circle x y r else begin
        draw_circle x y r;
        rec_c (r/2) (x+r/2) y (n-1);
        rec_c (r/2) (x-r/2) y (n-1);
        rec_c (r/2) x (y+r/2) (n-1);
        rec_c (r/2) x (y-r/2) (n-1); end in
  rec_c r x y limit;;

four_circles 200 (250,250) 1;;
four_circles 200 (250,250) 2;;
four_circles 200 (250,250) 3;;
four_circles 200 (250,250) 4;;
four_circles 200 (250,250) 5;;


(* arrow *)

let arrow r (x,y) o limit =
  clear_graph ();
  set_color black;
  if limit < 0 then
    invalid_arg "arrow : limit cannot be inferior to 0"
  else
    let rec rec_a r x y o n =
      if n = 0 then fill_circle x y r else begin
          fill_circle x y r;
          if o <> 'N' then rec_a (r/2) x (y-r*15/10) 'S' (n-1) else ();
          if o <> 'S' then rec_a (r/2) x (y+r*15/10) 'N' (n-1) else ();
          if o <> 'O' then rec_a (r/2) (x-r*15/10) y 'E' (n-1) else ();
          if o <> 'E' then rec_a (r/2) (x+r*15/10) y 'O' (n-1) else ()
        end in
    rec_a r x y o limit;;

arrow 80 (250,250) 'N' 6;;
arrow 80 (250,250) 'E' 6;;
arrow 80 (250,250) 'S' 6;;
arrow 80 (250,250) 'O' 6;;


(* pytagora_tree *)

let square (x1,y1) (x2,y2)  =
  set_color red;
  moveto x1 y1;
  lineto x2 y2;
  lineto (x2 - (y2-y1)) (y2 + (x2-x1));
  lineto (x1 - (y2-y1)) (y1 +(x2 - x1));
  lineto x1 y1;;

let pytagora_tree  n (x,y) size =
  clear_graph ();
  square (x,y) ((x+size),y);
  let rec rec_p n (x1,y1) (x2,y2) =
    let a = (x1 +. x2) /. 2. -. (y2 -. (y2 +. y1) /.2.) and
        b = (y1 +. y2) /. 2. +. (x2 -. (x2 +. x1) /.2.) in
    let c = x1 -. (b -. y1) and
        d = y1 +. (a -. x1) and
        e = a -. (b -. y1) and
        f = b +. (a -. x1) and
        g = a -. (y2 -. b) and
        h = b +. (x2 -. a) and
        i = x2 -. (y2 -. b) and
        j = y2 +. (x2 -. a) in
    let draw_pyth (x1,y1) (x2,y2) = (* takes floats *)
      let go x y = lineto (int_of_float x) (int_of_float y) and
          fly x y = moveto (int_of_float x) (int_of_float y) in
      set_color blue;
      fly x2 y2;
      go i j;
      go g h;
      go a b;
      go x2 y2;
      set_color red;
      fly x1 y1;
      go a b;
      go e f;
      go c d;
      go x1 y1 in
    if n = 1 then draw_pyth (x1,y1) (x2,y2) else begin
        draw_pyth (x1,y1) (x2,y2);
        rec_p (n-1) (c,d) (e,f);
        rec_p (n-1) (g,h) (i,j);
      end in
  if n > 0 then
    rec_p n (float_of_int(x), float_of_int(y+size)) (float_of_int(x+size), float_of_int(y+size))
  else ();;

pytagora_tree 18 (850,50) 220;;


(* Vicsek *)

let vicsek n (x,y) size =
  clear_graph ();
  let five_squares x y s =
    let ss = (s/.3.) and ss2 = (s/.3.*.2.) in
    let fill x y s = fill_rect (int_of_float(x)) (int_of_float(y)) (int_of_float(s))  (int_of_float(s)) in
    fill x y ss;
    fill (x+.ss) (y+.ss) ss;
    fill (x+.ss2) y ss;
    fill (x+.ss2) (y+.ss2) ss;
    fill x (y+.ss2) ss in
  let rec rec_v n x y s =
    let ss = (s/.3.) and ss2 = (s/.3.*.2.) in
    if n = 0 then five_squares x y s else begin
        rec_v (n-1) (x)  (y)  (ss);
        rec_v (n-1) (x+.ss)  (y+.ss)  (ss);
        rec_v (n-1) (x+.ss2)  (y)  (ss);
        rec_v (n-1) (x)  (y+.ss2)  (ss);
        rec_v (n-1) (x+.ss2)  (y+.ss2)  (ss) end in
  rec_v n (float_of_int(x)) (float_of_int(y)) (float_of_int(size));;


vicsek 3 (50,50) 400;;

(* koch *)

let koch_curve n (x,y) (z,t) =
  let rec rec_k n (xa,ya) (xe,ye) =
    let xb = xa +. (xe -. xa)/.3. and yb = ya +. (ye -. ya)/.3. and
        xd = xa +. (xe -. xa)/.3.*.2. and yd = ya +. (ye -. ya)/.3.*.2. in
    let vBDx = (xd -. xb) and vBDy = (yd -. yb) in (* vector from b to d *)
    let xc = xb +. vBDx/.2. -. vBDy*.sqrt(3.)/.2. and
        yc = yb +. vBDy/.2. +. vBDx*.sqrt(3.)/.2. in
    if n > 0 then begin
        rec_k (n-1) (xa,ya) (xb,yb);
        rec_k (n-1) (xb,yb) (xc,yc);
        rec_k (n-1) (xc,yc) (xd,yd);
        rec_k (n-1) (xd,yd) (xe,ye) end
    else begin
        moveto (int_of_float xa) (int_of_float ya);
        lineto (int_of_float xb) (int_of_float yb);
        lineto (int_of_float xc) (int_of_float yc);
        lineto (int_of_float xd) (int_of_float yd);
        lineto (int_of_float xe) (int_of_float ye) end in
  rec_k n (x,y) (z,t);;

let koch_snowflake n (x,y) s =
  set_color black;
  let xA = float_of_int x and yA = float_of_int y in
  let xB = xA+.(float_of_int s) and yB = yA in
  let vABx = (xB -. xA) and vABy = (yB -. yA) in (* vector from A to B *)
  let xC = xA +. vABx/.2. -. vABy*.sqrt(3.)/.2. and
      yC = yA +. vABy/.2. +. vABx*.sqrt(3.)/.2. in 
  clear_graph();
  koch_curve n (xB,yB) (xA,yA);
  koch_curve n (xA,yA) (xC,yC);
  koch_curve n (xC,yC) (xB,yB);;


koch_snowflake 0 (500,300) 800;;
koch_snowflake 1 (500,300) 800;;
koch_snowflake 2 (500,300) 800;;
koch_snowflake 3 (500,300) 800;;
koch_snowflake 4 (500,300) 800;;


(* small xC,yC means the coordinates in the complex plan whereas aX,aY represent actual coordinates on the graph*) 

let mandelbrot n (xC,yC) size  = (* int -> float*float -> float -> unit *)
  clear_graph ();
  let size_graph = 1000 in (* feel free to modify this value depending of your screen size*)
  let f_size_graph = float_of_int size_graph in
  let couleur_pic original_n (x,y) = (* int -> int * int -> unit *)
    let xc = (float_of_int x)/.(f_size_graph/.size) +. xC and
        yc = (float_of_int y)/.(f_size_graph/.size) +. yC in
    (* optimisation checkups *)
    let p = sqrt((xc-.0.25)*.(xc-.0.25)+.yc*.yc) in
    if xc < p -. 2.*.p*.p +. 0.25 || (xc +. 1.)*.(xc +. 1.) +. yc*.yc < 1./.16. then
      begin (* we already know that the point belongs to mandelbrot, no need to check it recursively *)
        set_color (rgb 0 0 0);
        plot x y end else begin
      let rec rec_c n (xz,yz)  = (* int -> float * float -> unit *)
        let zcx = xc +. xz and zcy = yc +.yz in (* addition of z and c *)
        let modulezcsquared = ((zcx*.zcx) *. (zcy*.zcy)) in (* calcul of the module *)
        if modulezcsquared > 4. (* 4 is 2**2 to save the sqrt call*)  then begin (* if the module is bigger than 2 then it's not borned *)
            set_color (rgb (240-240*n/original_n) (255-250*n/original_n) (255-230*n/original_n)); (* WORK TO DO HERE WITH GRADIENTS*)
            plot x y end
        else if n <= 1 then begin (* if the iteration goes further than n we assume that it's borned *)
            set_color (rgb 0 0 0);
            plot x y end
        else rec_c (n-1) (((xz*.xz-.yz*.yz)+.xc),((xz*.yz*.2.)+.yc)) in (* call with n-1 c and z**2+c (zn+1) *)
      rec_c original_n (0.,0.) end in (* z0 + c *)
  let rec column aY aX = (* int -> int -> unit *)
    if aY > 0 then begin
        couleur_pic n (aX,aY);
        column (aY-1) aX end
    else () in
  let rec allColumns aY aX = (* int -> int -> unit *)
    if aX > 0 then begin
        column aY aX;
        allColumns aY (aX-1) end
    else () in
  allColumns size_graph size_graph;;


mandelbrot 70 (-2.,-1.5) 3.;;
(* nice zooms in *)
mandelbrot 100 (-0.4,0.5) 0.5;;
mandelbrot 300 (-0.15,0.6) 0.2;;
mandelbrot 300 (-0.05,0.68) 0.05;;
mandelbrot 300 (-0.03,0.700) 0.025;;
mandelbrot 300 (-0.0235,0.7075) 0.005;;
