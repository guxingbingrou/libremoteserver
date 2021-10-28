add_executable(X11VideoCaptureTest 
	${PROJECT_SOURCE_DIR}/tests/VideoTest/X11VideoCaptureTest.cpp 
	${PROJECT_SOURCE_DIR}/src/Base/Logger/Logger.cpp
	${PROJECT_SOURCE_DIR}/src/Base/BufferQueue/BufferQueue.hpp
	)
	
set_target_properties(X11VideoCaptureTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set_target_properties(X11VideoCaptureTest PROPERTIES CLEAN_DIRECT_OUTPUT 1)
link_directories(${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(X11VideoCaptureTest 
	VideoCapture)

add_executable(FFmepgVideoEncoderTest 
	${PROJECT_SOURCE_DIR}/tests/VideoTest/FFmepgVideoEncoderTest.cpp 
	${PROJECT_SOURCE_DIR}/src/Base/Logger/Logger.cpp
	${PROJECT_SOURCE_DIR}/src/Base/BufferQueue/BufferQueue.hpp
	)
	
set_target_properties(FFmepgVideoEncoderTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set_target_properties(FFmepgVideoEncoderTest PROPERTIES CLEAN_DIRECT_OUTPUT 1)
link_directories(${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(FFmepgVideoEncoderTest 
	VideoCapture
	VideoEncoder)

	
	
#sdl play images
find_package(SDL2 REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/src ${SDL2_INCLUDE_DIRS})
add_executable(sdl_play sdl_play.cpp  ${PROJECT_SOURCE_DIR}/src/Base/Logger/Logger.cpp)
set_target_properties(sdl_play PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set_target_properties(sdl_play PROPERTIES CLEAN_DIRECT_OUTPUT 1)
target_link_libraries(sdl_play ${SDL2_LIBRARIES})	

#live555 RTSP
include_directories(${PROJECT_SOURCE_DIR}/include/live555/BasicUsageEnvironment
	${PROJECT_SOURCE_DIR}/include/live555/groupsock
	${PROJECT_SOURCE_DIR}/include/live555/liveMedia
	${PROJECT_SOURCE_DIR}/include/live555/UsageEnvironment)
add_executable(TestLive555 TestLive555.cpp  ${PROJECT_SOURCE_DIR}/src/Base/Logger/Logger.cpp)
set_target_properties(TestLive555 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set_target_properties(TestLive555 PROPERTIES CLEAN_DIRECT_OUTPUT 1)

target_link_libraries(TestLive555 
	VideoCapture
	VideoEncoder
	RtspSever
    ${PROJECT_SOURCE_DIR}/lib/Live555/libliveMedia.so.94.0.3
    ${PROJECT_SOURCE_DIR}/lib/Live555/libgroupsock.so.30.0.4
    ${PROJECT_SOURCE_DIR}/lib/Live555/libBasicUsageEnvironment.so.1.0.1
    ${PROJECT_SOURCE_DIR}/lib/Live555/libUsageEnvironment.so.3.1.0
	-lssl -lcrypto
	)	


# Razor
include_directories(${PROJECT_SOURCE_DIR}/include/Razor)
add_executable(sim_sender_test sim_sender_test.cpp)
link_directories(${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(sim_sender_test 
	VideoCapture
	VideoEncoder
	${PROJECT_SOURCE_DIR}/lib/Razor/librazor.so)

set_target_properties(sim_sender_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set_target_properties(sim_sender_test PROPERTIES CLEAN_DIRECT_OUTPUT 1)








