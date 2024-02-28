import random

def uprosc_zdanie(tekst, dl_slowa, liczba_slow):
    tekst = tekst.split()
    res = []
    for slowo in tekst:
        if len(slowo) <= dl_slowa: res.append(slowo)
    while len(res) > liczba_slow:
        slowo = res[(random.randint(0, len(res)-1))]
        res.remove(slowo)
    return " ".join(res)


print(uprosc_zdanie("Podział peryklinalny inicjałów wrzecionowatych \
kambium charakteryzuje się ścianą podziałową inicjowaną \
w płaszczyźnie maksymalnej.", 10, 5))

print(uprosc_zdanie("""Phasellus commodo odio in urna congue sodales a et neque. 
Nunc sodales ipsum lectus, vel porttitor justo porta sit amet. 
Sed a augue id odio feugiat rutrum condimentum id diam.""", 6, 10))