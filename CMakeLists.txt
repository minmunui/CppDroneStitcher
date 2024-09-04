cmake_minimum_required(VERSION 3.8)

project(OpenCVExample)

# OpenCV 패키지 찾기
find_package(OpenCV REQUIRED)

# 실행 파일 생성
add_executable(OpenCVExample main.cpp)

# OpenCV 라이브러리 연결
target_link_libraries(OpenCVExample ${OpenCV_LIBS})
