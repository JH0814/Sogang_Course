from turtle import *
shape('turtle')

color('red', 'yellow')
begin_fill()
for i in range(3):
    fd(100)
    lt(360/3)
end_fill()

fillcolor('green')
begin_fill()
for i in range(4):
    fd(100)
    rt(360/4)
end_fill()

