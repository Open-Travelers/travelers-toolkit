cmake_minimum_required(VERSION 3.24)
project(ttoolkit)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CXX_STANDARD 23)

# portable-file-dialogs
add_subdirectory(ext/portable-file-dialogs)
set(LIBRARIES portable_file_dialogs)
set(EXT_INCLUDES ext/portable-file-dialogs)

# opengl
if(UNIX AND NOT APPLE)
    find_package(OpenGL REQUIRED)
    set(LIBRARIES ${LIBRARIES} ${OPENGL_LIBRARY})
elseif (WIN32)
    set(LIBRARIES opengl32)
endif()

# sfml
include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)
set(LIBRARIES
        ${LIBRARIES}
        sfml-graphics)

# imgui
set(SOURCES
        ${SOURCES}
        src/imgui-SFML.cpp
        ext/imgui/imgui.cpp
        ext/imgui/imgui_draw.cpp
        ext/imgui/imgui_widgets.cpp
        ext/imgui/imgui_tables.cpp)
include_directories(ext/imgui)
set(EXT_INCLUDES ${EXT_INCLUDES} ext/imgui)

# glm
add_subdirectory(ext/glm)
set(LIBRARIES 
        ${LIBRARIES}
        glm::glm)
set(EXT_INCLUDES ${EXT_INCLUDES} ext/glm)

# glad
set(SOURCES
        ${SOURCES}
        ext/glad/src/glad.c)
set(EXT_INCLUDES ${EXT_INCLUDES} ext/glad/include)

# main program
set(SOURCES
        ${SOURCES}
        src/main.cpp
        src/platform.h
        src/application.h
        src/application.cpp
        src/imgui_impl_opengl3.cpp

        src/shader.h
        src/shader.cpp
        src/file_binary_reader.h
        src/file_binary_reader.cpp

        src/twoc/reader_item.h

        src/twoc/binary_reader.h
        src/twoc/binary_reader.cpp
        src/twoc/executable.h
        src/twoc/executable.cpp
        src/twoc/executable_elf.cpp
        src/twoc/level_data.h
        src/twoc/level_data.cpp
        src/twoc/level.h
        src/twoc/level.cpp
        src/twoc/elf/file.h
        src/twoc/elf/file.cpp
        src/twoc/elf/header.h
        src/twoc/elf/header.cpp
        src/twoc/elf/program_header.h
        src/twoc/elf/program_header.cpp
        src/twoc/elf/section_header.h
        src/twoc/elf/section_header.cpp
        src/twoc/elf/symbol.h
        src/twoc/elf/symbol.cpp
        src/twoc/crt/crate_group.h
        src/twoc/crt/crate_group.cpp
        src/twoc/crt/crate_store.h
        src/twoc/crt/crate_store.cpp
        src/twoc/crt/crate.h
        src/twoc/crt/crate.cpp
        src/twoc/nu/instance.h
        src/twoc/nu/instance.cpp
        src/twoc/nu/material.h
        src/twoc/nu/material.cpp
        src/twoc/nu/texture.h
        src/twoc/nu/texture.cpp
        src/twoc/nu/vertex.h
        src/twoc/nu/vertex.cpp
        src/twoc/nu/scene.h
        src/twoc/nu/scene.cpp

        src/ui/types.h 
        src/ui/window.h
        src/ui/manager.h
        src/ui/manager.cpp
        src/ui/view.h 
        src/ui/view.cpp 

        src/application_project.h
        src/application_project.cpp
        src/application_data.h

        src/views/welcome/welcome_view.h
        src/views/welcome/welcome_view.cpp 
)

add_executable(ttoolkit ${SOURCES}
    src/twoc/character_data.h
    src/twoc/character_data.cpp
    src/memory_binary_reader.h
    src/memory_binary_reader.cpp
    src/twoc/dat/file.h
    src/twoc/nu/special_object.h
    src/twoc/nu/spline.h
    src/twoc/nu/spline.cpp
    src/twoc/nu/primitive.h
    src/twoc/nu/primitive.cpp
    src/twoc/nu/mesh.h
    src/twoc/nu/mesh.cpp
    src/twoc/nu/geometry_object.h
    src/twoc/nu/geometry_object.cpp
    src/twoc/dat/file.cpp
    src/twoc/dat/file_header.h
    src/twoc/dat/file_header.cpp
    src/scene_renderer.h
    src/scene_renderer.cpp
    src/camera.h
    src/camera.cpp
    src/gl.h
    src/gl.cpp)
target_include_directories(ttoolkit PUBLIC src/ ${EXT_INCLUDES})
target_link_libraries(ttoolkit PRIVATE ${LIBRARIES})

# post build stuff
if(WIN32)
    add_custom_command(
            TARGET ttoolkit
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:ttoolkit>
            VERBATIM)
    add_custom_command(
            TARGET ttoolkit
            COMMENT "Copy SFML DLLs" POST_BUILD 
            COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:ttoolkit> $<TARGET_RUNTIME_DLLS:ttoolkit>
            COMMAND_EXPAND_LISTS    )
endif()
install(TARGETS ttoolkit)
