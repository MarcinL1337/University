type User = {
    name: string;
    age: number;
    occupation: string;
  };
  type Admin = {
    name: string;
    age: number;
    role: string;
  };
  export type Person = User | Admin;
  
  export const persons: Person[] = [
    {
      name: "Jan Kowalski",
      age: 17,
      occupation: "Student",
    },
    {
      name: "Tomasz Malinowski",
      age: 20,
      role: "Administrator",
    },
  ];

import log from "./zad5";

import { fib_memo } from "./zad2";


let arr = [1, 2, 3, 4];

log(persons[0]);
log(persons[1]);

const fib = fib_memo();
console.log(fib(15));