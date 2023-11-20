type User = {
  type: 'user'; // zmiana
  name: string;
  age: number;
  occupation: string;
};
type Admin = {
  type: 'admin'; // zmiana
  name: string;
  age: number;
  role: string;
};
export type Person = User | Admin;

export const persons: Person[] = [
  {
    type: 'user',
    name: "Jan Kowalski",
    age: 17,
    occupation: "Student",
  },
  {
    type: 'admin',
    name: "Tomasz Malinowski",
    age: 20,
    role: "Administrator",
  },
];

export function isAdmin(person: Person): person is Admin { // zmiana
  return person.type === "admin";
}
export function isUser(person: Person): person is User { // zmiana
  return person.type === "user";
}
export function logPerson(person: Person) {
  let additionalInformation: string = "";
  if (isAdmin(person)) {
    additionalInformation = person.role;
  }
  if (isUser(person)) {
    additionalInformation = person.occupation;
  }
  console.log(` - ${person.name}, ${person.age}, ${additionalInformation}`);
}


logPerson(persons[0]);
logPerson(persons[1]);