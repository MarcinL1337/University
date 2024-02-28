function getLastProto(o) {
  var p = o;
  do {
    o = p;
    p = Object.getPrototypeOf(o);
  } while (p);
  return o;
}

let o1 = {name : "aaa"};
let o2 = {name : "bbb"};
console.log(getLastProto(o1) == getLastProto(o2));

let o3 = [];
let o4 = [];
console.log(getLastProto(o3) == getLastProto(o4));