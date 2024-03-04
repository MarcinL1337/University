public class AccessModifiersExample
{
    // public: Dostęp z dowolnego miejsca
    public string PublicField = "To jest pole publiczne.";

    // protected: Dostęp z wnętrza klasy oraz klas pochodnych
    protected int ProtectedMethod()
    {
        Console.WriteLine("To jest metoda z kwalifikatorem protected.");
        return 42;
    }

    // internal: Dostęp tylko w obrębie zestawu (assembly)
    internal void InternalMethod()
    {
        Console.WriteLine("To jest metoda z kwalifikatorem internal.");
    }

    // private: Dostęp tylko w obrębie tej klasy
    private double PrivateProperty { get; set; } = 3.14;

    public void DisplayValues()
    {
        Console.WriteLine($"PublicField: {PublicField}");
        
        // Wywołanie metody z kwalifikatorem protected w obrębie klasy
        int result = ProtectedMethod();
        Console.WriteLine($"Wynik metody ProtectedMethod: {result}");

        InternalMethod(); // Wywołanie metody z kwalifikatorem internal

        Console.WriteLine($"PrivateProperty: {PrivateProperty}");
    }
}

class Program
{
    static void Main()
    {
        AccessModifiersExample example = new AccessModifiersExample();
        
        // Dostęp do pola publicznego z dowolnego miejsca
        Console.WriteLine($"Wartość pola PublicField: {example.PublicField}");
        
        // Nie można bezpośrednio uzyskać dostępu do metody z kwalifikatorem protected spoza klasy
        
        // example.ProtectedMethod(); // To spowoduje błąd kompilacji
        example.InternalMethod(); 

        example.DisplayValues(); // Wywołanie metody publicznej, która używa wszystkich kwalifikatorów dostępu
    }
}
