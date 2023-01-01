import random
import string

def random_char(y):
    l = ['A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z']
    result = ""
    counter = 0

    while counter < y:
        result += l[random.randint(0,25)]
        counter += 1

    return result
    

texts = []
strings = []

def write_output(n):
    for i in range(n):
        text = (random_char(random.randint(6,25)))
        string = (random_char(random.randint(3,6)))
        texts.append(text)
        strings.append(string)

    filetext = "T"+str(n)+".txt"
    filestring = "S"+str(n)+".txt"


    f = open(filetext, "w")
    f2 = open(filestring, "w")

    for i in range(len(texts)):
        f.write(texts[i]+"\n")
        f2.write(strings[i]+"\n")


write_output(4)
write_output(16)
write_output(64)
write_output(1024)

