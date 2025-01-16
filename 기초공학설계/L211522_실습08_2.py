D = {'c':7, 'f':3, 'a':5}
k = list(D.items())
if k[0][0] > k[1][0]:
    k[0], k[1] = k[1], k[0]
if k[1][0] > k[2][0]:
    k[1], k[2] = k[2], k[1]
if k[0][0] > k[1][0]:
    k[0], k[1] = k[1], k[0]
print('After sorted :', k)