import timeit
import pandas as pd

def doskonala(n):
    res = []
    for i in range(1, n):
        if n % i == 0:
            res.append(i)
    return sum(res) == n


def imper(n):
    res = []
    for i in range(1, n+1):
        if doskonala(i):
            res.append(i)
    return res


def skladana(n):
    res = range(1, n+1)
    return [num for num in res if doskonala(num)]


def fun(n):
    res = range(1, n+1)
    return list(filter(lambda x: doskonala(x), res))


argument, imper_data, skladana_data, fun_data = [], [], [], []

for i in range(1, 11):
    start_time = timeit.default_timer()
    imper(1000*i)
    czas = timeit.default_timer() - start_time
    imper_data.append(czas)

    start_time = timeit.default_timer()
    skladana(1000*i)
    czas = timeit.default_timer() - start_time
    skladana_data.append(czas)

    start_time = timeit.default_timer()
    fun(1000*i)
    czas = timeit.default_timer() - start_time
    fun_data.append(czas)

    argument.append(1000*i)

data = {
    "Argument": argument,
    "Imperatywne": imper_data,
    "Składane": skladana_data,
    "Funkcyjne": fun_data
}


print(pd.DataFrame(data))