import queue

# State = [cur_pos, cur_box_pos, cur_moves]

board = []

with open("/home/marcin/UWr/Lato22-23/SI/P2/zad_input.txt", "r") as inp:
    inp = inp.readlines()
    row_len = len(inp[0]) - 1
    col_len = len(inp)
    for line in inp:
        board.append(line.strip())

output = open("/home/marcin/UWr/Lato22-23/SI/P2/zad_output.txt", "w")

boxes_pos = []
boxes_goals = []
start = []

for i in range(col_len):
    for j in range(row_len):
        if board[i][j] == 'G':
            boxes_goals.append((i, j))
        if board[i][j] == 'K':
            start = (i, j)
        if board[i][j] == 'B':
            boxes_pos.append((i, j))
        if board[i][j] == '*':
            boxes_pos.append((i, j))
            boxes_goals.append((i, j))
        if board[i][j] == '+':
            start = (i, j)
            boxes_goals.append((i, j))


def checkIfCanWin(box_pos):
    x = box_pos[0]
    y = box_pos[1]
    if board[x][y] != "G":
        if board[x-1][y] == "W" and board[x][y-1] == "W": return False
        if board[x-1][y] == "W" and board[x][y+1] == "W": return False
        if board[x+1][y] == "W" and board[x][y-1] == "W": return False
        if board[x+1][y] == "W" and board[x][y+1] == "W": return False
    return True

    # czemu to nie działa? xd
    # if board[x][y] != "G":
    #     if board[x-1][y] == "W" or board[x+1][y] == "W":
    #         adjacent = board[x][y-1] + board[x][y+1]
    #         if adjacent not in ["..", "G.", ".G", "GG"]: return False
    #     if board[x][y-1] == "W" or board[x][y+1] == "W":
    #         adjacent = board[x-1][y] + board[x+1][y]
    #         # print(adjacent)
    #         if adjacent not in ["..", "G.", ".G", "GG"]: return False
    # return True

    

def newState(cur_pos, cur_box_pos, moves, possible_move):
    next_move = (cur_pos[0] + possible_move[1][0], cur_pos[1] + possible_move[1][1])

    if board[next_move[0]][next_move[1]] == "W": return None

    if next_move in cur_box_pos:
        next_next_move = (next_move[0] + possible_move[1][0], next_move[1] + possible_move[1][1])
        if board[next_next_move[0]][next_next_move[1]] != "W" and next_next_move not in cur_box_pos:
            if not checkIfCanWin(next_next_move): return None
            new_box_pos = []
            for pos in cur_box_pos:
                if pos == next_move:
                    new_box_pos.append(next_next_move)
                else:
                    new_box_pos.append(pos)
            return [next_move, new_box_pos, moves + possible_move[0]]
        else:
            return None
    return [next_move, cur_box_pos, moves + possible_move[0]]
    

def finished(box_positions):
    for pos in box_positions:
        if pos not in boxes_goals:
            return False
    return True


def BFS():
    visited = set()
    q = queue.Queue()
    q.put((start, boxes_pos, ""))
    while not q.empty():
        cur_state = q.get()
        cur_pos = cur_state[0]
        cur_box_pos = cur_state[1]
        cur_moves = cur_state[2]

        if (cur_pos, tuple(sorted(cur_box_pos))) in visited:
            continue
        visited.add((cur_pos, tuple(sorted(cur_box_pos))))

        if finished(cur_box_pos):
            output.write(cur_moves)
            return
        
        for possible_move in [["U", (-1, 0)], ["D", (1, 0)], ["L", (0, -1)], ["R", (0, 1)]]:
            next_state = newState(cur_pos, cur_box_pos, cur_moves, possible_move)
            if next_state and (next_state[0], tuple(sorted(next_state[1]))) not in visited:
                q.put([next_state[0], next_state[1], next_state[2]])


BFS()
