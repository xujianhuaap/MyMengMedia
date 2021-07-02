cmake_minimum_required(VERSION 3.10.2)
#set(ANDROID_NDK /home/xu/Android/Sdk/ndk/21.1.6352462)
set(JNI_LIBS /home/xu/Documents/codes/codes_c/MyMengMedia/app/jnilibs)
set(FFMPEG_LIBS ${JNI_LIBS}/ffmpeg)

add_library(native-lib SHARED native-lib.cpp)
include_directories(include)
#NDK已经够建后的库,而且在cmake能够找到的路径可以直接使用
#findlibrary()找到的是一个变量(log-lib)
find_library(log-lib log)
#NDK中包含一些源码,可以直接包含到你的原生库中,将android_native_app_glue.c构建一个静态库,关联到native-lib这个库
#glue负责管理NativeActivity生命周期和触摸输入
#add_library(app-glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

#预构建的库
add_library(AvFormat-lib SHARED IMPORTED)
message("[info] --> ${FFMPEG_LIBS}/${CMAKE_ANDROID_ARCH_ABI}/libavformat.so")
set_target_properties(AvFormat-lib PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIBS}/${CMAKE_ANDROID_ARCH_ABI}/lib/libavformat.so)

add_library(avcodec-lib SHARED IMPORTED)
message("[info] --> ${FFMPEG_LIBS}/${CMAKE_ANDROID_ARCH_ABI}/libavcodec.so")
set_target_properties(avcodec-lib PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIBS}/${CMAKE_ANDROID_ARCH_ABI}/lib/libavcodec.so)


include_directories(${FFMPEG_LIBS}/${CMAKE_ANDROID_ARCH_ABI}/include)
target_link_libraries(native-lib  AvFormat-lib  avcodec-lib ${log-lib})
