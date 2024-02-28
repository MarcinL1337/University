from random import choice
from copy import deepcopy
class Jungle:
    PIECE_VALUES = {
        0: 4,
        1: 1,
        2: 2,
        3: 3,
        4: 5,
        5: 7,
        6: 8,
        7: 10
    }
    MAXIMAL_PASSIVE = 30
    DENS_DIST = 0.1
    MX = 7
    MY = 9
    traps = {(2, 0), (4, 0), (3, 1), (2, 8), (4, 8), (3, 7)}
    ponds = {(x, y) for x in [1, 2, 4, 5] for y in [3, 4, 5]}
    dens = [(3, 8), (3, 0)]
    dirs = [(0, 1), (1, 0), (-1, 0), (0, -1)]

    rat, cat, dog, wolf, jaguar, tiger, lion, elephant = range(8)

    def __init__(self, starting_player=0):
        self.board = self.initial_board()
        self.pieces = {0: {}, 1: {}}

        for y in range(Jungle.MY):
            for x in range(Jungle.MX):
                C = self.board[y][x]
                if C:
                    pl, pc = C
                    self.pieces[pl][pc] = (x, y)
        self.curplayer = starting_player
        self.peace_counter = 0
        self.winner = None

    def initial_board(self):
        pieces = """
        L.....T
        .D...C.
        R.J.W.E
        .......
        .......
        .......
        e.w.j.r
        .c...d.
        t.....l
        """

        B = [x.strip() for x in pieces.split() if len(x) > 0]
        T = dict(zip('rcdwjtle', range(8)))

        res = []
        for y in range(9):
            raw = 7 * [None]
            for x in range(7):
                c = B[y][x]
                if c != '.':
                    if 'A' <= c <= 'Z':
                        player = 1
                    else:
                        player = 0
                    raw[x] = (player, T[c.lower()])
            res.append(raw)
        return res

    def random_move(self, player):
        ms = self.moves(player)
        if ms:
            return choice(ms)
        return None

    def evaluate_board(self, player):
        player1_score = 0
        player2_score = 0

        for dict_player, dictionary in self.pieces.items():
            for piece, coords in dictionary.items():
                if player == dict_player:
                    player1_score += self.PIECE_VALUES[piece]
                    player1_score += (10 - (abs(coords[0] - 3) + abs(coords[1] - 0)))**2
                else:
                    player2_score += self.PIECE_VALUES[piece]

        return player1_score - player2_score

    def minimax(self, depth, alpha, beta, maximizing_player):
        if depth == 0 or self.isGameOver():
            return self.evaluate_board(self.curplayer)

        if maximizing_player:
            max_eval = float('-inf')
            for move in self.moves(self.curplayer):
                board_copy = deepcopy(self.board)
                pieces_copy = deepcopy(self.pieces)
                player_copy = self.curplayer
                self.do_move(move)
                eval = self.minimax(depth - 1, alpha, beta, False)
                self.board = board_copy
                self.pieces = pieces_copy
                self.curplayer = player_copy

                max_eval = max(max_eval, eval)
                alpha = max(alpha, eval)
                if beta <= alpha:
                    break
            return max_eval
        else:
            min_eval = float('inf')
            for move in self.moves(self.curplayer):
                board_copy = deepcopy(self.board)
                pieces_copy = deepcopy(self.pieces)
                player_copy = self.curplayer
                self.do_move(move)
                eval = self.minimax(depth - 1, alpha, beta, True)
                self.board = board_copy
                self.pieces = pieces_copy
                self.curplayer = player_copy

                min_eval = min(min_eval, eval)
                beta = min(beta, eval)
                if beta <= alpha:
                    break
            return min_eval

    def min_max_move(self, player):
        poss_moves = self.moves(player)
        # print(poss_moves)
        res = []
        if not poss_moves:
            return None
        for m in poss_moves:
            board_copy = deepcopy(self.board)
            pieces_copy = deepcopy(self.pieces)
            player_copy = self.curplayer
            self.do_move(m)
            value = self.minimax(3, float('-inf'), float('inf'), False)
            res.append((m, value))
            self.board = board_copy
            self.pieces = pieces_copy
            self.curplayer = player_copy
        best_val = max(res, key=lambda move: move[1])[1]
        good_moves = [n for n in res if n[1] == best_val]
        best_move = choice(good_moves)[0]
        # print(best_move)
        return best_move

    def heuristic_move(self, player):
        poss_moves = self.moves(player)
        # print(poss_moves)
        res = []
        if not poss_moves:
            return None
        for m in poss_moves:
            board_copy = deepcopy(self.board)
            pieces_copy = deepcopy(self.pieces)
            player_copy = self.curplayer
            self.do_move(m)
            value = self.evaluate_board(player)
            res.append((m, value))
            self.board = board_copy
            self.pieces = pieces_copy
            self.curplayer = player_copy
        best_val = max(res, key=lambda move: move[1])[1]
        good_moves = [n for n in res if n[1] == best_val]
        best_move = choice(good_moves)[0]
        # print(best_move)
        return best_move

    def can_beat(self, p1, p2, pos1, pos2):
        if pos1 in Jungle.ponds and pos2 in Jungle.ponds:
            return True  # rat vs rat
        if pos1 in Jungle.ponds:
            return False  # rat in pond cannot beat any piece on land
        if p1 == Jungle.rat and p2 == Jungle.elephant:
            return True
        if p1 == Jungle.elephant and p2 == Jungle.rat:
            return False
        if p1 >= p2:
            return True
        if pos2 in Jungle.traps:
            return True
        return False

    def pieces_comparison(self):
        for i in range(7, -1, -1):
            ps = []
            for p in [0, 1]:
                if i in self.pieces[p]:
                    ps.append(p)
            if len(ps) == 1:
                return ps[0]
        return None

    def rat_is_blocking(self, player_unused, pos, dx, dy):
        x, y = pos
        nx = x + dx
        for player in [0, 1]:
            if Jungle.rat not in self.pieces[1 - player]:
                continue
            rx, ry = self.pieces[1 - player][Jungle.rat]
            if (rx, ry) not in self.ponds:
                continue
            if dy != 0:
                if x == rx:
                    return True
            if dx != 0:
                if y == ry and abs(x - rx) <= 2 and abs(nx - rx) <= 2:
                    return True
        return False

    def draw(self):
        TT = {0: 'rcdwjtle', 1: 'RCDWJTLE'}
        for y in range(Jungle.MY):

            L = []
            for x in range(Jungle.MX):
                b = self.board[y][x]
                if b:
                    pl, pc = b
                    L.append(TT[pl][pc])
                else:
                    L.append('.')
            print(''.join(L))
        print('')

    def moves(self, player):
        res = []
        for p, pos in self.pieces[player].items():
            x, y = pos
            for (dx, dy) in Jungle.dirs:
                pos2 = (nx, ny) = (x + dx, y + dy)
                if 0 <= nx < Jungle.MX and 0 <= ny < Jungle.MY:
                    if Jungle.dens[player] == pos2:
                        continue
                    if pos2 in self.ponds:
                        if p not in (Jungle.rat, Jungle.tiger, Jungle.lion):
                            continue
                        # if self.board[ny][nx] is not None:
                        #    continue  # WHY??
                        if p == Jungle.tiger or p == Jungle.lion:
                            if dx != 0:
                                dx *= 3
                            if dy != 0:
                                dy *= 4
                            if self.rat_is_blocking(player, pos, dx, dy):
                                continue
                            pos2 = (nx, ny) = (x + dx, y + dy)
                    if self.board[ny][nx] is not None:
                        pl2, piece2 = self.board[ny][nx]
                        if pl2 == player:
                            continue
                        if not self.can_beat(p, piece2, pos, pos2):
                            continue
                    res.append((pos, pos2))
        return res

    def isGameOver(self):
        return self.victory(0) or self.victory(1)

    def victory(self, player):
        oponent = 1 - player
        if len(self.pieces[oponent]) == 0:
            self.winner = player
            return True

        x, y = self.dens[oponent]
        if self.board[y][x]:
            self.winner = player
            return True
        return False

    def do_move(self, m):
        self.curplayer = 1 - self.curplayer
        if m is None:
            return
        pos1, pos2 = m
        x, y = pos1
        pl, pc = self.board[y][x]

        x2, y2 = pos2
        if self.board[y2][x2]:  # piece taken!
            pl2, pc2 = self.board[y2][x2]
            del self.pieces[pl2][pc2]
            self.peace_counter = 0
        else:
            self.peace_counter += 1

        self.pieces[pl][pc] = (x2, y2)
        self.board[y2][x2] = (pl, pc)
        self.board[y][x] = None

    def update(self, player, move_string):
        assert player == self.curplayer
        move = tuple(int(m) for m in move_string.split())
        if len(move) != 4:
            raise WrongMove
        possible_moves = self.moves(player)
        if not possible_moves:
            if move != (-1, -1, -1, -1):
                raise WrongMove
            move = None
        else:
            move = ((move[0], move[1]), (move[2], move[3]))
            if move not in possible_moves:
                raise WrongMove
        self.do_move(move)

        if self.victory(player):
            assert self.winner is not None
            return 2 * self.winner - 1
        else:
            return None
