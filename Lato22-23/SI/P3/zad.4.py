def B(i,j):
    return 'B_%d_%d' % (i,j)

def rowcolsum(rows, cols):
    res = []
    for i in range(len(rows)):
        temp = []
        for j in range(len(cols)):
            temp.append(B(i, j))
        tempstr = " + ".join(temp)
        tempstr += " #= " + str(rows[i])
        res.append(tempstr)
    for i in range(len(cols)):
        temp = []
        for j in range(len(rows)):
            temp.append(B(j, i))
        tempstr = " + ".join(temp)
        tempstr += " #= " + str(cols[i])
        res.append(tempstr)
    return res

def triplehorizontal(row, col):
    res = []
    for i in range(row):
        for j in range(1, col - 1):
            X = B(i, j - 1)
            Y = B(i, j)
            Z = B(i, j + 1)
            res.append(f"tuples_in( [[{X}, {Y}, {Z}]], [ [0,0,0], [1,1,0], [1,0,0], [0,1,1], [0,0,1], [1,1,1], [1,0,1]])")
    return res

def triplevertical(row, col):
    res = []
    for i in range(col):
        for j in range(1, row - 1):
            X = B(j - 1, i)
            Y = B(j, i)
            Z = B(j + 1, i)
            res.append(f"tuples_in( [[{X}, {Y}, {Z}]], [ [0,0,0], [1,1,0], [1,0,0], [0,1,1], [0,0,1], [1,1,1], [1,0,1]])")
    return res

def box(row, col):
    res = []
    for i in range(0, row - 1):
        for j in range(0, col - 1):
            X = B(i, j)
            Y = B(i, j + 1)
            Z = B(i + 1, j)
            A = B(i + 1, j + 1)
            #res.append(f"({X},{A}) #<=> ({Y}, {Z})")
            res.append(f"tuples_in( [[{X}, {Y}, {Z}, {A}]], [ [1,1,1,1], [1,0,1,0], [0,0,1,1], [0,1,0,1], [1,1,0,0], [1,0,0,0], [0,1,0,0], [0,0,1,0], [0,0,0,1], [0,0,0,0]])")
    return res

def startfields(triples):
    res = []
    for i in triples:
        res.append(B(i[0], i[1]) + " #= " + str(i[2]))
    return res

def domain(Bs):
    res = []
    for b in Bs:
        res.append(b + " in 0..1")
    return res

def print_constraints(Cs, indent, d):
    position = indent
    output.write(indent * ' ')
    for c in Cs:
        output.write(c + ', ')
        position += len(c)
        if position > d:
            position = indent
            output.write("\n")
            output.write(indent * ' ')

def storms(rows, cols, triples):
    writeln(':- use_module(library(clpfd)).')
    
    R = len(rows)
    C = len(cols)
    
    bs = [ B(i,j) for i in range(R) for j in range(C)]
    
    writeln('solve([' + ', '.join(bs) + ']) :- ')
    
    #TODO: add some constraints
    cs = domain(bs) + triplehorizontal(len(rows), len(cols)) + triplevertical(len(rows), len(cols)) + box(len(rows), len(cols)) + rowcolsum(rows, cols)  + startfields(triples)
    print_constraints(cs, 4, 70)

    writeln('\n    labeling([ff], [' +  ', '.join(bs) + ']).' )
    writeln('')
    writeln(":- tell('prolog_result.txt'), solve(X), write(X), nl, told.")

def writeln(s):
    #print(s)
    output.write(s + '\n')

txt = open('zad_input.txt').readlines()
output = open('zad_output.txt', 'w')

rows = list(map(int, txt[0].split()))
cols = list(map(int, txt[1].split()))
triples = []

for i in range(2, len(txt)):
    if txt[i].strip():
        triples.append(list(map(int, txt[i].split())))


storms(rows, cols, triples)  