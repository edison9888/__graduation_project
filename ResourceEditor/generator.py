#! /usr/bin/env python
#-*- coding:utf8 -*-

import os
import Image
import plistlib

resourcePath = '../Resources/iphone/spritesheets'

stringByDeletingPathExtension = lambda s: s[:s.rfind('.')]
pathExtension = lambda s: s[s.rfind('.') + 1:]

direction4x4 = [
    'down',
    'left',
    'right',
    'up'
]

def crop(path, width=4, height=4):
    spritesheet = Image.open(path)
    spritesheetSize = spritesheet.size
    spriteWidth = spritesheetSize[0] / width
    spriteHeight = spritesheetSize[1] / height
    y = 0
    plist = plistlib.Plist()
    texture = plistlib.Dict()
    frames = plistlib.Dict()
    texture.width = spritesheetSize[0]
    texture.height = spritesheetSize[1]
    plist.texture = texture
    
    basepath = os.path.dirname(path)
    basename = stringByDeletingPathExtension(os.path.basename(path))
    extension = pathExtension(path)

    for i in range(width):
        x = 0
        for j in range(height):
            rect = (x, y, x + spriteWidth, y + spriteHeight)
            sprite = spritesheet.crop(rect)

            key = '%s-%s-%d'%(basename, direction4x4[i], j)
            #frame info
            frame = plistlib.Dict()
            frame.x = x
            frame.y = y
            frame.width = spriteWidth
            frame.height = spriteHeight
            frame.offsetX = 0
            frame.offsetY = 0
            frame.originalWidth = spriteWidth
            frame.originalHeight = spriteHeight

            frames[key] = frame
            
            x += spriteWidth
        y += spriteHeight
    plist.frames = frames

    savebasepath = basepath + os.sep + basename + '.'
    savePListPath = savebasepath + 'plist'

    plistlib.writePlist(plist, savePListPath)

def doCrop(path):
    files = os.listdir(path)
    for f in files:
        if f.endswith('.png'):
            fullpath = path + os.sep + f
            crop(fullpath)
        
doCrop(resourcePath)
