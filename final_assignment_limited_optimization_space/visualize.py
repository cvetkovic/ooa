from dataclasses import dataclass
from PIL import Image, ImageDraw
from random import randint


@dataclass
class Rectangle:
    pointX: int
    pointY: int
    width: int
    height: int


path = '/mnt/c/Users/jugos000/CLionProjects/ooa/final_assignment_2/fa_layout.txt'
file = open(path, "r")

canvasWidth = int(file.readline())
canvasHeight = int(file.readline())
numberOfRectangles = int(file.readline())

rectangles = []
for i in range(0, numberOfRectangles):
    line = file.readline().split(' ')
    rectangles.append(Rectangle(int(line[0]), int(line[1]), int(line[2]), int(line[3])))

file.close()

image = Image.new('RGB', (canvasWidth, canvasHeight))
draw = ImageDraw.Draw(image)

# No seed intentionally

for r in rectangles:
    bottomLeftX = r.pointX;
    bottomLeftY = canvasHeight - r.pointY;

    upperLeftX = bottomLeftX
    upperLeftY = bottomLeftY - r.height
    lowerRightX = bottomLeftX + r.width
    lowerRightY = bottomLeftY

    r = randint(40, 255)
    g = randint(40, 255)
    b = randint(40, 255)

    for i in range(upperLeftX, lowerRightX):
        for j in range(upperLeftY, lowerRightY):
            draw.point((i, j), fill=(r, g, b))

image.save('/mnt/c/Users/jugos000/Desktop/test.png', quality=100)
