i = input('수식 입력(예: 20 * 40) : ')
num1, r, num2 = i.split()
num1 = float(num1)
num2 = float(num2)

if (r == '+'):
    print('%.6f %s %.6f = %.6f' %(num1, r, num2, num1+num2))
elif(r == '-'):
    print('%.6f %s %.6f = %.6f' %(num1, r, num2, num1-num2))
elif(r == '*'):
    print('%.6f %s %.6f = %.6f' %(num1, r, num2, num1*num2))
elif(r == '/'):
    if num2 == 0:
        print('%.6f 로 나누기를 수행할 수 없습니다.' %num2)
    else:
        print('%.6f %s %.6f = %.6f' %(num1, r, num2, num1/num2))
else:
    print(r, '지원하지 않는 연산자입니다.')