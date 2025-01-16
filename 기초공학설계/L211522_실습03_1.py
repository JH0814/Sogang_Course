price = 0
ame_price = 2000
caf_price = 3000
cap_price = 3500
ame = 10
caf = 20
cap = 30
cap_dc = 0.1
print('아메리카노 판매 개수?', ame)
print('카페라떼 판매 개수?', caf)
print('카푸치노 판매 개수?', cap)
print('카푸치노 할인률?', cap_dc)
price += ame * ame_price
price += caf * caf_price
price += cap * cap_price * (1-cap_dc)
print('총 매출은', int(price), '원 입니다')
