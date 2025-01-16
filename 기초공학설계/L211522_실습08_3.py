fruit = {'배':[2, 1000], '자몽':[1,2000], '메론':[1,8000], '감':[6,800]}

a = input('먹고 싶은 과일은? : ')

re = fruit.keys()
if a in re:
    if fruit[a][0] == 0:
        print(a, '준비되어 있지 않습니다')
    else:
        print(a, '맛있게 드세요')
        fruit[a][0] -= 1
else:
    print(a, '준비되어 있지 않습니다')

print()
print('각 과일 당 최소 5개는 되도록 구입합니다')
money = 0
for i in re:
    if fruit[i][0] > 5:
        continue
    money = money + (5-fruit[i][0])*fruit[i][1]
print('구입에 필요한 총 금액은 :', money, '원 입니다')