# Step 1: Read the name of a text file from the console.
filename = input("Enter the name of the text file: ")

word_counts = {}

# Step 2: Read in the text file, not all at once.
with open(filename, 'r') as file:
    for line in file:  # Reading line by line
        # Convert each line to remove punctuation but retain letters. For instance, "it's" becomes "its".
        words = [''.join(char for char in word if char.isalpha()).lower() for word in line.split()]
        
        # Count each word's occurrence.
        for word in words:
            word_counts[word] = word_counts.get(word, 0) + 1

# Sort words based on the count in descending order. For same count, sort alphabetically.
sorted_words = sorted(word_counts.items(), key=lambda x: (-x[1], x[0]))

# Step 7: Output the first ten words in the sorted list, along with their counts.
print("Top 10 words:")
for word, count in sorted_words[:10]:
    print(f"{word}: {count}")

# Step 8: Output the last ten words in the sorted list, along with their counts.
print("\nBottom 10 words:")
for word, count in sorted_words[-10:]:
    print(f"{word}: {count}")

# Step 9: Output all ‘unique words’ (the word count is 1).
print("\nUnique words:")
for word, count in sorted_words:
    if count == 1:
        print(word)
