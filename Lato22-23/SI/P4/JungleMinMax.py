import numpy as np
import random
from random import choice
from copy import deepcopy
from Zad1_Jungle import MCTS
from jungleClass import Jungle

idle = (-1, -1)
counter = 0


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
        Random_MCTS_Agent = MCTS(gameState)
        res = 0
        while not Random_MCTS_Agent.root_state.isGameOver():
            # print(f"Player: {Random_MCTS_Agent.root_state.curplayer}")
            # Random_MCTS_Agent.root_state.draw()
            # print()
            if Random_MCTS_Agent.root_state.curplayer: # Duże litery
                # print("Search")
                Random_MCTS_Agent.search()
                # print("Best move")
                move = Random_MCTS_Agent.best_move()
                # print("Agent Move")
                Random_MCTS_Agent.root_state.do_move(move)
                Random_MCTS_Agent.move(move)
            else: # Małe litery
                move = Random_MCTS_Agent.root_state.min_max_move(Random_MCTS_Agent.root_state.curplayer)

                Random_MCTS_Agent.root_state.do_move(move)
                Random_MCTS_Agent.move(move)

        Random_MCTS_Agent.root_state.draw()
        res = Random_MCTS_Agent.root_state.winner
        if res == 0:
            resB += 1
        else:
            resW += 1

    print(f"White: {resW}, Black: {resB}, Draw: {resD}")
