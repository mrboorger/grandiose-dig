cmake_minimum_required(VERSION 3.14)

project(grandious_dig LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -Og")

# You can set CMake prefix path here or pass it to CMake executable.

#set(CMAKE_PREFIX_PATH "~/Qt/5.15.2/gcc_64/lib/cmake")
#set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/mingw81_64/lib/cmake")

if (NOT CMAKE_PREFIX_PATH)
    message(WARNING "CMAKE_PREFIX_PATH is not defined, you may need to set it "
            "(-DCMAKE_PREFIX_PATH=\"path/to/Qt/lib/cmake\")")
endif ()

set(QT_VERSION 5)
set(REQUIRED_LIBS Core Widgets Multimedia)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Widgets Qt5::Multimedia)
find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(RESOURCES resources.qrc)

set(SOURCES
        main.cpp
        controller/abstract_controller.h controller/abstract_controller.cpp
        controller/controller.h controller/controller.cpp
        model/model.h model/model.cpp view/view.h view/view.cpp
        model/constants.h model/block.h model/block.cpp model/map.h model/map.cpp
        model/abstract_map_generator.h model/abstract_map_generator.cpp
        model/flat_map_generator.h model/flat_map_generator.cpp
        view/camera.h view/camera.cpp view/block_drawer.h view/block_drawer.cpp

        )

add_executable(${PROJECT_NAME} ${SOURCES} ${RESOURCES})
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})

