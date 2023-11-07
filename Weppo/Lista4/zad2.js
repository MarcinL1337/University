function hasProperty(obj, field) {
  return obj.hasOwnProperty(field);
}

let p = {
  name2: "jan",
};
let q = {
  surname: "kowalski",
  name: "marcin",
};

Object.setPrototypeOf(p, q);

function enumerateOwnProperties(obj) {
  for (let key in p) {
    if (hasProperty(p, key)) {
      console.log(key);
    }
  }
}

function enumerateAllProperties(obj) {
  for (let key in p) {
    console.log(key);
  }
}

console.log(hasProperty(p, "name"));

// console.log(p.name);
// console.log(p.surname);
// console.log(p.name2);

enumerateOwnProperties(p);
enumerateAllProperties(p);
