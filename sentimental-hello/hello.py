
# Get Input
name = input("What is your name: ")

# Making sure you can only put alphabetical characters
while name.isalpha() == False:

    # Get promted until user cooperates
    print("Error: not Alphabetical")
    name = input("What is your name: ")

# Print the results
print(f"Hello, {name}")