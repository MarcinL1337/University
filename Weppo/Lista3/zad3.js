// var jest scope'owany przez funkcje, a let przez każde "nawiasy" ({}),
// wiec zmiana var na let powoduje, że każda pętla for tworzy dla siebie
// nową zmienna, która ma scope w tej pętli

function createFs(n) { // tworzy tablicę n funkcji
    var fs = []; // i-ta funkcja z tablicy ma zwrócić i
    for (var i = 0; i < n; i++) {
        fs[i] =
            (function (j) {
                //var j = i; // bez tego dalej będzie stare i
                return function() {return j;};
            })(i);
    };
    return fs;
}
var myfs = createFs(10);
console.log(myfs[0]()); // zerowa funkcja miała zwrócić 0
console.log(myfs[2]()); // druga miała zwrócić 2
console.log(myfs[7]());
// 10 10 10 // ale wszystkie zwracają 10!?