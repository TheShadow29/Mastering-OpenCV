cmake_minimum_required(VERSION 2.8)
project( Cartoonify )
find_package( OpenCV REQUIRED )

Set (SRC main_cartoon.cpp)

add_executable(  ${PROJECT_NAME} ${SRC})
target_link_libraries(  ${PROJECT_NAME}  ${OpenCV_LIBS} )