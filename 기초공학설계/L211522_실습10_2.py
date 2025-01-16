def chk():
    for i in range(2, N):
        if N % i == 0:
            return False
    return True

        
N = int(input())
primeChk = chk()
if primeChk == True:
    print('prime')
else:
    print('not prime')