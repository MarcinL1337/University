import math
import timeit
import pandas as pd


def is_prime(n):
    if n < 2:
        return False
    if n == 2:
        return True
    for i in range(2, int(math.sqrt(n) + 1)):
        if n % i == 0:
            return False
    return True


def imper(n):
    num = 0
    res = []
    while num <= n:
        if is_prime(num):
            res.append(num)
        num += 1
    return res


def skladana(n):
    res = range(n+1)
    return [num for num in res if is_prime(num)]


def fun(n):
    res = range(n+1)
    return list(filter(lambda x : is_prime(x), res))


argument, imper_data, skladana_data, fun_data = [], [], [], []

for i in range(1, 11):
    start_time = timeit.default_timer()
    imper(10000*i)
    czas = timeit.default_timer() - start_time
    imper_data.append(czas)

    start_time = timeit.default_timer()
    skladana(10000*i)
    czas = timeit.default_timer() - start_time
    skladana_data.append(czas)

    start_time = timeit.default_timer()
    fun(10000*i)
    czas = timeit.default_timer() - start_time
    fun_data.append(czas)

    argument.append(10000*i)

data = {
    "Argument": argument,
    "Imperatywne": imper_data,
    "Składane": skladana_data,
    "Funkcyjne": fun_data
}


print(pd.DataFrame(data))