i = input('두 자리 정수 두개를 입력 : ')
num1, num2 = i.split()
if num1 == num2:
    print('두 정수는 모두', num1, '로 같은 정수입니다.')
elif num1[0] == num2[1] and num1[1] == num2[0]:
    print(num1, ',', num2, ': 자리 값만 다른 정수입니다.')
elif num1[0] in num2 and num1[1] not in num2 or num1[1] in num2 and num1[0] not in num2:
    print(num1, ',', num2, ': 하나의 숫자만 일치합니다.')
else:
    print(num1, ',', num2, ': 일치하지 않는 정수입니다.')