price = 3928
c_500 = price//500
price %= 500
c_100 = price//100
price %= 100
print('500원짜리 동전 :', c_500, '개')
print('100원짜리 동전 :', c_100, '개')
print('남은 금액 :', price, '원')
