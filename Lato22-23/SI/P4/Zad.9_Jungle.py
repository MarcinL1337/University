import numpy as np
from random import choice
from copy import deepcopy
from jungleClass import Jungle


import numpy as np
import random
from random import choice
from copy import deepcopy
from jungleClass import Jungle

idle = (-1, -1)
counter = 0


class Node:
    def __init__(self, parent=None, parent_action=None):
        self.parent = parent
        self.parent_action = parent_action
        self.children = {}
        self.num_of_visits = 0
        self.result = 0
        self.outcome = None

    def add_children(self, children):
        for child in children:
            self.children[child.parent_action] = child

    def value(self):
        explore = 0.5
        if self.num_of_visits == 0:
            return 100000
        else:
            return self.result / self.num_of_visits + explore * np.sqrt(
                2 * np.log(self.parent.num_of_visits) / self.num_of_visits)


class MCTS:
    def __init__(self, state: Jungle):
        self.root_state = deepcopy(state)
        self.root = Node()

    # MCTS functions

    def search(self):
        max_iter = 20
        iteration = 0
        global counter
        counter = 0
        while counter < 20000:
            # print(f"Search Iter: {iteration} Counter: {counter}")
            node, game_state = self.select_node()
            # turn = game_state.toPlay
            # print("Autobots Roll out!")
            outcome = self.autobots_rollout(game_state)
            # print("Backpropagate")
            self.backpropagate(node, outcome)
            # iteration += 1
        # print(iteration)

    def select_node(self):
        node = self.root
        state = deepcopy(self.root_state)

        while len(node.children) != 0:
            children = node.children.values()
            max_value = max(children, key=lambda child: child.value()).value()
            max_nodes = [n for n in node.children.values()
                         if n.value() == max_value]
            node = choice(max_nodes)
            state.do_move(node.parent_action)

            if node.num_of_visits == 0:
                return node, state

        if self.expand(node, state):
            node = choice(list(node.children.values()))
            state.do_move(node.parent_action)
        return node, state

    @staticmethod
    def expand(parent: Node, state: Jungle) -> bool:
        children = []
        if state.isGameOver():
            return False
        for move in state.moves(state.curplayer):
            children.append(Node(parent, move))
        parent.add_children(children)
        return True

    @staticmethod
    def autobots_rollout(state: Jungle) -> int:
        global counter
        while not state.victory(state.curplayer):
            counter += 1
            # printBoard(state.board)
            # print()
            move = state.random_move(state.curplayer)
            state.do_move(move)
        # state.draw()
        # print()
        return state.winner

    @staticmethod
    def backpropagate(node: Node, outcome: int):
        while node is not None:
            node.num_of_visits += 1
            if outcome > 0:
                node.result += 1
            node = node.parent

    def best_move(self) -> tuple:
        if self.root_state.victory(self.root_state.curplayer):
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
            # self.root_state.agentMove(child.parent_action)
            return
        else:
            # print(f"Invalid move: {move}")
            # print(f"Children: {self.root.children}")
            # print(1 - self.root_state.toPlay)
            # printBoard(self.root_state.board)
            # print()
            # raise (Exception("Invalid move, Tree broke"))
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
    for i in range(10):
        print(f"Iteracja nr: {i}")
        gameState = Jungle(i % 2)
        MCTS_Agent = MCTS(gameState)
        while not MCTS_Agent.root_state.isGameOver():
            print(f"Player: {MCTS_Agent.root_state.curplayer}")
            MCTS_Agent.root_state.draw()
            print()
            if MCTS_Agent.root_state.curplayer: #Duże litery
                # print("Search")
                MCTS_Agent.search()
                # print("Best move")
                move = MCTS_Agent.best_move()
                # print("Agent Move")
                MCTS_Agent.root_state.do_move(move)
                # print(f"Player: {MCTS_Agent.root_state.toPlay}")
                # print("Tree Move")
                MCTS_Agent.move(move)
            else: # Małe litery
                move = MCTS_Agent.root_state.min_max_move(MCTS_Agent.root_state.curplayer)
                MCTS_Agent.root_state.do_move(move)
                # if not MCTS_Agent.expand(MCTS_Agent.root, MCTS_Agent.root_state):
                #     continue
                MCTS_Agent.move(move)

        res = MCTS_Agent.root_state.winner
        MCTS_Agent.root_state.draw()
        if res == 0:
            resB += 1
        else:
            resW += 1

    print(f"White: {resW}, Black: {resB}, Draw: {resD}")
