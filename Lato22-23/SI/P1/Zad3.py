import random

def figurant():
    karty = [(i, j) for j in ["karo", "kier", "trefl", "pik"] for i in range(11, 15)]

    length = len(karty)
    karty_figuranta = []
    for i in range(5):
        karta = random.randint(0, length-1)
        karty_figuranta.append(karty.pop(karta))
        length -= 1

    return sorted(karty_figuranta)


def blotkarz():
    karty = [(i, j) for j in ["karo", "kier", "trefl", "pik"] for i in range(2, 11)]

    length = len(karty)
    karty_blotkarza = []
    for i in range(5):
        karta = random.randint(0, length-1)
        karty_blotkarza.append(karty.pop(karta))
        length -= 1

    return sorted(karty_blotkarza)


def blotkarzv2():
    karty = [(i, j) for j in ["karo", "kier", "trefl", "pik"] for i in range(8, 11)]

    length = len(karty)
    karty_blotkarza = []
    for i in range(5):
        karta = random.randint(0, length-1)
        karty_blotkarza.append(karty.pop(karta))
        length -= 1

    return sorted(karty_blotkarza)

def poker(karty):
    suma = 0
    kolor = karty[0][1]
    for i in karty:
        if i[1] != kolor: return False
        suma += karty[0][0]
    return suma == ((karty[0][0] + karty[4][0]) * 5 / 2)

# def poker_krolewski(karty):
#     return poker(karty) and karty[0][0] == 14

def kareta(karty):
    return karty[0][0] == karty[3][0] or karty[1][0] == karty[4][0]

def full(karty):
    return (karty[0][0] == karty[2][0] and karty[3][0] == karty[4][0]) or (karty[0][0] == karty[1][0] and karty[2][0] == karty[4][0])

def kolor(karty):
    for i in range(1, len(karty)):
        if karty[0][1] != karty[i][1]: return False
    return True

def strit(karty):
    suma = 0
    for i in karty:
        suma += karty[0][0]
    return suma == ((karty[0][0] + karty[4][0]) * 5 / 2)

def trojka(karty):
    return karty[0][0] == karty[2][0] or karty[1][0] == karty[3][0] or karty[2][0] == karty[4][0]

def dwie_pary(karty):
    return (karty[0][0] == karty[1][0] and karty[2][0] == karty[3][0]) or (karty[0][0] == karty[1][0] and karty[3][0] == karty[4][0]) or (karty[1][0] == karty[2][0] and karty[3][0] == karty[4][0])

def para(karty):
    for i in range(4):
        if karty[i][0] == karty[i+1][0]: return True
    return False

# zwraca True jeżeli figurant wygrał, wpp. False
def zwyciezca(fig, blot):
    if(poker(blot)): return False
    
    if(kareta(fig)): return True
    elif(kareta(blot)): return False

    if(full(blot) or kolor(blot) or strit(blot)): return False

    if(trojka(fig)): return True
    elif(trojka(blot)): return False
    
    if(dwie_pary(fig)): return True
    elif(dwie_pary(blot)): return False

    if(para(fig)): return True
    elif(para(blot)): return False

    return False

def staty():
    iters = 10000
    fig_win = 0
    for i in range(iters):
        blot = blotkarzv2()
        fig = figurant()
        if zwyciezca(fig, blot):
            fig_win += 1

    print(f"Figurant wygrał: {fig_win}, Blotkarz wygrał: {iters-fig_win}.")
    print(f"Szansa na wygraną Blotkarza wynosi: {round((iters-fig_win)/iters*100, 2)}%")


staty()