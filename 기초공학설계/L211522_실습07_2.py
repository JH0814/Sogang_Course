L = [[1, 2, 3], [4, 5], [6], [7, 8]]
S = []
sum = 0
for i in L:
    for j in i:
        sum += j*j
    S.append(sum)
    sum = 0
print(S)
    