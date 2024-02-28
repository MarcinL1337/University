import decimal as d

lista = [0.2, 0.5, 4.59, 6]

def vat_faktura(lista):
    suma = 0
    for i in lista:
        suma += i
    return suma * 0.23


def vat_paragon(lista):
    suma = 0
    for i in lista:
        suma += i * 0.23
    return suma


def vat_faktura2(lista):
    suma = d.Decimal(0)
    for i in lista:
        suma += d.Decimal(i)
    return suma * d.Decimal(0.23)


def vat_paragon2(lista):
    suma = d.Decimal(0)
    for i in lista:
        suma += d.Decimal(i) * d.Decimal(0.23)
    return suma


print(vat_faktura(lista) == vat_paragon(lista))
print(vat_faktura2(lista) == vat_paragon2(lista))
print(vat_paragon(lista))
print(vat_faktura(lista))
print(vat_paragon2(lista))
print(vat_faktura2(lista))