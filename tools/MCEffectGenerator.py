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

kMCEffectsDirectory = '../Resources/iphone/effects'

stringByDeletingPathExtension = lambda s: s[:s.rfind('.')]
pathExtension = lambda s: s[s.rfind('.') + 1:]
    
class MCEffect:
    MCEffectFrame = None
    
    def __init__(self, name):
        self.name = name
        self.frames = []
        self.plist = None
        self.texture = None
        
    def __repr__(self):
        return '<MCEffect :name=%s :frames=%d>'%(self.name, len(self.frames))
        
    def addFrame(self, filepath):
        if MCEffect.MCEffectFrame is None:
            class MCEffectFrame:
                def __init__(self, frameName, filepath):
                    self.frameName = frameName
                    self.filepath = filepath
        
                def __repr__(self):
                    return "<MCEffectFrame :frameName=%s :filepath=%s>"%(self.frameName, self.filepath)
                    
            MCEffect.MCEffectFrame = MCEffectFrame
        basename = os.path.basename(filepath)
        frameName = stringByDeletingPathExtension(basename)
        frame = MCEffect.MCEffectFrame(frameName, filepath)
        self.frames.append(frame)
        
    def generate(self):
        effectFrames = []
        for frame in self.frames:
            effectFrame = Image.open(frame.filepath)
            effectFrame.frameName = frame.frameName
            effectFrames.append(effectFrame)
        firstEffectFrame = effectFrames[0]
        frameSize =firstEffectFrame.size
        
        frameWidth = frameSize[0]
        frameHeight = frameSize[1]
        framesLength = len(effectFrames)
        
        nw = 4096 / frameWidth #一行几个
        nh = framesLength / nw + (0 if (framesLength % nw) == 0 else 1)
        canvasSize = (frameWidth * nw, frameHeight * nh)
        
        # new a texture canvas
        canvas = Image.new('RGBA', canvasSize)
        
        plist = plistlib.Plist()
        frames = plistlib.Dict()
        
        x = 0
        y = 0
        c = 0
        for effectFrame in effectFrames:
            canvas.paste(effectFrame, (x, y))
            
            #frame info
            key = '%s-%d'%(self.name, c)
            frame = plistlib.Dict()
            frame.x = x
            frame.y = y
            frame.width = effectFrame.size[0]
            frame.height = effectFrame.size[1]
            frames[key] = frame
            frame.originalWidth = effectFrame.size[0]
            frame.originalHeight = effectFrame.size[1]

            x += effectFrame.size[0]
            c += 1
            if c == nw:
                x = 0
                y += frameHeight
        plist.frames = frames
        
        metadata = plistlib.Dict()
        metadata.textureFileName = self.name + '.et'
        plist.metadata = metadata
        
        self.plist = plist
        self.texture = canvas
        
    def writeTo(self, outputPath):
        if outputPath.endswith(os.sep) is False:
            outputPath = outputPath + os.sep

        plistFilepath = outputPath + self.name + '.plist'
        textureFilepath = outputPath + self.name + '.et'

        plistlib.writePlist(self.plist, plistFilepath)
        
        temppath = textureFilepath + '.png'
        self.texture.save(temppath)
        os.renames(temppath, textureFilepath)

class REGenerator(object):
    def __init__(self, workspace=None):
        self.workspace = workspace
        self.prebuild = []

    def build(self):
       for effect in self.prebuild:
           effect.generate()

    def install(self):
       for effect in self.prebuild:
           print effect.name
           effect.writeTo(kMCEffectsDirectory)
        
    def parse(self):
        if self.workspace is None:
            return
        effect_directories = os.listdir(self.workspace)
        for effect_directory in effect_directories:
            effect_directory_path = self.workspace + os.sep + effect_directory
            if not os.path.isdir(effect_directory_path):
                continue
            effect_frame_files = os.listdir(effect_directory_path)
            effect = MCEffect(effect_directory)
            for effect_frame_file in effect_frame_files:
                if effect_frame_file.endswith('.png') is False:
                    continue
                effect_frame_file_fullpath = effect_directory_path + os.sep + effect_frame_file
                effect.addFrame(effect_frame_file_fullpath)
            self.prebuild.append(effect)

if __name__ == '__main__':
    path = '../../raw-resources/Effects'
    g = REGenerator(path)
    g.parse()
    g.build() 
    g.install()
