include_directories(${PROJECT_SOURCE_DIR}/src ${X11_INCLUDE_DIRS} ${Xext_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include/FFmpeg)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
add_library(VideoCapture SHARED ${PROJECT_SOURCE_DIR}/src/VideoCapture/VideoCapture.cpp 
	${PROJECT_SOURCE_DIR}/src/VideoCapture/X11DesktopCapture.cpp 
	${PROJECT_SOURCE_DIR}/src/Base/Logger/Logger.cpp
	${PROJECT_SOURCE_DIR}/src/Base/BufferQueue/BufferQueue.hpp)
target_link_libraries(VideoCapture
    ${X11_LIBRARIES} 
    ${Xext_LIBRARIES}
    )
set_target_properties(VideoCapture PROPERTIES CLEAN_DIRECT_OUTPUT 1)


set (FFMPEG_LIBS_DIR ${PROJECT_SOURCE_DIR}/lib/FFmpeg)

set (AVCODEC_LIBRARIES ${FFMPEG_LIBS_DIR}/libavcodec.so)
set (AVFORMAT_LIBRARIES ${FFMPEG_LIBS_DIR}/libavformat.so)
set (AVUTIL_LIBRARIES ${FFMPEG_LIBS_DIR}/libavutil.so)
set (SWSCALE_LIBRARIES ${FFMPEG_LIBS_DIR}/libswscale.so)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
add_library(VideoEncoder SHARED ${PROJECT_SOURCE_DIR}/src/VideoEncoder/VideoEncoder.cpp 
	${PROJECT_SOURCE_DIR}/src/VideoEncoder/VideoEncoderFFmpeg.cpp 
	${PROJECT_SOURCE_DIR}/src/Base/Logger/Logger.cpp
	${PROJECT_SOURCE_DIR}/src/Base/BufferQueue/BufferQueue.hpp)
target_link_libraries(VideoEncoder
    ${AVCODEC_LIBRARIES} 
    ${AVFORMAT_LIBRARIES}
    ${AVUTIL_LIBRARIES} 
    ${SWSCALE_LIBRARIES}
    )
set_target_properties(VideoEncoder PROPERTIES CLEAN_DIRECT_OUTPUT 1)