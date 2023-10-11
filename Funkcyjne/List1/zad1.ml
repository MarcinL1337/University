(* int -> int *)
fun x -> if true then x else 0;;

(* (’a -> ’b) -> (’c -> ’a) -> ’c -> ’b *)
fun f -> fun g x -> f(g(x));;

(* ’a -> ’b -> ’a *)
fun x -> fun y -> x;;

(* ’a -> ’a -> ’a *)
fun x y -> if true then x else y;;

(* 'a *)
