namespace Zad5
{
    class Program
    {
        static void Main(string[] args)
        {
            Grid grid = new Grid(4, 4);

            grid[1, 3] = 42;

            int val = grid[1, 3]; // 42
            Console.WriteLine(val);

            int[] rowData = grid[1]; // {0, 0, 0, 42}
            for (int i = 0; i < rowData.Length; i++)
            {
                Console.Write(rowData[i]);
                Console.Write(" ");
            }
        }
    }


    public class Grid
    {
        public int[,] _grid;

        public Grid(int rows, int cols)
        {
            if (rows <= 0 || cols <= 0)
            {
                throw new ArgumentException("rows and cols must be >= 0");
            }

            _grid = new int[rows, cols];
        }

        public int this[int row, int col]
        {
            get
            {
                ValidateIndices(row, col);
                return _grid[row, col];
            }
            set
            {
                ValidateIndices(row, col);
                _grid[row, col] = value;
            }
        }

        public int[] this[int row]
        {
            get
            {
                if (row < 0 || row >= _grid.GetLength(0))
                {
                    throw new IndexOutOfRangeException("out of range");
                }

                int[] rowData = new int[_grid.GetLength(1)];
                for (int i = 0; i < _grid.GetLength(1); i++)
                {
                    rowData[i] = _grid[row, i];
                }
                return rowData;
            }
        }

        private void ValidateIndices(int row, int col)
        {
            if (row < 0 || row >= _grid.GetLength(0) || col < 0 || col >= _grid.GetLength(1))
            {
                throw new IndexOutOfRangeException("out of range");
            }
        }
    }

}