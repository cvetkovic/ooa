from dataclasses import dataclass
from PIL import Image, ImageDraw
from random import randint


@dataclass
class Rectangle:
    pointX: int
    pointY: int
    width: int
    height: int


path = '/mnt/c/Users/jugos000/CLionProjects/ooa/cmake-build-debug/fa_layout.txt'
file = open(path, "r")

canvasWidth = int(file.readline())
canvasHeight = int(file.readline())

rectangles = []
for i in range(0, 23):
    line = file.readline().split(' ')
    rectangles.append(Rectangle(int(line[0]), int(line[1]), int(line[2]), int(line[3])))

file.close()

image = Image.new('RGB', (canvasWidth, canvasHeight))
draw = ImageDraw.Draw(image)

# No seed intentionally

for r in rectangles:
    upperLeftX = r.pointX
    upperLeftY = r.pointY
    lowerRightX = upperLeftX + r.width
    lowerRightY = upperLeftY + r.height

    r = randint(40, 255)
    g = randint(40, 255)
    b = randint(40, 255)

    for i in range(upperLeftX, lowerRightX):
        for j in range(upperLeftY, lowerRightY):
            draw.point((i, j), fill=(r, g, b))

image.save('/mnt/c/Users/jugos000/Desktop/test.png', quality=95)
