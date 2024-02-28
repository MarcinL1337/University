import sys
import math
import queue

moving_player, white_king, white_rook, black_king = input().split()


def convert(coordinates):
    return (ord(coordinates[0]) - 96, int(coordinates[1]))


def convertBack(move):
    return chr(move[0] + 96) + str(move[1])

figures = [convert(white_king), convert(white_rook), convert(black_king), "", moving_player == "white"]

def isKingMoveValid(coords_w, coords_b):
    if abs(coords_w[0] - coords_b[0]) <= 1 and abs(coords_w[1] - coords_b[1]) <= 1: return False
    return True 


def isWithinBoard(coords):
    x = coords[0]
    y = coords[1]
    if x <= 0 or y <= 0 or x >= 9 or y >= 9: return False
    return True


def isBlackKingInSameCR(coords_rk, coords_bk):
    if coords_rk[0] == coords_bk[0] or coords_rk[1] == coords_bk[1]: return True
    return False


def isBlackMated(board):
    if isBlackKingInSameCR(board[1], board[2]) and not len(blackKingMoves(board)): return True
    return False


def isRookSafe(board, coords_rk):
    isNextToBKing = abs(coords_rk[0] - board[2][0]) <= 1 and (abs(coords_rk[1] - board[2][1]) <= 1)
    isNextToWKing = abs(coords_rk[0] - board[0][0]) <= 1 and (abs(coords_rk[1] - board[0][1]) <= 1)
    return not isNextToBKing or isNextToWKing
    

def rookMoves(board):
    x = board[1][0]
    y = board[1][1]
    moves = []
    for i in range(x+1, 9):
        if (i, y) == board[0]: break
        if (i, y) == board[2]:
            moves.append((i, y, 2))
            break
        moves.append((i, y, 1))
    for i in range(1, x):
        if (i, y) == board[0]: break
        if (i, y) == board[2]:
            moves.append((i, y, 2))
            break
        if (i, y) == (x, y): continue
        moves.append((i, y, 1))
    for i in range(y+1, 9):
        if (x, i) == board[0]: break
        if (x, i) == board[2]:
            moves.append((x, i, 2))
            break
        moves.append((x, i, 1))
    for i in range(1, y):
        if (x, i) == board[0]: break
        if (x, i) == board[2]:
            moves.append((x, i, 2))
            break
        if (x, i) == (x, y): continue
        moves.append((x, i, 1))
    moves2 = [coords for coords in moves if isRookSafe(board, coords)]
    return moves2


def whiteKingMoves(board):
    x = board[0][0]
    y = board[0][1]
    moves = [(x + i, y + j) for i in range(-1, 2) for j in range(-1, 2)]
    if (x, y) in moves: moves.remove((x, y))
    moves2 = []
    for i in moves:
        if isWithinBoard(i) and isKingMoveValid(i, board[2]) and not i in board: moves2.append(i)
    return moves2


def blackKingMoves(board):
    x = board[2][0]
    y = board[2][1]
    moves = [(x + i, y + j) for i in range(-1, 2) for j in range(-1, 2)]
    if (x, y) in moves: moves.remove((x, y))
    moves2 = []
    for i in moves:
        if isWithinBoard(i) and isKingMoveValid(board[0], i) and not isBlackKingInSameCR(board[1], i) and not i in board: moves2.append(i)
    return moves2


def BFS():
    searched = {}
    q = queue.Queue()
    q.put(figures)
    while True:
        board_state = q.get()
        pos = f"{board_state[0]}{board_state[1]}{board_state[2]}"
        if pos in searched:
            continue
        searched[pos] = 1
        if board_state[4]:
            rook_moves = rookMoves(board_state)
            for i in rook_moves:
                new_board_state = [board_state[0], i, board_state[2], board_state[3] + convertBack(board_state[1]) + convertBack(i) + " ", not board_state[4]]
                q.put(new_board_state)
            king_moves = whiteKingMoves(board_state)
            for i in king_moves:
                new_board_state = [i, board_state[1], board_state[2], board_state[3] + convertBack(board_state[0]) + convertBack(i) + " ", not board_state[4]]
                q.put(new_board_state)
        else:
            king_moves = blackKingMoves(board_state)
            if not len(king_moves) and isBlackKingInSameCR(board_state[1], board_state[2]):
                print(board_state[3][:-1])
                return
            for i in king_moves:
                new_board_state = [board_state[0], board_state[1], i, board_state[3] + convertBack(board_state[2]) + convertBack(i) + " ", not board_state[4]]
                q.put(new_board_state)
                if isBlackMated(new_board_state):
                    print(new_board_state[3][:-1])
                    return

BFS()