print('종료하려면 음수를 입력하시오')

score = []
sum = 0
while True:
    i = int(input('성적을 입력하시오: '))
    if i > 0:
        score.append(i)
    else:
        break
for i in score:
    sum += i
avg = sum/len(score)
print('평균은 %f입니다' %avg)
