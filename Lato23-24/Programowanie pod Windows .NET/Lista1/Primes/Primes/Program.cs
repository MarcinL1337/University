// See https://aka.ms/new-console-template for more information

/// <summary>
/// Metoda sprawdzająca, czy warunek z zadania zachodzi
/// </summary>
/// /// <param name="n">Argumenty będący liczbą</param>
bool checkPerfect(int n)
{
    if (n <= 0) return false;
    int new_n = n;
    int digit_sum = 0;
    while(new_n > 0)
    {
        int remainder = new_n % 10;
        if (remainder == 0) return false;
        if(n % remainder == 0)
        {
            digit_sum+=remainder;
            new_n /= 10;
            continue;
        }
        return false;
    }
    if(n % digit_sum == 0)
        return true;
    return false;
}


for(int i = 0; i < 100000; i++)
{
    if (checkPerfect(i) == true)
        Console.WriteLine(i);
}
