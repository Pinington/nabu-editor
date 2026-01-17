words = open("assets/unsorted_words.txt").read().split()
words.sort(key=len)
with open("assets/words.txt", "w") as f:
    f.write("\n".join(words))