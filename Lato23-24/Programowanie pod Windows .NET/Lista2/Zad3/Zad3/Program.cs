using System;

public class Person1
{
    private string _name; // backing field

    public string Name
    {
        get { return _name; }
        set { _name = value; }
    }
}

public class Person2
{
    // Auto-implemented property
    public string Name { get; set; }
}