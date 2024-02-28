// Nie powinno się robić Worker.prototype = Person.prototype; bo to przypisuje referencje, w szczególności jeśli
// zmienimy prototyp Worker.prototype to nadpiszemy też Person.prototype
// Ten drugi nie śmiga Worker.prototype = new Person(); bo my chcemy prototyp Person a nie obiekt person
// (w ten sposób nie będziemy mieć dostępu do łańcucha prototypów)