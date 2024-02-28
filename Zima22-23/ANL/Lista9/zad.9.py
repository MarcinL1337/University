import plotly.express as px
import math

punkty = [(0, 0),
          (3.5, 36),
          (25, 25),
          (25, 1.5),
          (-5, 3),
          (-5, 33),
          (15, 11),
          (-0.5, 35),
          (19.5, 15.5),
          (7, 0),
          (1.5, 10.5)]

wagi = [1, 6, 4, 2, 3, 4, 2, 1, 5, 4, 1]


def fact(x):
    return math.factorial(x)

def B(k, n):
    def pom(x): return (fact(n) / (fact(k) * fact(n-k))) * pow(x, k) * pow((1-x), (n-k))
    return pom

def show(x, y):
    fig = px.line(range_x=[-10, 40], range_y=[-10, 40])
    fig.add_scatter(x=x, y=y, line_color='blue', line_width=4)
    fig.update_layout(height=900, width=900)
    fig.show()

def zad_9():
    n = len(punkty)
    fun = [B(i, n-1) for i in range(n)]
    x = []
    y = []
    waga = [i**1 for i in wagi]
    zakres = 100
    for k in range(zakres + 1):
        j = k/zakres
        licznik = [0,0]
        mianownik = 0
        for i in range(n):
            licznik[0] += waga[i] * fun[i](j) * punkty[i][0]
            licznik[1] += waga[i] * fun[i](j) * punkty[i][1]
            mianownik += waga[i] * fun[i](j)
            print(fun[i](j))
        licznik[0] /= mianownik
        licznik[1] /= mianownik
        x.append(licznik[0])
        y.append(licznik[1])
    show(x, y)

zad_9()