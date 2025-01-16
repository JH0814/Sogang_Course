a = input('계좌번호를 입력하시오: ')
letter = ''
for i in a:
    if i.isalnum():
        letter += i
print(letter)