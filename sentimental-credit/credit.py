

credit_number = input("Input your credit-card number: ")

while credit_number.isnumeric() == False:
    credit_number = input("Input your credit-card number: ")

if len(credit_number) == 15 and credit_number.startswith(("34","37")):
    sum = 0
    card = "AMEX"
    round = False
    for i in range(int(credit_number),-1):
        if round:
            digit = credit_number[i-1]*2
            if digit > 9:
                digit = str(digit)
                sum += int(digit[0]) + int(digit[1])
            else:
                sum += int(digit)
            round = False
        else:
            digit = int(credit_number[i-1])
            sum += digit
            round = True

elif len(credit_number) == 16 and credit_number.startswith(("51","52","53","54","55")):
    sum = 0
    card = "MASTERCARD"
    round = False
    for i in range(int(credit_number),-1):
        if round:
            digit = credit_number[i-1]*2
            if digit > 9:
                digit = str(digit)
                sum += int(digit[0]) + int(digit[1])
            else:
                sum += int(digit)
            round = False
        else:
            digit = int(credit_number[i-1])
            sum += digit
            round = True

elif (len(credit_number) == 16 or len(credit_number) == 13) and credit_number.startswith("4"):
    sum = 0
    card = "VISA"
    round = False
    for i in range(int(credit_number),-1):
        if round:
            digit = credit_number[i-1]*2
            if digit > 9:
                digit = str(digit)
                sum += int(digit[0]) + int(digit[1])
            else:
                sum += int(digit)
            round = False
        else:
            digit = int(credit_number[i-1])
            sum += digit
            round = True

else:
    sum = 1

if (sum % 10 == 0):
    print(card)
else:
    print("INVALID")






