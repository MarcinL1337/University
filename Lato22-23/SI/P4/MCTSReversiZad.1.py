import numpy as np
from random import choice
from copy import deepcopy

idle = (-1, -1)


class GameState:
    def __init__(self):
        self.board = self.initial_board()
        self.toPlay = 1
        self.freePos = self.freePositions()
        self.whiteLegalMoves = set(self.getLegalMoves(1))
        self.blackLegalMoves = set(self.getLegalMoves(0))

    directions = [(0, 1), (1, 0), (-1, 0), (0, -1), (1, 1), (-1, -1), (1, -1), (-1, 1)]

    @staticmethod
    def initial_board():
        board = [[-1] * 8 for i in range(8)]
        board[3][3] = 1
        board[4][4] = 1
        board[3][4] = 0
        board[4][3] = 0
        return board

    def freePositions(self):
        free_positions = set()
        for i in range(8):
            for j in range(8):
                if self.board[i][j] == -1:
                    free_positions.add((i, j))
        return free_positions

    @staticmethod
    def isInsideBoard(x, y):
        return 0 <= x <= 7 and 0 <= y <= 7

    def canFlank(self, player, x, y, direction):
        x += direction[0]
        y += direction[1]

        if not self.isInsideBoard(x, y):
            return False

        counter = 0
        while self.board[x][y] == 1 - player:
            counter += 1
            x += direction[0]
            y += direction[1]
            if not self.isInsideBoard(x, y):
                return False
        return counter > 0 and self.board[x][y] == player

    def getLegalMoves(self, player):
        state_free_pos = self.freePos
        res = []
        if any(state_free_pos):
            for x, y in state_free_pos:
                if any(self.canFlank(player, x, y, direction) for direction in GameState.directions):
                    res.append((x, y))
        if not res:
            return [idle]
        return res

    def accessLegalMoves(self):
        if self.toPlay:
            return self.whiteLegalMoves
        else:
            return self.blackLegalMoves

    def isGameOver(self):
        if idle not in self.whiteLegalMoves or idle not in self.blackLegalMoves:
            return False
        return True

    def result(self):
        res = 0

        for i in range(8):
            for j in range(8):
                if self.board[i][j] == 0:
                    res -= 1
                elif self.board[i][j] == 1:
                    res += 1

        if res > 0:
            return 1
        elif res < 0:
            return -1
        else:
            return 0

    def flipDiscs(self, move, player):
        for dx, dy in self.directions:
            x, y = move
            to_flip = []
            x += dx
            y += dy
            while self.isInsideBoard(x, y) and self.board[x][y] == 1 - player:
                to_flip.append((x, y))
                x += dx
                y += dy
            if self.isInsideBoard(x, y) and self.board[x][y] == player:
                for fx, fy in to_flip:
                    self.board[fx][fy] = player

    def agentMove(self, move):
        if not move == idle:
            self.board[move[0]][move[1]] = self.toPlay
            self.flipDiscs(move, self.toPlay)
        self.updateState()

    def randomMove(self):
        poss_moves = self.whiteLegalMoves if self.toPlay else self.blackLegalMoves
        if idle in poss_moves:
            self.updateState()
            return idle
        x, y = poss_moves.pop()
        self.board[x][y] = self.toPlay
        self.flipDiscs((x, y), self.toPlay)
        self.updateState()
        return x, y

    def updateState(self):
        self.toPlay = 1 - self.toPlay
        self.freePos = self.freePositions()
        self.whiteLegalMoves = set(self.getLegalMoves(1))
        self.blackLegalMoves = set(self.getLegalMoves(0))


class Node:
    def __init__(self, parent=None, parent_action=None):
        self.parent = parent
        self.parent_action = parent_action
        self.children = {}
        self.num_of_visits = 0
        self.result = 0
        # self.outcome = None

    def add_children(self, children):
        for child in children:
            self.children[child.parent_action] = child

    def value(self):
        explore = 0.2
        if self.num_of_visits == 0:
            return 100000
        else:
            return self.result / self.num_of_visits + explore * np.sqrt(
                2 * np.log(self.parent.num_of_visits) / self.num_of_visits)


class MCTS:
    def __init__(self, state: GameState):
        self.root_state = deepcopy(state)
        self.root = Node()

    # MCTS functions

    def search(self):
        max_iter = 7
        for iteration in range(max_iter):
            # print(f"Search Iter: {iteration}")
            node, game_state = self.select_node()
            # turn = game_state.toPlay
            # print("Autobots Roll out!")
            outcome = self.autobots_rollout(game_state)
            # print("Backpropagate")
            self.backpropagate(node, outcome)

    def select_node(self):
        node = self.root
        state = deepcopy(self.root_state)

        while len(node.children) != 0:
            children = node.children.values()
            max_value = max(children, key=lambda child: child.value()).value()
            max_nodes = [n for n in node.children.values()
                         if n.value() == max_value]
            node = choice(max_nodes)
            state.agentMove(node.parent_action)

            if node.num_of_visits == 0:
                return node, state

        if self.expand(node, state):
            node = choice(list(node.children.values()))
            state.agentMove(node.parent_action)
        return node, state

    @staticmethod
    def expand(parent: Node, state: GameState) -> bool:
        children = []
        if state.isGameOver():
            return False
        for move in state.accessLegalMoves():
            children.append(Node(parent, move))
        parent.add_children(children)
        return True

    @staticmethod
    def autobots_rollout(state: GameState) -> int:
        while not state.isGameOver():
            # printBoard(state.board)
            # print()
            state.randomMove()
        return state.result()

    @staticmethod
    def backpropagate(node: Node, outcome: int):
        while node is not None:
            node.num_of_visits += 1
            if outcome >= 0:
                node.result += 1
            node = node.parent

    def best_move(self) -> tuple:
        if self.root_state.isGameOver():
            raise (Exception("The game is over in best_move"))
        nodes = [n for n in self.root.children.values() if n.num_of_visits != 0]
        max_node = max(nodes, key=lambda n: n.result / n.num_of_visits)
        max_value = max_node.result / max_node.num_of_visits
        max_nodes = [n for n in nodes if n.result / n.num_of_visits == max_value]
        best_child = choice(max_nodes)
        return best_child.parent_action

    def move(self, move: tuple):
        if move in self.root.children:
            child = self.root.children[move]
            child.parent = None
            self.root = child
            return
        else:
            self.root = Node()


def printBoard(board):
    for i in range(8):
        for j in range(8):
            if board[i][j] != -1:
                print(board[i][j], end=" ")
            else:
                print("*", end=" ")
        print()


if __name__ == '__main__':
    resW = 0
    resB = 0
    resD = 0
    for i in range(1000):
        print(f"Iteracja nr: {i}")
        gameState = GameState()
        MCTS_Agent = MCTS(gameState)
        while not MCTS_Agent.root_state.isGameOver():
            # print(f"Player: {MCTS_Agent.root_state.toPlay}")
            # printBoard(MCTS_Agent.root_state.board)
            # print()
            if MCTS_Agent.root_state.toPlay:
                # print("Search")
                MCTS_Agent.search()
                # print("Best move")
                move = MCTS_Agent.best_move()
                # print("Agent Move")
                MCTS_Agent.root_state.agentMove(move)
                # print(f"Player: {MCTS_Agent.root_state.toPlay}")
                # print("Tree Move")
                MCTS_Agent.move(move)
            else:
                move = MCTS_Agent.root_state.randomMove()
                # if not MCTS_Agent.expand(MCTS_Agent.root, MCTS_Agent.root_state):
                #     continue
                MCTS_Agent.move(move)

        res = MCTS_Agent.root_state.result()
        if res < 0:
            resB += 1
        elif res > 0:
            resW += 1
        else:
            resD += 1

    print(f"White: {resW}, Black: {resB}, Draw: {resD}")
