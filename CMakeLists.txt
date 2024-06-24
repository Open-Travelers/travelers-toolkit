cmake_minimum_required(VERSION 3.24)
project(ttoolkit)

set(CMAKE_CXX_STANDARD 23)
set(CXX_STANDARD 23)

# nativefiledialog-extended
add_subdirectory("ext/portable-file-dialogs")


# opengl
find_package(OpenGL REQUIRED)
set(LIBRARIES ${LIBRARIES} ${OPENGL_LIBRARY})

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
        "imgui-SFML.cpp"
        "ext/imgui/imgui.cpp"
        "ext/imgui/imgui_draw.cpp"
        "ext/imgui/imgui_widgets.cpp"
        "ext/imgui/imgui_tables.cpp")
include_directories("ext/imgui")

# glad
set(SOURCES
        ${SOURCES}
        "ext/glad/src/glad.c" math/vec3.cpp)
include_directories("ext/glad/include")

# main program
set(SOURCES
        ${SOURCES}
        main.cpp 
        application.cpp 
        shader.cpp
        binary_stream.cpp 
        scene_renderer.cpp
        imgui_impl_opengl3.cpp
        nu/scene.cpp 
        nu/texture.cpp 
        nu/material.cpp 
        nu/instance.cpp
        nu/geometry_definition.cpp 
        nu/geometry_part.cpp 
        nu/geometry_primitive.cpp 
        nu/vertex.cpp  
        math/vec2.cpp 
        math/vec3.cpp 
        math/vec4.cpp 
        math/mat4x4.h 
        math/mat4x4.cpp
        )

add_executable(ttoolkit ${SOURCES})
target_link_libraries(ttoolkit PRIVATE ${LIBRARIES})

# post build stuff
if(WIN32)
    add_custom_command(
            TARGET ttoolkit
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:ttoolkit>
            VERBATIM)
endif()
install(TARGETS ttoolkit)