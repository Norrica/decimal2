def insert_sequence(str1, str2, int):
    str1_split1 = str1[:int]
    str1_split2 = str1[int:]
    new_string = str1_split1 + str2 + str1_split2
    return new_string


#
div_2 = [1854727994, 751066592, 0, 1835008], [1854727993, 751066592, 0, 1835008]
div_8 = [447392427, 1737688055, 903501810, 524288], [2192222890, 173768805, 90350181, 458752]
div_9 = [447392427, 1737688055, 903501810, 2148007936], [2192222890, 173768805, 90350181, 2147942400]
div_10 = [447392427, 1737688055, 903501810, 2148007936], [2192222890, 173768805, 90350181, 2147942400]
div_11 = [447392427, 1737688055, 903501810, 524288], [2192222890, 173768805, 90350181, 458752]

a = div_8
for j in a:
    s = ""
    for i in list(reversed(j))[:1]:
        s += f"{i:032b}"
        s = insert_sequence(s, " ", 1)
        s = insert_sequence(s, " ", 33 - 16)
        s = insert_sequence(s, " ", 33 - 16 - 8)

    s += " "
    for i in list(reversed(j))[1:]:
        s += f"{i:032b}"
    print(j)
    print(s)
    print(int(s.split()[2], 2))
    print(int(s.split()[4], 2))
