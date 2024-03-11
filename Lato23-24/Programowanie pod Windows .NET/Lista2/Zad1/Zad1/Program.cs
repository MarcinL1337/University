class Zad1
{

    // modyfikator static dla klas
    /*
        A static class is basically the same as a non-static class,
        but there is one difference: a static class cannot be instantiated.
        In other words, you cannot use the new operator to create a variable of the class type.
    */

    // modyfikator static dla składowych klas (pól, metod)
    /* 
        The static member is callable on a class even when no instance of the class has been created.
        The static member is always accessed by the class name, not the instance name.
        Only one copy of a static member exists, regardless of how many instances of the class are created.
    */
    static class Statics
    {
        public static int n;
    }

    // modyfikator sealed dla klas
    // When applied to a class, the sealed modifier prevents other classes from inheriting from it.
    sealed class SealedClass
    {
        public static int n;
    }

    // modyfikator abstract dla klas
    /* 
        The abstract modifier indicates that the thing being modified has a missing or incomplete implementation.
        Use the abstract modifier in a class declaration to indicate that a class is intended
        only to be a base class of other classes, not instantiated on its own.
    */

    // modyfikator abstract dla składowych klas(metod)
    // do iterfejsów - nie ma ciała TRZEBA zaimplementować, z użyciem override

    abstract class AbstractS
    {
        abstract public int somefunc();
    }

    // słowa kluczowe virtual i override dla składowych klas (metod)
    /*
        The virtual keyword is used to modify a method, property, indexer, or event 
        declaration and allow for it to be overridden in a derived class.    
    */
    class Virtuals
    {
        virtual public int foo(int x)
        {
            return x + 1;
        }
    }
    class InheritedV : Virtuals
    {
        override public int foo(int x)
        {
            return x + 2;
        }
    }

    // słowo kluczowe partial w definicji klasy
    // Pozwala rozbijać implementacje klas
    partial class PartialClass
    {
        public void foo1() { }
    }
    partial class PartialClass
    {
        public void foo2() { }
    }

    // słowo kluczowe readonly w deklaracji pola klasy
    // ze zmiennej można tylko czytać - niemodyfikowalna kopia
    class ReadME
    {
        public readonly int n;
        ReadME(int n)
        {
            this.n = n;
        }
        void foo(int x) // jak referencja w C++
        {
            //n = x; // error
        }
    }

    // modyfikatory in, ref oraz out na liście parametrów metody
    /*
    IN - readonly ref
    REF - referencja    
    OUT - Variables passed as out arguments don't have to be initialized before being passed in a method call.
          However, the called method is required to assign a value before the method returns.
        */
    public void ProcessInput(in int value, ref int yes, out int nope)
    {
        // value += 1; // error
        yes = value;
        nope = yes;     // bez tego błąd
    }

    // modyfikator params na liście parametrów metody
    // params zaznacza że chcemy przekazać tablicę parametrów ze zmienną liczbą arguemntów
    public int Sum(params int[] numbers)
    {
        int sum = 0;
        foreach (int num in numbers)
            sum += num;
        return sum;
    }
}