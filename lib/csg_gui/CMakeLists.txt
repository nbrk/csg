set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)


set(SRCS
    include/csg/gui.h

    src/c_adapter.c
  )

set(NAME csg_gui)

add_library(${NAME} SHARED ${SRCS})
target_include_directories(${NAME} PUBLIC include/)
target_link_libraries(${NAME} PRIVATE GLEW GL m cglm)
target_link_directories(${NAME} PRIVATE /usr/local/lib)
target_include_directories(${NAME} PRIVATE  /usr/local/include)
