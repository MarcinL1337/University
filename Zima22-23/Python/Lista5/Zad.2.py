import itertools

class Formula:
    zmienne = {}

    def __init__(self):
        pass

    def __add__(self, f1):
        return And(self, f1)

    def __mul__(self, f1):
        return Or(self, f1)

    def tautologia(self):
        wartosciowania = list(itertools.product([True, False], repeat=len(self.zmienne)))
        for i in wartosciowania:
            d = dict(zip(self.zmienne.keys(), i))
            # print(self.oblicz(d))
            if not self.oblicz(d): return False
        return True

    def uprosc(self):
        if isinstance(self, Or):
            if isinstance(self.f1, Stala) and self.f1.oblicz() == False:
                return self.f2.uprosc()
            elif isinstance(self.f2, Stala) and self.f2.oblicz() == False:
                return self.f1.uprosc()
            return Or(self.f1.uprosc(), self.f2.uprosc()).uprosc()


        if isinstance(self, And):
            if isinstance(self.f1, Stala) and self.f1.oblicz() == False:
                return self.f1.uprosc()
            elif isinstance(self.f2, Stala) and self.f2.oblicz() == False:
                return self.f2.uprosc()
            return And(self.f1.uprosc(), self.f2.uprosc()).uprosc()

        return self



class Stala(Formula):
    def __init__(self, stala: bool):
        self.stala = stala

    def oblicz(self, zmienne = {}):
        return self.stala

    def __str__(self):
        return f"{self.stala}"


class Zmienna(Formula):
    def __init__(self, nazwa: str):
        self.nazwa = nazwa
    def oblicz(self, zmienne):
        return zmienne[self.nazwa]

    def __str__(self):
        return f"{self.nazwa}"


class And(Formula):
    def __init__(self, f1, f2):
        self.f1 = f1
        self.f2 = f2

    def oblicz(self, zmienne):
        return self.f1.oblicz(zmienne) and self.f2.oblicz(zmienne)

    def __str__(self):
        return f"({str(self.f1)} and {str(self.f2)})"


class Or(Formula):
    def __init__(self, f1, f2):
        self.f1 = f1
        self.f2 = f2

    def oblicz(self, zmienne):
        return self.f1.oblicz(zmienne) or self.f2.oblicz(zmienne)

    def __str__(self):
        return f"({str(self.f1)} or {str(self.f2)})"


class Not(Formula):
    def __init__(self, f):
        self.f = f
    
    def oblicz(self, zmienne):
        return not(self.f.oblicz(zmienne))

    def __str__(self):
        return f"(not {str(self.f)})"


x = Zmienna("x")
f4 = Or(Stala(False), x)
f5 = And(Stala(False), x)

print((f5 * f5) * f5)
print(((f5 * f5) * f5).uprosc())
print((f5 * f5) + f5)
print(((f5 * f5) + f5).uprosc())
print(f4)
print(f4.uprosc())
print(x)
print(x.uprosc())

print(f4.oblicz({"x": False}))