
def main():
    # Getting input from user
    height = input("Height: ")

    # promt user again if input isnt numeric
    while height.isnumeric() == False:
        height = input("Height: ")

    # change input to integer
    height = int(height)

    # Input needs to be inside these parameters
    while height < 1 or height > 8:
        height = int(input("Height: "))

    # Start this function
    block(height)




def block(height):
    # Print every line of height
    for i in range(height):
        print(" "*(height-i-1)+"#"*(i+1)+"  "+"#"*(i+1))


main()