#! /usr/bin/python

import subprocess
import os
import shutil

archs = ['armv7', 'armv7s', 'arm64', 'x86_64']

def exec_wait(command_line):
    p = subprocess.Popen(command_line, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    print(p.stdout.read().decode('utf-8'))
    p.wait()

def genStaticLib(release):
    action = ''
    sdk = ''
    if release:
        action = 'archive'
    command_template = 'xcodebuild -scheme EZGLSDK -arch {0} -derivedDataPath ./build/packages/build-{1} ONLY_ACTIVE_ARCH=NO VALID_ARCHS={2} -sdk {3} {4}'
    for arch in archs:
        if arch.startswith('arm'):
            sdk = 'iphoneos10.3'
        else:
            sdk = 'iphonesimulator10.3'
        print(str.format('Make Static Lib, Arch: {0} | Action: {1} Begin...', arch, action))
        command = str.format(command_template, arch, arch, arch, sdk ,action)
        print(command)
        output = exec_wait(command)
        print (str.format('Make Static Lib, Arch: {0} | Action: {1} Finish.', arch, action))
        print(output)

def collectLibs(release):
    subpaths = ['Build/Products/Debug-iphoneos/', 'Build/Products/Debug-iphonesimulator/']
    libs = []
    for arch in archs:
        for subpath in subpaths:
            lib_path = str.format('./build/packages/build-{0}/{1}', arch, subpath)
            if os.path.exists(lib_path):
                files = os.listdir(lib_path)
                for file in files:
                    if file.endswith('.a'):
                        libs.append(os.path.join(lib_path, file))
    return libs

def merge_libs(libs, output):
    if os.path.exists(output) == False:
        os.mkdir(output)
    components = libs[0].split('/')
    lib_name = components[len(components) - 1]
    command = str.format('lipo -create {0} -output {1}', ' '.join(libs), os.path.join(output, lib_name))
    print('Begin merge libs...')
    print(command)
    exec_wait(command)
    print('Merge complete.')

def copy_headers(src_dir, dst_dir):
    print(dst_dir)
    if os.path.exists(dst_dir) is False:
        os.mkdir(dst_dir)
    subpaths = os.listdir(src_dir)
    for subpath in subpaths:
        path = os.path.join(src_dir, subpath)
        dst_path = os.path.join(dst_dir, subpath)
        if os.path.isdir(path):
            copy_headers(path, dst_path)
        elif subpath.endswith('.h'):
            shutil.copy(path, dst_path)

def gen_framework(dst_dir, framework_name, lib_path, headers_dir):
    if os.path.exists(dst_dir) is False:
        os.mkdir(dst_dir)
    framework_path = os.path.join(dst_dir, framework_name + '.framework')
    print framework_path
    if os.path.exists(framework_path) == False:
        os.mkdir(framework_path)
    dir_to_be_mk = ['Versions', 'Versions/A', 'Versions/A/Headers', 'Versions/A/Resources']
    for dir in dir_to_be_mk:
        absolute_dir = os.path.join(framework_path, dir)
        if os.path.exists(absolute_dir) is False:
            os.mkdir(absolute_dir)

    dir_to_be_ln = {'A' : 'Versions/Current', 'Versions/Current/Resources' : 'Resources', 'Versions/Current/Headers' : 'Headers'}
    for dir in dir_to_be_ln:
        absolute_dst_dir = os.path.join(framework_path, dir_to_be_ln[dir])
        if os.path.exists(absolute_dst_dir) is False:
            command = str.format('ln -sfh "{0}" {1}', dir, absolute_dst_dir)
            print command
            exec_wait(command)
    # cp lib
    shutil.copy(lib_path, os.path.join(framework_path, framework_name))

    # cp headers
    copy_headers(headers_dir, os.path.join(framework_path, 'Versions/A/Headers'))

release = False
genStaticLib(release)
libs = collectLibs(release)
merge_libs(libs, './build/lib_universe')
print("Copy Headers Begin...")
copy_headers('../../../Classes/', './build/headers')
copy_headers('../../../libs/lua', './build/headers/lua')
copy_headers('../../../libs/wavefront', './build/headers/wavefront')
copy_headers('../../../libs/bullet', './build/headers/')
print("Copy Headers End...")
gen_framework('./build', 'EZGLib', './build/lib_universe/libEZGLSDK.a', './build/headers')
