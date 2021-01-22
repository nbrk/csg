set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)


set(SRCS
    include/csg/gui_glfw3.h
    include/csg/nuklear_glfw_gl3.h

    src/internal_nuklear.h
    src/c_adapter_ops_glfw3.c
    src/c_nuklear_impl.c
  )

set(NAME csg_gui_glfw3)

add_library(${NAME} SHARED ${SRCS})
target_include_directories(${NAME} PUBLIC include/)
target_link_libraries(${NAME} PRIVATE csg_core csg_gui GLEW GL glfw m )
target_link_directories(${NAME} PRIVATE /usr/local/lib)
target_include_directories(${NAME} PRIVATE  /usr/local/include)
