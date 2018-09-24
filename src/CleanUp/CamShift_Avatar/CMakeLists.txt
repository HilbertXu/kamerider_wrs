find_package(OpenCV)
add_executable(CamShift_Avatar src/CamShift_Avatar.cpp)
target_link_libraries(CamShift_Avatar ${OpenCV_LIBRARIES})
