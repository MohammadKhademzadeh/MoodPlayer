cmake_minimum_required(VERSION 3.16)
project(MoodPlayer VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)
find_package(X11 REQUIRED)

add_executable(MoodPlayer
    main.cpp
    MainWindow.cpp
    src/core/SettingsManager.cpp
    src/core/MoodEngine.cpp
    src/core/SystemMonitor.cpp
    src/audio/MusicEngine.cpp
    src/ui/PlayerController.cpp
    src/ui/SettingsDialog.cpp
    src/integrations/SpotifyConnector.cpp
    src/integrations/SpotifyOAuth.cpp
)

target_include_directories(MoodPlayer PRIVATE
    ${X11_INCLUDE_DIR}
    src
    src/core
    src/audio
    src/ui
    src/integrations
)

target_link_libraries(MoodPlayer PRIVATE
    Qt6::Widgets
    Qt6::Multimedia
    ${X11_LIBRARIES}
)

install(TARGETS MoodPlayer
    RUNTIME DESTINATION bin
)

install(FILES resources/MoodPlayer.desktop
    DESTINATION share/applications
)
