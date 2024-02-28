def kompresja(tekst):
    res = []
    for i in range(len(tekst)):
        if i > 0 and tekst[i] == tekst[i-1]:
            continue
        if i < len(tekst) - 1 and tekst[i] == tekst[i+1]:
            length = 2
            for j in range(i+2, len(tekst)):
                if tekst[j] == tekst[i]:
                    length += 1
                else:
                    break
            res.append((length, f'{tekst[i]}'))
        else:
            res.append((1, tekst[i]))
    return res

def dekompresja(tekst_skompresowany):
    res = ''
    for i in tekst_skompresowany:
        res += i[0] * i[1]
    return res

print(kompresja('ssssuuuupeer'))
print(dekompresja(kompresja('ssssuuuupeer')))

tekst =  """Fusce fringilla quam eget congue hendrerit. 
Nulla vel lorem nec mauris semper faucibus. 
Aenean mattis libero quam, vel porttitor purus congue vel. 
Morbi dapibus leo magna, eget interdum ligula convallis quis."""

print(kompresja(tekst))
print(dekompresja(kompresja(tekst)))