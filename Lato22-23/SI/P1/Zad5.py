import random

inp = open("/home/marcin/UWr/Lato22-23/SI/P1/zad5_input.txt", "r")

# Robi to co opt_dist ale generuje wszystkie możliwe optymalne. Zwraca losową parę zawierającą min ilość operacji i indeks najlepszego ciągu
def opt_dist(xs, D):
    min_inv = len(xs)
    min_idx = len(xs)
    result = []
    inv = 0
    i = j = 0
    while j != D:
        if not xs[j]:
            inv += 1
        j += 1
    while j != len(xs) + 1:
        if j <= len(xs):
            extra = sum(xs[0:i]) + sum(xs[j:len(xs)])
            if min_inv >= inv + extra:
                min_inv = inv + extra
                min_idx = i
                result.append([min_inv, min_idx])
        if j == len(xs):
            break
        inv -= (not xs[i])
        i += 1
        if not xs[j]:
            inv += 1
        j += 1

    res = list(filter(lambda x: x[0] == min_inv, result))
    return random.choice(res)


def isImgGood(canvas, rows, cols):
    for i in range(0, len(rows)):
        if opt_dist(canvas[i], rows[i])[0]:
            return False
    res = [[canvas[j][i] for j in range(len(canvas))] for i in range(len(canvas[0]))] #transpozycja
    for i in range(0, len(cols)):
        if opt_dist(res[i], cols[i])[0]:
            return False
    return True


def line_ok(line, num):
    if opt_dist(line, num)[0]:
        return False
    return True

def foo(rows, cols):
    nrows = len(rows)
    ncols = len(cols)
    max_iters = 0
    img = [[0 for j in range(ncols)] for i in range(nrows)]
    while not isImgGood(img, rows, cols):
        if max_iters > 10000:
            max_iters = 0
            img = [[0 for j in range(nrows)] for i in range(ncols)]

        transpose = random.randint(0, 1)
        if transpose:
            img = [[img[j][i] for j in range(len(img))] for i in range(len(img[0]))]
            cols, rows = rows, cols

        line = random.choice(img)
        max_iters2 = 0
        while line_ok(line, rows[img.index(line)]) and max_iters2 <= 10:
            max_iters2 += 1
            line = random.choice(img)
        res = opt_dist(line, rows[img.index(line)])
        pixel = random.randint(0, len(cols)-1)
        if res[1] <= pixel < res[1] + rows[img.index(line)]:
            img[img.index(line)][pixel] = 1
        else:
            img[img.index(line)][pixel] = 0

        if transpose:
            img = [[img[j][i] for j in range(len(img))] for i in range(len(img[0]))]
            cols, rows = rows, cols
        max_iters += 1
    # print(max_iters)
    return img

out = open("/home/marcin/UWr/Lato22-23/SI/P1/zad5_output.txt", "w")


inp = inp.read().split()

rows = int(inp[0])
cols = int(inp[1])

rowTab = [int(inp[i]) for i in range(2, rows + 2)]
colTab = [int(inp[i]) for i in range(2 + rows, rows + 2 + cols)]

def printImg(img):
    for i in img:
        line = ""
        for j in range(len(i)):
            if i[j]:
                line += "#"
            else:
                line += "."
        out.write(line+'\n')


printImg(foo(rowTab, colTab))