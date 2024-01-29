import pyperclip

cpparray = "{"

with open('_targets/out.bbx', 'r',) as file:
    for line in file:
        line = line.strip()
        cpparray += str(int(line)) + ", "

text = str(cpparray) + "}"

pyperclip.copy(text)
