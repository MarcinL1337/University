def is_palindrom(tekst):
    tekst = ''.join([ch for ch in tekst if ch.isalpha()]).lower()
    for i in range(len(tekst)):
        if tekst[i] != tekst[-1 - i]:
            return False
    return True

def is_palindrom2(tekst):
    tekst = ''.join([ch for ch in tekst if ch.isalpha()]).lower()
    return tekst == tekst[::-1]


print(is_palindrom("Kobyła ma mały bok"))
print(is_palindrom("rotor"))
print(is_palindrom("oko"))
print(is_palindrom("Eine güldne, gute Tugend: Lüge nie!"))
print(is_palindrom("Míč omočím."))
print(is_palindrom("coś"))
print("--------------------------------")
print(is_palindrom2("Kobyła ma mały bok"))
print(is_palindrom2("rotor"))
print(is_palindrom2("oko"))
print(is_palindrom2("Eine güldne, gute Tugend: Lüge nie!"))
print(is_palindrom2("Míč omočím."))
print(is_palindrom2("coś"))
