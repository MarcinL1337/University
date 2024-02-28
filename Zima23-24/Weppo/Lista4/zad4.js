var n = 1;
// liczba ma prototyp?
console.log( typeof Object.getPrototypeOf( n ) );
// można 'jej dopisać pole/funkcję?
n.foo = 'foo';
console.log( n.foo );


// Pierwsze console.log(typeof Object.getPrototypeOf(n)) zwróci wynik 'object',
// co może być mylące. Jednak to nie oznacza, że liczba n ma prototyp jako obiekt,
// ponieważ wartość typeof Object.getPrototypeOf(n) zawsze zwraca 'object' dla wartości prymitywnych.
// Jest to wynik wewnętrznej konwersji typu, która działa w tle.
//
// Drugie console.log(n.foo) zwróci wynik undefined, ponieważ próba dodania pola foo do liczby n
// nie jest możliwa. Wartość n jest typem prostym i nie ma prototypu, który by mógł zawierać dodatkowe
// pola lub funkcje. Dlatego próba dostępu do pola foo na n nie powiedzie się i zwróci undefined.