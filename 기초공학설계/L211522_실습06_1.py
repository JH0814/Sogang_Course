i = input('세 개의 정수를 입력하시오 : ')

a, b, c = i.split()
a = int(a)
b = int(b)
c = int(c)

if a < b:
    a, b = b, a
if b < c:
    b, c = c, b
if a < b:
    a, b = b, a

print('내림차순 정렬 :', a, b, c)
