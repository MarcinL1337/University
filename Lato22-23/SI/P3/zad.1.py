import random
import itertools


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

    for i in new_line:
        if i:
            return [new_line, forbidden]
    return None



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
    x = 0
    canvas = [[0 for j in range(len(cols))] for i in range(len(rows))]
    canvas_2 = [[-1 for j in range(len(cols))] for i in range(len(rows))]
    while not canvas_ok(canvas, rows, cols):
        for idx, line in enumerate(canvas):
            if not line_ok(line, rows[idx]):
                new_line = opt_dist(line, rows[idx], canvas_2[idx])
                if new_line:
                    canvas[idx] = new_line[0]
                    canvas_2[idx] = new_line[1]
            else:
                canvas_2[idx] = line

        x += 1
        canvas = [[canvas[j][i] for j in range(len(canvas))] for i in range(len(canvas[0]))]
        canvas_2 = [[canvas_2[j][i] for j in range(len(canvas_2))] for i in range(len(canvas_2[0]))]
        cols, rows = rows, cols

    if x % 2:
        canvas = [[canvas[j][i] for j in range(len(canvas))] for i in range(len(canvas[0]))]
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
