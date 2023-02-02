from math import log2

def expanded_euclid(a: int, b: int) -> int:
    numbers = []
    if a < b:
        a = a ^ b
        b = a ^ b
        a = a ^ b
    temp = a
    while b > 1:
        numbers.append(a // b)
        retain = a % b
        a = b
        b = retain
    numbers = [0, 1] + numbers
    for i in range(2, len(numbers)):
        numbers[i] = -(numbers[i]) * numbers[i - 1] + numbers[i - 2]
    return numbers[len(numbers) - 1] % temp

def gcd(a: int, b: int) -> int:
    if b == 0:
        return a
    return gcd(b, a % b)

def linear_equations(y: int, x: int, mod: int = 961) -> list:
    n = gcd(x, mod)
    if n == 1:
        return [(y * expanded_euclid(x, mod)) % mod]
    else:
        if (y % n) != 0:
            return []
        else:
            y //= n
            x //= n
            mod_t = mod // n
            x0 = (y * expanded_euclid(x, mod_t))
            return [(x0 + mod_t * i) % mod for i in range(n)]

def athenian_decryption(filename: str, mod: int = 961):
    cipher_text = "".join(open(filename, "r", encoding="utf-8").read().split())
    bigrams_count = dict(zip(bigrams, [0] * bigrams_length))
    for index in range(0, len(cipher_text) - 1, 2):
        bigrams_count[cipher_text[index] + cipher_text[index + 1]] += 1
    bigrams_count = {bigram: count for bigram, count in
                     sorted(bigrams_count.items(), key=lambda item: item[1], reverse=True)}
    most_frequent_bigrams = list(bigrams_count.keys())[:5]
    decrypted_text_list = list(cipher_text)
    decrypted_text_size = len(decrypted_text_list)
    all_a = []
    length = 5
    check = 0
    for index1 in range(length):
        for index2 in range(length):
            for index3 in range(length):
                for index4 in range(length):
                    if index2 == index1 or index3 == index4:
                        continue
                    X0 = bigrams_numbers[bigrams_sorted_by_possibility[index1]]
                    X1 = bigrams_numbers[bigrams_sorted_by_possibility[index2]]
                    Y0 = bigrams_numbers[most_frequent_bigrams[index3]]
                    Y1 = bigrams_numbers[most_frequent_bigrams[index4]]
                    possible_a = linear_equations((Y0 - Y1) % mod, (X0 - X1) % mod)
                    for x in possible_a:
                        if x in all_a:
                            possible_a.remove(x)
                        else:
                            all_a.append(x)
                    if not possible_a:
                        continue
                    possible_b = [0] * len(possible_a)
                    for i in range(len(possible_a)):
                        possible_b[i] = (Y0 - possible_a[i] * X0) % mod
                    for curr in range(len(possible_a)):
                        a = possible_a[curr]
                        b = possible_b[curr]
                        decrypted_text_list_copy = decrypted_text_list.copy()
                        alphabet_count = dict(zip(alphabet, [0] * alphabet_length))
                        for index in range(0, decrypted_text_size - 1, 2):
                            Y = bigrams_numbers[decrypted_text_list_copy[index] + decrypted_text_list_copy[index + 1]]
                            X = (expanded_euclid(a, mod) * (Y - b)) % mod
                            bigram = bigrams[X]
                            alphabet_count[bigram[0]] += 1
                            alphabet_count[bigram[1]] += 1
                            decrypted_text_list_copy[index] = bigram[0]
                            decrypted_text_list_copy[index + 1] = bigram[1]
                        if max(alphabet_count, key=alphabet_count.get) not in ("о", "е", "а", "и"):
                            continue
                        alphabet_poss = [i / decrypted_text_size for i in alphabet_count.values()]
                        entropy = -sum([i * log2(i) for i in alphabet_poss if i > 0])
                        if entropy > 4.5:
                            continue
                        print(f"a = {a}, b = {b}, Entropy: H = {entropy}")
                        print(str(decrypted_text_list_copy[:100]))
                        option = input("Does this look like valid text?\n \
                                    Y/y –→ for writing text in file\n \
                                    N/n –→ to continue decrypting\n")
                        if option.upper() == "Y":
                            answer = open("decrypted_" + filename, "w")
                            answer.write("".join(decrypted_text_list_copy))
                            answer.close()
                            print("Text was written to the file: decrypted_" + filename)
                            check = 1
                            break
                    if check:
                        break
                if check:
                    break
            if check:
                break
        if check:
            break
    return 0

alphabet = ["а", "б", "в", "г", "д", "е", "ж", "з", "и", "й", "к", "л", "м", "н", "о", "п", "р", "с", "т", "у",
            "ф", "х", "ц", "ч", "ш", "щ", "ы", "ь", "э", "ю", "я"]
alphabet_length = len(alphabet)
alphabet_numbers = dict(zip(alphabet, [i for i in range(alphabet_length)]))

bigrams = [i + j for i in alphabet for j in alphabet]
bigrams_length = len(bigrams)
bigrams_numbers = dict(zip(bigrams, [i for i in range(bigrams_length)]))
bigrams_sorted_by_possibility = ["ст", "но", "то", "на", "ен"]

cipher_text = input("Enter relative path of file to decrypt: ")
athenian_decryption(cipher_text)
