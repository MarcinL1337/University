polish_words = set(open("/home/marcin/UWr/Lato22-23/SI/P1/words_for_ai1.txt", "r").read().splitlines())
tadek = open("/home/marcin/UWr/Lato22-23/SI/P1/pan_tadeusz_bez_spacji.txt", "r")
out = open("/home/marcin/UWr/Lato22-23/SI/P1/zad2_output.txt", "w")


max_word_length = len(max(polish_words, key=len))

def program(line):
    memo = [[0, ''] for i in range(len(line) + 1)]

    for i in range(1, len(line) + 1):
        word = ''
        bound = max(i - max_word_length, -1)
        for j in range(i-1, bound, -1):
            word = line[j:i]
            if word in polish_words:
                if memo[i][0] < (memo[j][0] + len(word) ** 2):
                    memo[i] = [(memo[j][0] + len(word) ** 2), memo[j][1] + ' ' + word]
                    # print(memo)
    return memo[-1][1][1:]

# for line in tadek:
#     # line = line.strip()
#     # line = "".join(line.split())
#     out.write(program(line.strip()) + "\n")

with open("/home/marcin/UWr/Lato22-23/SI/P1/zad2_input.txt", "r") as inp:
    lines = inp.readlines()
    for line in lines:
        out.write(program(line.strip()) + "\n")

out.close()
