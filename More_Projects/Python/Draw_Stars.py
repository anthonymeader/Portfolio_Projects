#Python program that draws two starts and fills them in based off the color desired by the user
import turtle
star = turtle.Turtle()

user_color = input("What Color Would You Like for Star? ")
star.color('Black' , user_color)
star.begin_fill()



star.up()               #This and the next to lines are for position
star.goto(-200,100)
star.down()
star.stamp()           # if you have an image this leaves a stamp on the page

for number in range(5):
    star.forward(100)
    star.left(144) #144 ((360/5) x 2)

star.end_fill()

#Start Of Second Star

startwo = turtle.Turtle()

user_color = input("What Color Would You Like for StarTwo? ")
startwo.color('Black' , user_color)

startwo.begin_fill()

startwo.up()
startwo.goto(200,100)
startwo.down()
startwo.stamp()

for number in range(5):
    startwo.forward(100)
    startwo.left(144)

startwo.end_fill()






    
