while(True):
    height = int(input("Height: "))
    if (height < 1 or height > 8):
        height = int(input("Height: "))
    else:
        break

for i in range(height):
    print(" " * (height - i - 1) + "#" * (i + 1), end=" ") # Left pyramid
    print("#" * (i + 1)) # Right pyramid
