import numpy as np
import matplotlib.pyplot as plt

stu = ['A', 'B', 'C', 'D']
sub = ['Kor', 'Math', 'Eng', 'Com']
data = [[75, 89, 92, 78], [90, 76, 88, 83], [55, 99, 81, 88], [80, 67, 75, 91]]

data = np.array(data)

#과목별 평균
s_avg = np.sum(data, axis = 0)
s_avg = s_avg/4

#학생별 평균
t_avg = np.sum(data, axis = 1)
t_avg = t_avg/4

plt.plot(sub, s_avg)
plt.title('average for subject')
plt.xlabel('subject')
plt.ylabel('point')
plt.show()

plt.plot(stu, t_avg)
plt.title('average for student')
plt.xlabel('student')
plt.ylabel('point')
plt.show()

