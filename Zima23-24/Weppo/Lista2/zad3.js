console.log( (![]+[])[+[]]+(![]+[])[+!+[]]+([![]]+[][[]])[+!+[]+[+[]]]+(![]+[])[!+[]+!+[]] );
console.log((![]+[])[+[]])                  //f
console.log((![]+[])[+!+[]])                //a
console.log(([![]]+[][[]])[+!+[]+[+[]]])    //i
console.log((![]+[])[!+[]+!+[]])            //l

console.log([![]]+[][[]])
console.log([+[]])
console.log(+!+[])
console.log(+!+[]+[+[]])
console.log(+[0])


/* 
    - (![]+[])[+[]] -> ![] pusta tablica ewaluuje się do true, negacja daje false. false+[] konwertuje obie strony do stringa i daje w wyniku "false".
    +[] ewaluuje się do 0 (zamiana pustej tablicy na inta daje defaultowo 0), więc wynik to "false"[0] = "f"

    -(![]+[])[+!+[]] -> początek analogicznie, dalej [+!+[]]. Jak wcześniej +[] daje 0, natomiast !0 daje true, następnie konwersja +true daje 1, więc
    wynik to "false"[1] = "a"

    -([![]]+[][[]])[+!+[]+[+[]]] -> [![]] ewaluuje się do [false], a [][[]] do undefined, po konwersji przez + dostajemy "falseundefined".
    +!+[]+[+[]] ewaluuje się do 10, ponieważ: +!+[] jak wcześniej, daje 1, natomiast [+[]] do [0]. Po konwersji przez + dostajemy 10, zatem wynikiem
    jest "falseundefined"[10] = "i"

    -(![]+[])[!+[]+!+[]] -> analogicznie do "f" oraz "a", mamy sumę true + true co daje po konwersji 2, więc wynik to "false"[2] = "l"
*/