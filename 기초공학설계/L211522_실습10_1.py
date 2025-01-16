n = int(input('자연수 n을 입력하세요 :'))

def checkOddEven(n):
    if n%2 == 0:
        print('짝수')
    else:
        print('홀수')

checkOddEven(n)