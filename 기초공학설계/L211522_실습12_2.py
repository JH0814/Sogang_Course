import pandas_datareader.data as web
import datetime
import matplotlib.pyplot as plt
import pandas as pd

start = datetime.datetime(2021, 4, 1)
end = datetime.datetime(2021, 5, 21)

a = web.DataReader('000020.KS', 'yahoo', start, end)
b = web.DataReader('000040.KS', 'yahoo', start, end)
c = web.DataReader('000060.KS', 'yahoo', start, end)
d = web.DataReader('000070.KS', 'yahoo', start, end)
e = web.DataReader('000080.KS', 'yahoo', start, end)

plt.title('Close cost for 5 events')
plt.plot(a['Close'])
plt.plot(b['Close'])
plt.plot(c['Close'])
plt.plot(d['Close'])
plt.plot(e['Close'])
plt.xlabel('date')
plt.ylabel('Close')
plt.show()

a_4mean = round(web.DataReader('000020.KS', 'yahoo', datetime.datetime(2021, 4, 1), datetime.datetime(2021, 4, 30))['Volume'].mean(), 1)
a_5mean = round(web.DataReader('000020.KS', 'yahoo', datetime.datetime(2021, 5, 1), datetime.datetime(2021, 5, 21))['Volume'].mean(), 1)

a['mon'] = a.Date.str.split('-').str[1]

a_mean = round(a['Volume'].mean(), 1)

b_4mean = round(web.DataReader('000040.KS', 'yahoo', datetime.datetime(2021, 4, 1), datetime.datetime(2021, 4, 30))['Volume'].mean(), 1)
b_5mean = round(web.DataReader('000040.KS', 'yahoo', datetime.datetime(2021, 5, 1), datetime.datetime(2021, 5, 21))['Volume'].mean(), 1)
b_mean = round(b['Volume'].mean(), 1)

c_4mean = round(web.DataReader('000060.KS', 'yahoo', datetime.datetime(2021, 4, 1), datetime.datetime(2021, 4, 30))['Volume'].mean(), 1)
c_5mean = round(web.DataReader('000060.KS', 'yahoo', datetime.datetime(2021, 5, 1), datetime.datetime(2021, 5, 21))['Volume'].mean(), 1)
c_mean = round(c['Volume'].mean(), 1)

d_4mean = round(web.DataReader('000070.KS', 'yahoo', datetime.datetime(2021, 4, 1), datetime.datetime(2021, 4, 30))['Volume'].mean(), 1)
d_5mean = round(web.DataReader('000070.KS', 'yahoo', datetime.datetime(2021, 5, 1), datetime.datetime(2021, 5, 21))['Volume'].mean(), 1)
d_mean = round(d['Volume'].mean(), 1)

e_4mean = round(web.DataReader('000080.KS', 'yahoo', datetime.datetime(2021, 4, 1), datetime.datetime(2021, 4, 30))['Volume'].mean(), 1)
e_5mean = round(web.DataReader('000080.KS', 'yahoo', datetime.datetime(2021, 5, 1), datetime.datetime(2021, 5, 21))['Volume'].mean(), 1)
e_mean = round(e['Volume'].mean(), 1)

data4 = [a_4mean, b_4mean, c_4mean, d_4mean, e_4mean]
data5 = [a_5mean, b_5mean, c_5mean, d_5mean, e_5mean]
data = [a_mean, b_mean, c_mean, d_mean, e_mean]
low = ['A', 'B', 'C', 'D', 'E']
dna = {'종목/월':low, '4월':data4, '5월':data5, '평균':data}
dfM = pd.DataFrame(data=dna)

print(dfM)