sample = 'abcABCdEFaBCDeFAbC'
sample = sample.lower()
f1 = 'abc'
f2 = 'def'
a = sample.find(f1)
a1 = sample.count(f1)
b = sample.find(f2)
b1 = sample.count(f2)
print('%s 문자열 : %d 인덱스, %d 번 존재' %(f1, a, a1))
print('%s 문자열 : %d 인덱스, %d 번 존재' %(f2, b, b1))

