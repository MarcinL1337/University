def pierwiastek(n):
    k = i = 0
    while n > k:
        i += 1
        k += 2*i-1
    return i if n == k else i - 1

print("pierwiastek(5) =", pierwiastek(5))
print("pierwiastek(16) =", pierwiastek(16))
print("pierwiastek(20) =", pierwiastek(20))
print("pierwiastek(39) =", pierwiastek(39))
print("pierwiastek(49) =", pierwiastek(49))