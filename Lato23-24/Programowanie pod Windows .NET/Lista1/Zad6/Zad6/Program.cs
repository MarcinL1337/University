public class OverloadingExample
{
    // Przeciążanie sygnatur funkcji z różnymi typami zwracanymi
    public int Add(int a, int b)
    {
        return a + b;
    }

    public double Add(double a, double b)
    {
        return a + b;
    }

    // Funkcja przeciążona wywołująca inną funkcję przeciążoną
    public void DisplayResult(int a, int b)
    {
        int result = Add(a, b);
        Console.WriteLine($"Wynik dodawania (int): {result}");
    }

    public void DisplayResult(double a, double b)
    {
        double result = Add(a, b); // Wywołanie funkcji przeciążonej z inną sygnaturą
        Console.WriteLine($"Wynik dodawania (double): {result}");
    }

    // Przeciążanie konstruktorów klasy
    public OverloadingExample()
    {
        Console.WriteLine("Konstruktor domyślny");
    }

    public OverloadingExample(string message)
    {
        Console.WriteLine($"Konstruktor z komunikatem: {message}");
    }

    // Funkcja przeciążona wywołująca funkcję z klasy bazowej
    public void CallBaseMethod()
    {
        Console.WriteLine("Funkcja z klasy bazowej");
    }
}

public class DerivedExample : OverloadingExample
{
    // Przeciążanie konstruktorów klasy pochodnej
    public DerivedExample() : base()
    {
        Console.WriteLine("Konstruktor klasy pochodnej");
    }

    public DerivedExample(string message) : base(message)
    {
        Console.WriteLine($"Konstruktor klasy pochodnej z komunikatem: {message}");
    }

    // Przeciążanie funkcji z klasy pochodnej
    public new void CallBaseMethod()
    {
        base.CallBaseMethod(); // Wywołanie funkcji z klasy bazowej
        Console.WriteLine("Funkcja z klasy pochodnej");
    }
}

class Program
{
    static void Main()
    {
        OverloadingExample example = new OverloadingExample();

        // Wywołanie funkcji przeciążonej z różnymi sygnaturami
        example.DisplayResult(5, 10);
        example.DisplayResult(5.5, 10.5);

        Console.WriteLine("\nPrzeciążanie konstruktorów:");

        // Wywołanie konstruktorów klasy bazowej
        OverloadingExample example1 = new OverloadingExample();
        OverloadingExample example2 = new OverloadingExample("Hello, Overloading!");

        Console.WriteLine("\nPrzeciążanie funkcji w klasie pochodnej:");

        // Wywołanie funkcji przeciążonej z klasy pochodnej
        DerivedExample derivedExample = new DerivedExample();
        derivedExample.CallBaseMethod();
        DerivedExample derivedExampleWithMessage = new DerivedExample("Hello, Derived!");
        derivedExampleWithMessage.CallBaseMethod();
    }
}
