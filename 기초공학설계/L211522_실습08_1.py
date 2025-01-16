employee = {'공유', '고수', '보검', '태현', '종민', '세윤', '준호', '우종', '시우', '두준'}
late = {'우종', '보검'}
absent = {'종민', '우종', '보검', '두준'}
bonus = set()
for i in employee:
    if i not in late and i not in absent:
        bonus.add(i)
ya = late.intersection(absent)
print('전체 사원 명단 :', employee)
print('지각자 명단 :', late)
print('결근자 명단 :', absent)
print('보너스 지급 대상자 명단 :', bonus)
print('야근 대상자 명단 :', ya)
new = input('신입사원 명단 입력 : ').split()
check = 0
for i in new:
    if i in employee:
        check = 1
    employee.add(i)
if check == 1:
    print('신입사원의 이름이 기존 사원의 이름과 중복')
print('전체 사원 명단 :', employee)