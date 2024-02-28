/*
    typeof najlepiej używać dla prostych typów wbudowanych. W typach zdefiniowanych przez użytkownika nie daje on dużo informacji,
    w przeciwieństwie do insance of.
*/

var type = function() {};
var instance = new type();

console.log(typeof instance);
console.log(typeof instance == 'type');

console.log(instance instanceof Object);
console.log(instance instanceof type);


console.log('string' instanceof String);
console.log(typeof 'string' == 'string');