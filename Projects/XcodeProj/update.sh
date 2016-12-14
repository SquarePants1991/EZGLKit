#!/bin/bash
cp -r ../../Classes/  EZGLKit/Classes/CppClasses

cd EZGLKit/Classes/CppClasses
for i in `ls *.cpp`; do
  newFileName=''
  echo $i | sed "s/cpp/mm/g" | $newFileName
  echo 'rename '$i' to'$newFileName
done

#cd ../../../Example
#pod install
