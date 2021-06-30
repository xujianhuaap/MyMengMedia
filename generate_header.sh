#!/bin/bash
echo "input class qualified name"
read input
ProjectFileDir=$(pwd)
echo "${ProjectFileDir}/app/build/tmp/kotlin-classes/debug"
/usr/java/jdk1.8.0_251/bin/javah -cp ${ProjectFileDir}/app/build/tmp/kotlin-classes/debug -jni \
  -d ${ProjectFileDir}/app/src/main/cpp/include ${input}
