#! /usr/bin/env python
#-*- codoing:utf8 -*-

import Image
import os

resourcePath = '../Resources/iphone/spritesheets'
spritesheetPath = resourcePath + os.sep + 'test.png'

stringByDeletingPathExtension = lambda s: s[:s.rfind('.')]
pathExtension = lambda s: s[s.rfind('.') + 1:]
direction4x4 = [
    'down',
    'left',
    'right',
    'up'
]

def crop(path, width=4, height=4):
    spritesheet = Image.open(spritesheetPath)
    spritesheetSize = spritesheet.size
    spriteWidth = spritesheetSize[0] / width
    spriteHeight = spritesheetSize[1] / height
    y = 0
    basepath = os.path.dirname(path)
    extension = pathExtension(path)

    for i in range(width):
        x = 0
        for j in range(height):
            spritePath = '%s/%s_%d.%s'%(basepath, direction4x4[i], j, extension)
            rect = (x, y, x + spriteWidth, y + spriteHeight)
            sprite = spritesheet.crop(rect)
            sprite.save(spritePath)
            x += spriteWidth
        y += spriteHeight

crop(spritesheetPath)
