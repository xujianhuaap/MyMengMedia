cmake_minimum_required(VERSION 3.10.2)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
set(JNI_LIBS C:/Users/10910/AndroidStudioProjects/MyMengMedia/app/jnilibs)
set(FFMPEG_LIBS ${JNI_LIBS}/ffmpeg)



set(android-libs
        android
        EGL
        GLESv3
        OpenSLES
        log
        m
        z
        )


file( GLOB_RECURSE src_files
        ${CMAKE_SOURCE_DIR}/*.cpp
        ${CMAKE_SOURCE_DIR}/utils/*.cpp
        ${CMAKE_SOURCE_DIR}/recorder/*.cpp
        ${CMAKE_SOURCE_DIR}/player/*.cpp
        ${CMAKE_SOURCE_DIR}/player/decoder/*.cpp
        ${CMAKE_SOURCE_DIR}/player/render/audio/*.cpp)
add_library(native-lib SHARED ${src_files})



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

add_library(avdevice-lib SHARED IMPORTED)
message("[info] --> ${FFMPEG_LIBS}/${CMAKE_ANDROID_ARCH_ABI}/libavdevice.so")
set_target_properties(avdevice-lib PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIBS}/${CMAKE_ANDROID_ARCH_ABI}/lib/libavdevice.so)


add_library(avfilter-lib SHARED IMPORTED)
message("[info] --> ${FFMPEG_LIBS}/${CMAKE_ANDROID_ARCH_ABI}/libavfilter.so")
set_target_properties(avfilter-lib PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIBS}/${CMAKE_ANDROID_ARCH_ABI}/lib/libavfilter.so)

add_library(avutil-lib SHARED IMPORTED)
message("[info] --> ${FFMPEG_LIBS}/${CMAKE_ANDROID_ARCH_ABI}/libavutil.so")
set_target_properties(avutil-lib PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIBS}/${CMAKE_ANDROID_ARCH_ABI}/lib/libavutil.so)

add_library(swresample-lib SHARED IMPORTED)
message("[info] --> ${FFMPEG_LIBS}/${CMAKE_ANDROID_ARCH_ABI}/libswresample.so")
set_target_properties(swresample-lib PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIBS}/${CMAKE_ANDROID_ARCH_ABI}/lib/libswresample.so)

add_library(swscale-lib SHARED IMPORTED)
message("[info] --> ${FFMPEG_LIBS}/${CMAKE_ANDROID_ARCH_ABI}/libswscale.so")
set_target_properties(swscale-lib PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIBS}/${CMAKE_ANDROID_ARCH_ABI}/lib/libswscale.so)


add_library(device-lib SHARED IMPORTED)
message("[info] --> ${FFMPEG_LIBS}/${CMAKE_ANDROID_ARCH_ABI}/libavdevice.so")
set_target_properties(device-lib PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIBS}/${CMAKE_ANDROID_ARCH_ABI}/lib/libavdevice.so)



target_link_libraries(native-lib  AvFormat-lib  avdevice-lib avfilter-lib avutil-lib
        avcodec-lib swresample-lib  swscale-lib device-lib  ${android-libs})

target_include_directories(native-lib PUBLIC
        ${CMAKE_SOURCE_DIR}/glm
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/utils/include
        ${CMAKE_SOURCE_DIR}/recorder/include
        ${CMAKE_SOURCE_DIR}/player/render/audio
        ${CMAKE_SOURCE_DIR}/player/render
        ${CMAKE_SOURCE_DIR}/player/decoder
        ${CMAKE_SOURCE_DIR}/player
        ${CMAKE_SOURCE_DIR}/bean
        ${FFMPEG_LIBS}/${CMAKE_ANDROID_ARCH_ABI}/include)