import math
from tabulate import tabulate

#---------------------Zad_3---------------------

def f3(x): return x - 0.49

def Zad3():
    a = 0
    b = 1
    s = (a + b) / 2
    if f3(s) < 0: a = s
    else: b = s
    for i in range(1, 6):
        s = (a + b) / 2
        e = abs(0.49 - s) - pow(2, -i - 1)
        print("|e_n| = ", abs(0.49 - s))
        print("abs(0.49 - s) <= pow(2, -i - 1): ", abs(0.49 - s) <= pow(2, -i - 1))
        print("|e_n| - oszacowanie = ", e)
        if f3(s) < 0: a = s
        else: b = s

#---------------------Zad_4---------------------

def bisekcja(f, a, b, e):
    while True:
        s = (a + b)/2
        if (b - a) <= e or f(s) == 0: return s
        if f(a) * f(s) > 0: a = s
        else: b = s

def g(x): return pow(x,4)
def h(x): return 6 * math.sin(3*x - 1)
def f(x): return g(x) - h(x)

def Zad4():
    punkty = [(-2, -1), (-1, 0), (0, 1), (1, 2)]
    for i in range(len(punkty)):
        x = bisekcja(f, punkty[i][0], punkty[i][1], 1e-8)
        print(f"x{i+1} : {x}")

#---------------------Zad_6---------------------
count = 0

def f6(x, a):
    global count
    count += 1
    return 0.5 * x * (3 - a * x * x)

def Zad6():
    tab = []
    for i in range(0, 20):
        pom = i/20
        global count
        count = 0
        while abs(pom - 0.5) > 1e-8 and count < 50:
            pom = f6(pom, 4)
        tab.append([i/20, count, pom])
    print(tabulate(tab, headers=['x0', 'powtórzenia','wynik'], floatfmt=".15f"))

Zad3()
