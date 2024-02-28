def opt_dist(block_list, D):
    cur_sum = 0
    max_sum = len(block_list)
    max_idx = 0
    for i in range(len(block_list)):
        if block_list[i] == 1: cur_sum += 1
        else:
            if(cur_sum > 0 and abs(D - cur_sum) < max_sum):
                max_sum = abs(D - cur_sum)
                max_idx = i-cur_sum
            cur_sum = 0
    if(cur_sum > 0 and abs(D - cur_sum) < max_sum):
        max_sum = abs(D - cur_sum)
        max_idx = i-cur_sum+1
    
    count = 0
    for i in range(len(block_list)):
        if i < max_idx and block_list[i] == 1: count += 1
        elif i >= max_idx and i < max_idx + D and block_list[i] == 0: count += 1
        elif i >= max_idx + D and block_list[i] == 1: count += 1
    
    return count

# print(opt_dist([0, 0, 1, 0, 0, 0, 1, 0, 0, 0], 5))
# print(opt_dist([0, 0, 1, 0, 0, 0, 1, 0, 0, 0], 4))
# print(opt_dist([0, 0, 1, 0, 0, 0, 1, 0, 0, 0], 3))
# print(opt_dist([0, 0, 1, 0, 0, 0, 1, 0, 0, 0], 2))
# print(opt_dist([0, 0, 1, 0, 0, 0, 1, 0, 0, 0], 1))
# print(opt_dist([0, 0, 1, 0, 0, 0, 1, 0, 0, 0], 0))
# print("\n")
# print(opt_dist([0, 0, 1, 0, 1, 0, 1, 0, 0, 0], 5))
# print(opt_dist([0, 0, 1, 0, 1, 0, 1, 0, 0, 0], 4))
# print(opt_dist([0, 0, 1, 0, 1, 0, 1, 0, 0, 0], 3))
# print(opt_dist([0, 0, 1, 0, 1, 0, 1, 0, 0, 0], 2))
# print(opt_dist([0, 0, 1, 0, 1, 0, 1, 0, 0, 0], 1))
# print(opt_dist([0, 0, 1, 0, 1, 0, 1, 0, 0, 0], 0))
# print(opt_dist([0, 0, 0, 0, 0, 0, 0, 0, 0, 1], 1))


inp = open("/home/marcin/UWr/Lato22-23/SI/P1/zad4_input.txt", "r").readlines()
out = open("/home/marcin/UWr/Lato22-23/SI/P1/zad4_output.txt", "w")

for line in inp:
    length = int(line[len(line) - 2])
    s = line[0:len(line) - 3]
    tab = []
    for c in s:
        tab.append(int(c))
    out.write(f'{opt_dist(tab, length)}' + '\n')