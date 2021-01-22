set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)


set(SRCS
  include/csg/core.h

  src/internal.h
  src/c_malloc.c
  src/c_node.c
  src/c_camera.c
  src/c_transform.c
  src/c_render.c
  src/c_geometry.c
  src/c_material.c
  )

set(NAME csg_core)

add_library(${NAME} SHARED ${SRCS})
target_include_directories(${NAME} PUBLIC include/)
target_link_libraries(${NAME} PRIVATE GLEW GL m cglm)
target_link_directories(${NAME} PRIVATE /usr/local/lib)
target_include_directories(${NAME} PRIVATE  /usr/local/include)

