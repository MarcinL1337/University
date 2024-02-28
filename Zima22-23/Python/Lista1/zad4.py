import random

def lotto():
    ilosc = 1
    pierwsza = random.randint(1, 49)
    liczby = [pierwsza]
    while ilosc < 6:
        if liczby[ilosc-1] > 36:
            liczby.append(liczby[ilosc-1] - 36)
        else:
            liczby.append(liczby[ilosc-1] + 13)
        ilosc += 1
    return liczby


print(lotto())