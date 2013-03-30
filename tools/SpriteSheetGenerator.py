#! /usr/bin/env python
#-*- coding:utf8 -*-
'''
Created on 2013-2-24

@author: DF
'''

import os
import sys
import shutil
import Image
import ImageChops
import plistlib

kMCSpriteSheetsDirectory = '../Resources/iphone/spritesheets'

stringByDeletingPathExtension = lambda s: s[:s.rfind('.')]
pathExtension = lambda s: s[s.rfind('.') + 1:]

direction4x4 = [
    'down',
    'left',
    'right',
    'up'
]

def trim(image):
    bg = Image.new(image.mode, image.size, image.getpixel((0,0))) 
    diff = ImageChops.difference(image, bg)
    bbox = diff.getbbox()
    if bbox:
        return image.crop(bbox)
    
def toTransparent(image):
    image = image.convert('RGBA')
    transpanret = (0, 0, 0, 0)
    alpha = image.getpixel((0, 0))
    for x in range(image.size[0]):
        for y in range(image.size[1]):
            if image.getpixel((x, y)) == alpha:
                image.putpixel((x, y), transpanret)
    return image

def generate(path, width=4, height=4, padding=0):
    spritesheet = Image.open(path)
    spritesheetTextureSavePath = stringByDeletingPathExtension(path)
    spritesheetSize = spritesheet.size
    spriteWidth = spritesheetSize[0] / width
    spriteHeight = spritesheetSize[1] / height
    y = 0
    plist = plistlib.Plist()
    frames = plistlib.Dict()
    metadata = plistlib.Dict()

    basepath = os.path.dirname(spritesheetTextureSavePath)
    basename = os.path.basename(spritesheetTextureSavePath) #已经无后缀名了
    metadata.textureFileName = basename

    trimmed = [[] for _ in range(height)]
    spritesheet = toTransparent(spritesheet) #透明化

    for i in range(width):
        x = 0
        for j in range(height):
            rect = (x, y, x + spriteWidth, y + spriteHeight)
            sprite = spritesheet.crop(rect)
            sprite = trim(sprite)
            trimmed[i].append(sprite)

            x += spriteWidth
        y += spriteHeight

    plist.metadata = metadata

    #获取新图像的宽高
    sizes = []
    for i in range(width):
        newWidth = 0
        newHeight = 0
        for j in range(height):
            sprite = trimmed[i][j]
            newWidth += sprite.size[0]
            if sprite.size[1] > newHeight:
                newHeight = sprite.size[1]
        sizes.append((newWidth, newHeight))
    
    maxTrimmedWidth = 0
    maxTrimmedHeight = 0
    for size in sizes:
        if size[0] > maxTrimmedWidth:
            maxTrimmedWidth = size[0]
        maxTrimmedHeight += size[1]
    
    #新图像画布 
    canvas = Image.new('RGBA', (maxTrimmedWidth + padding * 3, maxTrimmedHeight + padding * 3))
    
    #重组图像
    y = 0
    for i in range(width):
        x = 0
        for j in range(height):
            sprite = trimmed[i][j]
            canvas.paste(sprite, (x, y))

            key = '%s-%s-%d'%(stringByDeletingPathExtension(basename), direction4x4[i], j)
            #frame info
            frame = plistlib.Dict()
            frame.x = x
            frame.y = y
            frame.width = sprite.size[0]
            frame.height = sprite.size[1]
            frame.offsetX = 0
            frame.offsetY = 0
            frame.originalWidth = sprite.size[0]
            frame.originalHeight = sprite.size[1]

            frames[key] = frame

            x += sprite.size[0] + padding
            
        y += sizes[i][1] + padding
    plist.frames = frames

    savePListPath = stringByDeletingPathExtension(spritesheetTextureSavePath) + '.plist'

    plistlib.writePlist(plist, savePListPath)
    temppath = spritesheetTextureSavePath + '.png'
    canvas.save(temppath)
    os.renames(temppath, spritesheetTextureSavePath)
    return spritesheetTextureSavePath

class REGenerator(object):
    def __init__(self, workspace=None):
        self.workspace = workspace
        self.prebuild = []

    def build(self):
        build = self.workspace + os.sep + 'build'
        if os.path.exists(build):
            old = build + '.old'
            if os.path.exists(old):
                os.system('rm -r %s'%old)
            os.rename(build, old)
        for filepath in self.prebuild:
            extension = pathExtension(filepath)
            print 'generate %s'%filepath
            outputTextureFilepath = generate(filepath, padding=2)
            outputBasepath = os.path.dirname(outputTextureFilepath)
            outputBasename = os.path.basename(outputTextureFilepath)
            buildTextureFilepath = build + os.sep + outputBasename
            buildPListFilepath = build + os.sep + stringByDeletingPathExtension(outputBasename) + '.plist'
            os.renames(outputTextureFilepath, buildTextureFilepath)
            os.renames(outputBasepath + os.sep + stringByDeletingPathExtension(outputBasename) + '.plist', buildPListFilepath)
            os.remove(filepath)


    def install(self):
        build = self.workspace + os.sep + 'build'
        if os.path.exists(build):
            files = os.listdir(build)
            targetDirectory = kMCSpriteSheetsDirectory + os.sep
            for f in files:
                filepath = build + os.sep + f
                newFilepath = targetDirectory + os.sep + f
                print 'copy %s to %s'%(filepath, newFilepath)
                shutil.copy2(filepath, newFilepath)
        
    def parse(self):
        if self.workspace is None:
            return
        build = self.workspace + os.sep + 'prebuild'
        if os.path.exists(build):
            old = build + '.old'
            if os.path.exists(old):
                os.system('rm -r %s'%old)
            os.rename(build, old)
        self.prebuild = []
        directories = os.listdir(self.workspace)
        for d in directories:
            path = self.workspace + os.sep + d
            if os.path.isdir(path):
                files = os.listdir(path)
                for f in files:
                    filepath = path + os.sep + f
                    if os.path.isdir(filepath):
                        dirpath = filepath
                        files = os.listdir(dirpath)
                        for f in files:
                            extension = pathExtension(f)
                            filepath = dirpath + os.sep + f
                            if os.path.isfile(filepath) and extension == 'png':
                                #at first,copy
                                #newFilepath = "%s%s%s%s%s.st"%(build, os.sep, d, os.sep, stringByDeletingPathExtension(f))
                                newFilepath = "%s%s%s%s%s-%s.st"%(build,
                                                                  os.sep,
                                                                  d,
                                                                  os.sep,
                                                                  f[0].lower(),
                                                                  stringByDeletingPathExtension(os.path.basename(f))[1:])
                                print newFilepath
                                basepath = os.path.dirname(newFilepath)
                                if os.path.exists(basepath) is False:
                                    os.makedirs(basepath)
                                newFilepath = newFilepath + '.pre'
                                shutil.copy2(filepath, newFilepath)
                                self.prebuild.append(newFilepath)
                    else:
                        extension = pathExtension(f)
                        filepath = path + os.sep + f
                        if os.path.isfile(filepath) and extension == 'png':
                            #at first,copy
                            #newFilepath = "%s%s%s%s%s.st"%(build, os.sep, d, os.sep, stringByDeletingPathExtension(f))
                            newFilepath = "%s%s%s%s%s-%s.st"%(build,
                                                              os.sep,
                                                              d,
                                                              os.sep,
                                                              f[0].lower(),
                                                              stringByDeletingPathExtension(os.path.basename(f))[1:])
                            print newFilepath
                            basepath = os.path.dirname(newFilepath)
                            if os.path.exists(basepath) is False:
                                os.makedirs(basepath)
                            newFilepath = newFilepath + '.pre'
                            shutil.copy2(filepath, newFilepath)
                            self.prebuild.append(newFilepath)

if __name__ == '__main__':
    path = '../../raw-resources'
    g = REGenerator(path)
    g.parse()
    g.build()
    g.install()
