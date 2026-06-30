cmake_minimum_required(VERSION 3.16)
project(MoodPlayer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# فعال کردن ابزار MOC
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)
find_package(X11 REQUIRED)

# اجرای اجباری ابزار moc روی فایل هدر ما
qt6_wrap_cpp(MOC_SOURCES MainWindow.h)

add_executable(MoodPlayer
    main.cpp
    MainWindow.cpp
    ${MOC_SOURCES} # اضافه کردن فایل تولید شده توسط moc
)

target_link_libraries(MoodPlayer PRIVATE Qt6::Widgets Qt6::Multimedia ${X11_LIBRARIES})
target_include_directories(MoodPlayer PRIVATE ${X11_INCLUDE_DIR})