namespace Zad6
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Vector2 vector1 = new Vector2(3, 4);
            Vector2 vector2 = new Vector2(1, 2);

            Vector2 vector3 = vector1 + vector2;
            Console.WriteLine(vector3.ToString());

            Console.WriteLine($"Dot product: {vector1 * vector2}");

            Console.ReadLine();
        }
    }

    class Vector2
    {
        public double X { get; set; }
        public double Y { get; set; }

        public Vector2(double x, double y)
        {
            X = x;
            Y = y;
        }

        public static Vector2 operator +(Vector2 vector1, Vector2 vector2)
        {
            return new Vector2(vector1.X + vector2.X, vector1.Y + vector2.Y);
        }

        public static double operator *(Vector2 vector1, Vector2 vector2)
        {
            return vector1.X * vector2.X + vector1.Y * vector2.Y;
        }

        public override string ToString()
        {
            return $"({X}, {Y})";
        }
    }
}
