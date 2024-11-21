cmake_minimum_required(VERSION 3.24)
project(ttoolkit)

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

# glad
set(SOURCES
        ${SOURCES}
        ext/glad/src/glad.c)
include_directories(ext/glad/include)
set(EXT_INCLUDES ${EXT_INCLUDES} ext/glad/include)

# main program
set(SOURCES
        ${SOURCES}
        src/main.cpp
        src/application.cpp
        src/shader.cpp
        src/binary_stream.cpp
        src/scene_renderer.cpp
        src/imgui_impl_opengl3.cpp
        src/nu/scene.cpp
        src/nu/texture.cpp
        src/nu/material.cpp
        src/nu/instance.cpp
        src/nu/geometry_definition.cpp
        src/nu/geometry_part.cpp
        src/nu/geometry_primitive.cpp
        src/nu/vertex.cpp
        src/math/vec2.cpp
        src/math/vec3.cpp
        src/math/vec4.cpp
        src/math/mat4x4.h
        src/math/mat4x4.cpp
        )

add_executable(ttoolkit ${SOURCES})
target_include_directories(ttoolkit PUBLIC ${EXT_INCLUDES})
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