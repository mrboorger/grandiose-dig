cmake_minimum_required(VERSION 3.14)

project(grandiose_dig LANGUAGES CXX)

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
set(REQUIRED_LIBS Widgets Multimedia)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Widgets Qt5::Multimedia)
find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(RESOURCES resources.qrc)

set(SOURCES
        controller/controller.cpp
        main.cpp
        model/abstract_map.cpp
        model/block.cpp
        model/chunk.cpp
        model/chunk_map.cpp
        model/effects.cpp
        model/flat_chunk_map_generator.cpp
        model/flat_map_generator.cpp
        model/inventory_item.cpp
        model/inventory.cpp
        model/map.cpp
        model/mob.cpp
        model/model.cpp
        model/move_vector.cpp
        model/moving_object.cpp
        model/perlin_chunk_map_generator.cpp
        model/perlin_noise1d.cpp
        model/perlin_noise2d.cpp
        model/player.cpp
        model/strategy.cpp
        utils.cpp
        view/abstract_menu.cpp
        view/block_drawer.cpp
        view/buffered_map_drawer.cpp
        view/camera.cpp
        view/gl_func.cpp
        view/gl_map_drawer.cpp
        view/inventory_drawer.cpp
        view/light.cpp
        view/light_map.cpp
        view/main_menu.cpp
        view/menu_button.cpp
        view/named_menu_slider.cpp
        view/map_drawer.cpp
        view/moving_object_drawer.cpp
        view/pause_menu.cpp
        view/scrollable_v_box_widget.cpp
        view/settings_menu.cpp
        view/sound_manager.cpp
        view/texture_atlas.cpp
        view/view.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${RESOURCES})
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})
