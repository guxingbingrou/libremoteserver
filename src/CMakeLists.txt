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
add_library(VideoEncoder SHARED 
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

include_directories(${PROJECT_SOURCE_DIR}/include/live555/liveMedia
    ${PROJECT_SOURCE_DIR}/include/live555/groupsock
    ${PROJECT_SOURCE_DIR}/include/live555/BasicUsageEnvironment
    ${PROJECT_SOURCE_DIR}/include/live555/UsageEnvironment)
add_library(RtspSever SHARED ${PROJECT_SOURCE_DIR}/src/Rtsp/Live555/H264LiveVideoSource.cpp 
	${PROJECT_SOURCE_DIR}/src/Rtsp//Live555/H264VideoLiveServerMediaSubsession.cpp 
	${PROJECT_SOURCE_DIR}/src/Base/Logger/Logger.cpp
	${PROJECT_SOURCE_DIR}/src/Base/BufferQueue/BufferQueue.hpp)
target_link_libraries(RtspSever
    ${PROJECT_SOURCE_DIR}/lib/Live555/libliveMedia.so.94.0.3
    ${PROJECT_SOURCE_DIR}/lib/Live555/libgroupsock.so.30.0.4
    ${PROJECT_SOURCE_DIR}/lib/Live555/libBasicUsageEnvironment.so.1.0.1
    ${PROJECT_SOURCE_DIR}/lib/Live555/libUsageEnvironment.so.3.1.0
    )
set_target_properties(RtspSever PROPERTIES CLEAN_DIRECT_OUTPUT 1)