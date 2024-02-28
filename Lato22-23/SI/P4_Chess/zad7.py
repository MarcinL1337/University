import sys

import chess
import chess.polyglot

PLAYER = chess.BLACK


def evaluate(board):
    piece_values = {
        chess.PAWN: 1,
        chess.KNIGHT: 3,
        chess.BISHOP: 3,
        chess.ROOK: 5,
        chess.QUEEN: 9,
        chess.KING: 0
    }

    material_score = 0
    mobility_score = 0
    pawn_structure_score = 0
    king_safety_score = 0
    position_score = 0

    for square in chess.SQUARES:
        piece = board.piece_at(square)
        if piece is not None:
            if piece.color == PLAYER:
                material_score += piece_values[piece.piece_type]
                mobility_score += len(board.attacks(square))
            else:
                material_score -= piece_values[piece.piece_type]
                mobility_score -= len(board.attacks(square))

    # King Safety Estimation
    king_square = board.king(chess.WHITE)
    if king_square is not None:
        if chess.WHITE == PLAYER:
            king_safety_score -= len(board.attackers(chess.WHITE, king_square))
        else:
            king_safety_score += len(board.attackers(chess.WHITE, king_square))

    king_square = board.king(chess.BLACK)
    if king_square is not None:
        if chess.BLACK == PLAYER:
            king_safety_score -= len(board.attackers(chess.BLACK, king_square))
        else:
            king_safety_score += len(board.attackers(chess.BLACK, king_square))

    total_score = material_score + mobility_score + pawn_structure_score + king_safety_score
    return total_score


opening_book = chess.polyglot.open_reader("codekiddy.bin")


def minimax(board, depth, alpha, beta, maximizing_player):
    if depth == 0 or board.is_game_over():
        return evaluate(board)

    if maximizing_player:
        max_eval = float('-inf')
        for move in board.legal_moves:
            board.push(move)
            eval = minimax(board, depth - 1, alpha, beta, False)
            board.pop()
            max_eval = max(max_eval, eval)
            alpha = max(alpha, eval)
            if beta <= alpha:
                break
        return max_eval
    else:
        min_eval = float('inf')
        for move in board.legal_moves:
            board.push(move)
            eval = minimax(board, depth - 1, alpha, beta, True)
            board.pop()
            min_eval = min(min_eval, eval)
            beta = min(beta, eval)
            if beta <= alpha:
                break
        return min_eval


def hear():
    line = sys.stdin.readline().split()
    return line[0], line[1:]


def say(what):
    sys.stdout.write(what)
    sys.stdout.write('\n')
    sys.stdout.flush()


def play_chess():
    board = chess.Board()
    best_move = None
    global PLAYER
    PLAYER = chess.BLACK

    while True:
        cmd, args = hear()
        if cmd == 'UGO':
            PLAYER = chess.WHITE
            try:
                best_move = opening_book.weighted_choice(board).move  # Wybór ruchu na podstawie księgi otwarć
            except:
                max_eval = float('-inf')
                for move in board.legal_moves:
                    board.push(move)
                    eval = minimax(board, 2, float('-inf'), float('inf'), False)
                    board.pop()
                    if eval > max_eval:
                        max_eval = eval
                        best_move = move
            board.push(best_move)
            say("IDO " + best_move.__str__())
        elif cmd == 'HEDID':
            unused_move_timeout, unused_game_timeout = args[:2]
            move = args[2]
            # print(move)
            board.push(chess.Move.from_uci(move))

            try:
                best_move = opening_book.weighted_choice(board).move  # Wybór ruchu na podstawie księgi otwarć
            except:
                max_eval = float('-inf')
                for move in board.legal_moves:
                    board.push(move)
                    eval = minimax(board, 2, float('-inf'), float('inf'), False)
                    board.pop()
                    if eval > max_eval:
                        max_eval = eval
                        best_move = move
            board.push(best_move)
            say("IDO " + best_move.__str__())
        elif cmd == 'ONEMORE':
            sys.stdout.write("RDY")
            sys.stdout.write('\n')
            sys.stdout.flush()
            play_chess()
            continue
        elif cmd == 'BYE':
            break


if __name__ == '__main__':
    sys.stdout.write("RDY")
    sys.stdout.write('\n')
    sys.stdout.flush()
    play_chess()
