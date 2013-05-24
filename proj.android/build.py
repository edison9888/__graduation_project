#! /usr/bin/env python
#-*- coding:utf8 -*-

import os
import sys

kMCCommandOptionDebug = '--debug'
kMCCommandOptionRelease = '--release'
kMCCommandOptionClean = '--clean'

MCCommandOptions = [
    kMCCommandOptionDebug,
    kMCCommandOptionRelease,
    kMCCommandOptionClean
]

kMCProjectName = 'Military Confrontation'
kMCKeystoreFilepath = '/Users/DF/Softwares/adt-bundle-mac-x86_64/eclipse/key/default'

def main():
    cmd = None
    sign_package = True
    for arg in sys.argv[1:]:
        if arg in MCCommandOptions:
            cmd = 'ant ' + arg[2:]
            output = 'bin/%s-%s.apk'%(kMCProjectName, arg[2:])
            if arg == kMCCommandOptionClean:
                sign_package = False
    print cmd
    return
    if len(sys.argv) < 2:
        print './build.py [--debug|--release|--clean] [--re-generate] [--re-build]'
        sys.exit(0)
    elif cmd is None:
        print '不支持的命令！'
        sys.exit(1)

    re_generate = '--re-generate' in sys.argv
    if re_generate:
        os.system('cd jni;./gen_Android.mk.py;cd ..')
    re_build = '--re-build' in sys.argv
    if re_build:
        os.system('./build_native.sh')

    print 'run ' + cmd
    os.system(cmd)

    if sign_package:
        print 'will sign package'
        os.system('jarsigner -verbose -sigalg MD5withRSA -digestalg SHA1 -keystore %s -storepass 123456 -keypass 123456 %s df'%(kMCKeystoreFilepath, output))
    
    sys.exit(0)

if __name__ == '__main__':
    main()
