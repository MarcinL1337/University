from SlowlyLoosingSanity import MCTS, GameState, printBoard
import sys


class Player(object):
    def __init__(self):
        self.say("RDY")
        self.game = GameState(0)
        self.MCTS = MCTS(self.game)

    def reset(self):
        self.game = GameState(0)
        self.MCTS = MCTS(self.game)
        self.say("RDY")

    @staticmethod
    def say(what):
        sys.stdout.write(what)
        sys.stdout.write('\n')
        sys.stdout.flush()

    @staticmethod
    def hear():
        line = sys.stdin.readline().split()
        return line[0], line[1:]

    def loop(self):
        while True:
            cmd, args = self.hear()
            if cmd == 'HEDID':
                unused_move_timeout, unused_game_timeout = args[:2]
                move = (int(args[-1]), int(args[-2]))
                # if move == (-1, -1):
                #     move = None
                self.MCTS.root_state.agentMove(move)
                self.MCTS.move(move)
                # print("DUPA________________", file=sys.stderr)
                # printBoard(self.MCTS.root_state.board)
                # print("DUPA________________", file=sys.stderr)
            elif cmd == 'ONEMORE':
                self.reset()
                continue
            elif cmd == 'BYE':
                break
            else:
                assert cmd == 'UGO'
                self.MCTS.root_state.toPlay = 0
                self.MCTS.root_state.ourPlayer = 0

            self.MCTS.search()
            move = self.MCTS.best_move()
            self.MCTS.root_state.agentMove(move)
            self.MCTS.move(move)
            self.say('IDO %d %d' % (move[1], move[0]))


if __name__ == '__main__':
    player = Player()
    player.loop()
