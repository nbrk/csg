set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)


set(SRCS
    include/csg/gui_glfw3.h

    src/c_adapter_ops_glfw3.c
  )

set(NAME csg_gui_glfw3)

add_library(${NAME} SHARED ${SRCS})
target_include_directories(${NAME} PUBLIC include/)
target_link_libraries(${NAME} PRIVATE csg_gui GLEW GL glfw )
target_link_directories(${NAME} PRIVATE /usr/local/lib)
target_include_directories(${NAME} PRIVATE  /usr/local/include)
