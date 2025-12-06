words = open("unsorted_words.txt").read().split()
words.sort(key=len)
with open("by_length.txt", "w") as f:
    f.write("\n".join(words))