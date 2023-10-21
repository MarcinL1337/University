let aux_obj = {
    klucz1: 1
}

let new_obj = {
    klucz1: "wartość1",
    klucz2: 1,
    klucz3: {klucz1_1: "wartość1_1", klucz2_1: 2},
    "klucz 4": "kropką nie można odczytać",
    2: 2,
    aux_obj: "obiekt"
}

console.log("A)");
console.log(new_obj['klucz1']);
console.log(new_obj.klucz1);
// console.log(new_obj."klucz 4"); // Nie można tak
console.log(new_obj["klucz 4"]);


// ==========================================================================

console.log("\n\nB)");
console.log(new_obj[2]); //konwertuje się do stringa/symbolu i znajduje odpowiedni klucz
console.log(new_obj[aux_obj]);

new_obj2 = {};
new_obj2[aux_obj] = 1;
new_obj2[2] = 2;

console.log(new_obj2);
console.log(new_obj2['[object Object]']);

// ==========================================================================

console.log("\n\nC)");

let new_arr = [1, 2, 3, 4, 5];

console.log(new_arr["1"]);
console.log(new_arr[new_obj2]);