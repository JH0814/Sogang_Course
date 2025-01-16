sum = 0
while True:
    N = int(input('양의 정수를 입력하세요: '))
    if N > 0:
        for i in range(1, N+1):
            sum += i
        break
    else:
        print('양수를 입력하세요')
print(sum)