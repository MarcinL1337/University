# import itertools
# from collections import deque
# import copy


# def opt_dist(line, setup, forbidden, is_inferring):
#     how_many = len(setup)
#     length = len(line)

#     def check(choice):
#         for idx, x in enumerate(choice):
#             if x + setup[idx] > length:
#                 return False
#             if idx + 1 < how_many:
#                 if x + setup[idx] + 1 > choice[idx + 1]:
#                     return False
#         return True

#     def find_ok(choice):
#         for idx, x in enumerate(choice[1]):
#             if not ((x & line[idx]) == line[idx]):
#                 return False
#             if x == 1 and forbidden[idx] == 0:
#                 return False
#         return True

#     def generate_line(choice):
#         new_line = [0] * length
#         for idx, x in enumerate(choice):
#             for i in range(setup[idx]):
#                 new_line[x + i] = 1
#         return new_line

#     poss_choices = itertools.combinations([x for x in range(0, length)], how_many)
#     poss_choices = list(map(generate_line, filter(check, poss_choices)))
#     poss_choices = list(filter(find_ok, enumerate(poss_choices)))

#     # Infer guaranteed and impossible tiles
#     if is_inferring:
#         new_line = [1 for i in range(length)]
#         for poss_line in poss_choices:
#             for i in range(length):
#                 new_line[i] = new_line[i] & poss_line[1][i]

#         new_line_2 = [0 for i in range(length)]
#         for poss_line in poss_choices:
#             for i in range(length):
#                 new_line_2[i] = new_line_2[i] or poss_line[1][i]

#         for idx, i in enumerate(new_line_2):
#             if not i:
#                 forbidden[idx] = 0

#         if not len(poss_choices):
#             return None, None

#         for i in new_line:
#             if i:
#                 return new_line, forbidden
#         return None, None
#     else:
#         return [x[1] for x in poss_choices]


# def line_ok(line, setup):
#     line_str = '0' + ''.join(map(str, line)) + '0'
#     seqs = ['0' + '1' * x + '0' for x in setup]
#     for idx, seq in enumerate(seqs):
#         f = line_str.find(seq)
#         if f == -1:
#             return False
#         else:
#             line_str = line_str[f + setup[idx]:]
#     if not (sum(map(int, setup)) == sum(map(int, line))):
#         return False
#     return True


# def canvas_ok(canvas, rows, cols):
#     for i in range(0, len(rows)):
#         if not line_ok(canvas[i], rows[i]):
#             return False
#     rez = [[canvas[j][i] for j in range(len(canvas))] for i in range(len(canvas[0]))]
#     for i in range(0, len(cols)):
#         if not line_ok(rez[i], cols[i]):
#             return False
#     return True


# def print_canvas(canvas):
#     for i in canvas:
#         line = ""
#         for j in range(len(i)):
#             if i[j] == 1:
#                 line += "#"
#             else:
#                 line += "."
#         print(line)
#     print("\n")


# def print_memo(memo):
#     line = ''
#     for is_row, idx in memo:
#         if is_row:
#             line += 'R'
#         else:
#             line += 'C'
#         line += f'{idx} '
#     line += '\n'
#     print(line)


# def inference(canvas, canvas_2, rows, cols):
#     is_changed = True
#     while not canvas_ok(canvas, rows, cols) and is_changed:
#         is_changed = False

#         def infer_lines(is_line_changed):
#             for idx, line in enumerate(canvas):
#                 if not line_ok(line, rows[idx]):
#                     new_line, forbidden = opt_dist(line, rows[idx], canvas_2[idx], True)
#                     if new_line:
#                         canvas[idx] = new_line
#                         canvas_2[idx] = forbidden
#                         if new_line != canvas[idx]:
#                             is_line_changed = True
#                 else:
#                     canvas_2[idx] = line
#             return is_line_changed

#         for i in range(2):
#             is_changed = infer_lines(is_changed)
#             canvas = [[canvas[j][i] for j in range(len(canvas))] for i in range(len(canvas[0]))]
#             canvas_2 = [[canvas_2[j][i] for j in range(len(canvas_2))] for i in range(len(canvas_2[0]))]
#             cols, rows = rows, cols
#     return canvas, canvas_2


# def back_track(canvas, canvas_2, rows, cols, branching_factor, depth, memo):
#     canvas, canvas_2 = inference(canvas, canvas_2, rows, cols)
#     # print(f'Back Track Iter, depth: {depth} branching factor: {branching_factor}')
#     # print_canvas(canvas)
#     if canvas_ok(canvas, rows, cols):
#         return canvas
#     state_queue = deque()
#     is_row = True
#     invalid_lines = 0
#     for i in range(2):
#         for idx, line in enumerate(canvas):
#             if not line_ok(line, rows[idx]):
#                 invalid_lines += 1
#                 poss_moves = opt_dist(line, rows[idx], canvas_2[idx], False)
#                 if len(poss_moves):
#                     if len(poss_moves) < branching_factor and (is_row, idx) not in memo:
#                         for move in poss_moves:
#                             state_queue.append((idx, is_row, move))
#                 else:
#                     return None
#         is_row = False
#         canvas = [[canvas[j][i] for j in range(len(canvas))] for i in range(len(canvas[0]))]
#         canvas_2 = [[canvas_2[j][i] for j in range(len(canvas_2))] for i in range(len(canvas_2[0]))]
#         cols, rows = rows, cols
#     # print(f'Back Track Iter, depth: {depth} branching factor: {branching_factor} invalid_lines: {invalid_lines}'
#     #       f' possible moves: {len(state_queue)}')
#     # print_memo(memo)
#     # print_canvas(canvas)
#     while len(state_queue) > 0:
#         new_canvas = copy.deepcopy(canvas)
#         new_canvas_2 = copy.deepcopy(canvas_2)
#         st_idx, st_is_row, st_move = state_queue.pop()
#         new_memo = copy.copy(memo)
#         new_memo.append((st_is_row, st_idx))
#         if not st_is_row:
#             new_canvas = [[new_canvas[j][i] for j in range(len(new_canvas))] for i in range(len(new_canvas[0]))]
#         new_canvas[st_idx] = st_move
#         if not st_is_row:
#             new_canvas = [[new_canvas[j][i] for j in range(len(new_canvas))] for i in range(len(new_canvas[0]))]
#         res = back_track(new_canvas, new_canvas_2, rows, cols, branching_factor, depth + 1, new_memo)
#         if res:
#             return res
#     return None


# def zad2(rows, cols):
#     canvas = [[0 for j in range(len(cols))] for i in range(len(rows))]
#     canvas_2 = [[-1 for j in range(len(cols))] for i in range(len(rows))]
#     branching_factor = 5
#     res = None
#     while not res:
#         # print(f'Driver Loop Iteration, branching factor: {branching_factor}')
#         new_canvas = copy.deepcopy(canvas)
#         new_canvas_2 = copy.deepcopy(canvas_2)
#         res = back_track(new_canvas, new_canvas_2, rows, cols, branching_factor, 0, [])
#         branching_factor += 5
#     return res


# def main():
#     with open('zad_input.txt') as f:
#         file = f.read().split("\n")
#         rows, cols = map(int, file[0].split())
#         idx = 1
#         row_descs = []
#         col_descs = []
#         for i in range(rows):
#             row_descs.append(list(map(int, file[idx].split())))
#             idx += 1
#         for i in range(cols):
#             col_descs.append(list(map(int, file[idx].split())))
#             idx += 1

#     with open('zad_output.txt', 'w') as f:
#         res = zad2(row_descs, col_descs)
#         for i in res:
#             line = ""
#             for j in range(len(i)):
#                 if i[j] == 1:
#                     line += "#"
#                 else:
#                     line += "."
#             print(line)
#             line += "\n"
#             f.write(line)


# main()


import itertools
import copy
import queue
from collections import deque



def opt_dist(line, setup, forbidden):
    how_many = len(setup)
    length = len(line)

    def check(choice):
        for idx, x in enumerate(choice):
            if x + setup[idx] > length:
                return False
            if idx + 1 < how_many:
                if x + setup[idx] + 1 > choice[idx + 1]:
                    return False
        return True

    def find_ok(choice):
        for idx, x in enumerate(choice[1]):
            if not ((x & line[idx]) == line[idx]):
                return False
            if x == 1 and forbidden[idx] == 0:
                return False
        return True

    def generate_line(choice):
        new_line = [0] * length
        for idx, x in enumerate(choice):
            for i in range(setup[idx]):
                new_line[x + i] = 1
        return new_line

    poss_choices = itertools.combinations([x for x in range(0, length)], how_many)
    poss_choices = list(map(generate_line, filter(check, poss_choices)))
    poss_choices = list(filter(find_ok, enumerate(poss_choices)))

    if len(poss_choices) == 0:
        return [2, None]

    new_line = [1 for i in range(length)]
    for poss_line in poss_choices:
        for i in range(length):
            new_line[i] = new_line[i] & poss_line[1][i]

    new_line_2 = [0 for i in range(length)]
    for poss_line in poss_choices:
        for i in range(length):
            new_line_2[i] = new_line_2[i] or poss_line[1][i]

    for idx, i in enumerate(new_line_2):
        if not i:
            forbidden[idx] = 0

    if sum(new_line) == sum(line):
        return [1, poss_choices, forbidden]

    for i in new_line:
        if i:
            return [0, new_line, forbidden]

    if len(poss_choices):
        return [1, poss_choices, forbidden]
    return [2, None]



def line_ok(line, setup):
    line_str = '0' + ''.join(map(str, line)) + '0'
    seqs = ['0' + '1' * x + '0' for x in setup]
    for idx, seq in enumerate(seqs):
        f = line_str.find(seq)
        if f == -1:
            return False
        else:
            line_str = line_str[f + setup[idx]:]
    if not (sum(map(int, setup)) == sum(map(int, line))):
        return False
    return True


def canvas_ok(canvas, rows, cols):
    for i in range(0, len(rows)):
        if not line_ok(canvas[i], rows[i]):
            return False
    rez = [[canvas[j][i] for j in range(len(canvas))] for i in range(len(canvas[0]))]
    for i in range(0, len(cols)):
        if not line_ok(rez[i], cols[i]):
            return False
    return True


def zad1(rows, cols, nrows, ncols):
    stack = queue.Queue()
    canvas = [[0 for j in range(len(cols))] for i in range(len(rows))]
    canvas_2 = [[-1 for j in range(len(cols))] for i in range(len(rows))]
    stack.put((canvas, canvas_2))
    es = 0
    while not canvas_ok(canvas, rows, cols):
        x = 0
        es += 1
        canvas, canvas_2 = stack.get()
        min_poss_choices = []
        min_poss_choices_num = 99999999999999999
        min_poss_choices_line = 99999
        for idx, line in enumerate(canvas):
            if not line_ok(line, rows[idx]):
                new_line = opt_dist(line, rows[idx], canvas_2[idx])
                if new_line[0] == 0:
                    x += 1
                    canvas[idx] = new_line[1]
                    canvas_2[idx] = new_line[2]
                elif new_line[0] == 1:
                    canvas_2[idx] = new_line[2]
                    if len(new_line[1]) < min_poss_choices_num:
                        min_poss_choices = new_line[1]
                        min_poss_choices_num = len(min_poss_choices)
                        min_poss_choices_line = idx
                else:
                    continue
            else:
                canvas_2[idx] = line

        canvas = [[canvas[j][i] for j in range(len(canvas))] for i in range(len(canvas[0]))]
        canvas_2 = [[canvas_2[j][i] for j in range(len(canvas_2))] for i in range(len(canvas_2[0]))]
        cols, rows = rows, cols

        min_poss_choices_cols = []
        min_poss_choices_num_cols = 99999999999999999
        min_poss_choices_cols_line = 999
        for idx, line in enumerate(canvas):
            if not line_ok(line, rows[idx]):
                new_line = opt_dist(line, rows[idx], canvas_2[idx])
                if new_line[0] == 0:
                    x += 1
                    canvas[idx] = new_line[1]
                    canvas_2[idx] = new_line[2]
                elif new_line[0] == 1:
                    canvas_2[idx] = new_line[2]
                    if len(new_line[1]) < min_poss_choices_num_cols:
                        min_poss_choices_cols = new_line[1]
                        min_poss_choices_num_cols = len(min_poss_choices_cols)
                        min_poss_choices_cols_line = idx
                else:
                    continue
            else:
                canvas_2[idx] = line

        canvas = [[canvas[j][i] for j in range(len(canvas))] for i in range(len(canvas[0]))]
        canvas_2 = [[canvas_2[j][i] for j in range(len(canvas_2))] for i in range(len(canvas_2[0]))]
        cols, rows = rows, cols

        if x == 0:
            if min_poss_choices_num == 99999999999999999 and min_poss_choices_num_cols == 99999999999999999:
                continue
            elif min_poss_choices_num < min_poss_choices_num_cols:
                for poss_moves in min_poss_choices:
                    canvas_temp = copy.deepcopy(canvas)
                    canvas_2_temp = copy.deepcopy(canvas_2)
                    canvas_temp[min_poss_choices_line] = poss_moves[1]
                    stack.put((canvas_temp, canvas_2_temp))
            else:
                for poss_moves in min_poss_choices_cols:
                    canvas_temp = copy.deepcopy(canvas)
                    canvas_2_temp = copy.deepcopy(canvas_2)
                    canvas_temp = [[canvas_temp[j][i] for j in range(len(canvas_temp))] for i in range(len(canvas_temp[0]))]
                    canvas_temp[min_poss_choices_cols_line] = poss_moves[1]
                    canvas_temp = [[canvas_temp[j][i] for j in range(len(canvas_temp))] for i in
                                   range(len(canvas_temp[0]))]
                    stack.put((canvas_temp, canvas_2_temp))
        else:
            stack.put((canvas, canvas_2))

    return canvas


with open('zad_input.txt') as f:
    file = f.read().split("\n")
    rows, cols = map(int, file[0].split())
    idx = 1
    row_descs = []
    col_descs = []
    for i in range(rows):
        row_descs.append(list(map(int, file[idx].split())))
        idx += 1
    for i in range(cols):
        col_descs.append(list(map(int, file[idx].split())))
        idx += 1

with open('zad_output.txt', 'w') as f:
    res = zad1(row_descs, col_descs, rows, cols)
    for i in res:
        line = ""
        for j in range(len(i)):
            if i[j]:
                line += "#"
            else:
                line += "."
        print(line)
        line += "\n"
        f.write(line)