from tabulate import tabulate

sudoku = [
    [7,8,0,4,0,0,1,2,0],
    [6,0,0,0,7,5,0,0,9],
    [0,0,0,6,0,1,0,7,8],
    [0,0,7,0,4,0,2,6,0],
    [0,0,1,0,5,0,9,3,0],
    [9,0,4,0,6,0,0,0,5],
    [0,7,0,3,0,0,0,1,2],
    [1,2,0,0,0,7,4,0,0],
    [0,4,9,2,0,6,0,0,7]
]

puste_sudoku = []
for i in range(9):
    puste_sudoku.append([0, 0, 0, 0, 0, 0, 0, 0, 0])


def row_check(sudoku, x, val):
    for i in range(len(sudoku)):
        if sudoku[x][i] == val: return False
    return True


def col_check(sudoku, y, val):
    for i in range(len(sudoku)):
        if sudoku[i][y] == val: return False
    return True


def square_check(sudoku, x, y, val):
    x -= x % 3
    y -= y % 3
    for i in range(x, x+3):
        for j in range(y, y+3):
            if sudoku[i][j] == val: return False
    return True


def insert_val_check(sudoku, x, y, val):
    return row_check(sudoku, x, val) and col_check(sudoku, y, val) and square_check(sudoku, x, y, val)


def insert(sudoku, x, y, val):
    sudoku[x][y] = val
    return sudoku


def coords(sudoku):
    tab = []
    for i in range(len(sudoku)):
        for j in range(len(sudoku)):
            if sudoku[i][j] == 0:
                tab.append((i, j))
    return tab

""" Wersja v1, bez yielda. Działająca rozwiązywarka sudoku.
def solve(sudoku):
    aux = coords(sudoku)
    def solve_aux(sudoku, i):
        if i < len(aux) and i >= 0:
            x = aux[i][0]
            y = aux[i][1]
            for j in range(sudoku[x][y]+1, 10):
                if insert_val_check(sudoku, x, y, j):
                    return solve_aux(insert(sudoku, x, y, j), i+1)
            return solve_aux(insert(sudoku, x, y, 0), i-1)
        
        if i > 0: return sudoku
        else: return None

    return solve_aux(sudoku, 0)

print(tabulate(solve(sudoku),tablefmt="rounded_grid"))
"""

# Wersja z yield.
def solve2(sudoku):
    aux = coords(sudoku)
    def solve_aux(sudoku, i):
        if i < len(aux) and i >= 0:
            x = aux[i][0]
            y = aux[i][1]
            for j in range(sudoku[x][y]+1, 10):
                if insert_val_check(sudoku, x, y, j):
                    return solve_aux(insert(sudoku, x, y, j), i+1)
            return solve_aux(insert(sudoku, x, y, 0), i-1)
        
        if i > 0: return sudoku
        else: return None

    k = solve_aux(sudoku, 0)
    while True:
        if k == None:
            break
        yield tabulate(k, tablefmt="rounded_grid")
        k = solve_aux(sudoku, len(aux)-1)


counter = 0
for i in solve2(puste_sudoku):
    if counter > 10: break
    print(i)
    counter += 1