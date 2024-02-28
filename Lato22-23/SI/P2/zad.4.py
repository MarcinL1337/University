from collections import deque

perms = ['RLDU', 'LRUD', 'LDUR', 'ULRD', 'RULD', 'DULR', 'DLRU', 'DRUL', 'LRDU', 'RDLU', 'DLUR', 'UDLR', 'DURL', 'LUDR',
         'DRLU', 'ULDR', 'URDL', 'URLD', 'UDRL', 'RLUD', 'LURD', 'LDRU', 'RUDL', 'RDUL']

# K - współrzędne komandosa, board - aktualna plansza
def pre_process(K, board):
    global perms
    pre_moves = []
    mini = 100
    best_seq = ""
    best_K = K
    iters = 19
    for x in perms:
        pre_moves.append(x[0] * iters + x[1] * iters + x[2] * iters + x[3] * iters)
    for move_seq in pre_moves:
        temp_K = K
        for c in move_seq:
            temp_K = move(c, temp_K, board)
        if len(temp_K) < mini:
            mini = len(temp_K)
            best_seq = move_seq
            best_K = temp_K
    return best_K, best_seq


def Zad4():
    with open('zad_input.txt', "r") as f:
        lab = f.read().split("\n")
        initial_board = [[*x] for x in lab[:-1]]
    temp = []
    for i in range(len(initial_board)):
        for j in range(len(initial_board[0])):
            if initial_board[i][j] == "S" or initial_board[i][j] == "B":
                temp.append((i, j))
    temp.sort()
    K = tuple(temp)
    K, initial_seq = pre_process(K, initial_board)

    case3 = "L" * 9 + "D" * 4
    case6 = "L" * 4 + "R"
    case8 = "L" * 9 + "D" * 4
    case9 = "R" * 3 + "D" * 5
    specific_cases = ""
    specific_cases = specific_cases + case3 + case8 + case9 + case6
    for c in specific_cases:
        K = move(c, K, initial_board)
    initial_seq += specific_cases
    moves, final_board = BFS(K, initial_board)
    initial_seq += moves

    with open('zad_output.txt', "w") as f:
        f.write(initial_seq)


def move(direction, komandos, board):
    length = len(komandos)
    board_size_x = len(board)
    board_size_y = len(board[0])
    res = set()
    for i in range(length):
        flag = True
        x, y = komandos[i]
        match direction:
            case "L":
                if y > 0 and board[x][y - 1] != "#":
                    res.add((x, y - 1))
                    flag = False
            case "R":
                if y < board_size_y - 1 and board[x][y + 1] != "#":
                    res.add((x, y + 1))
                    flag = False
            case "U":
                if x > 0 and board[x - 1][y] != "#":
                    res.add((x - 1, y))
                    flag = False
            case "D":
                if x < board_size_x - 1 and board[x + 1][y] != "#":
                    res.add((x + 1, y))
                    flag = False
        if flag:
            res.add((x, y))
    list(res).sort()
    return tuple(res)


def check(board, initial_board):
    isOver = True
    for x, y in board:
        if initial_board[x][y] != "G" and initial_board[x][y] != "B":
            isOver = False
            break
    return isOver


def BFS(init_komando, board):
    q = deque()
    q.append((init_komando, ""))
    all_combinations = set()
    while True:
        komandos, moves = q.popleft()

        if check(komandos, board):
            return moves, komandos

        hash_val = hash(komandos)

        if hash_val in all_combinations:
            continue
        else:
            all_combinations.add(hash_val)

        poss_moves = "URLD"
        for i in range(4):
            new_komandos = move(poss_moves[i], komandos, board)
            q.append((new_komandos, moves + poss_moves[i]))


Zad4()