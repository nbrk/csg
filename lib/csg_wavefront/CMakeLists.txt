set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)


set(SRCS
    include/csg/wavefront.h

  src/internal_tinyobj_loader_c.h
  src/c_tinyobj_loader_impl.c
  src/c_wavefront.c
  )

set(NAME csg_wavefront)

add_library(${NAME} SHARED ${SRCS})
target_include_directories(${NAME} PUBLIC include/)
target_link_libraries(${NAME} PRIVATE csg_core csg_gui GLEW GL )
target_link_directories(${NAME} PRIVATE /usr/local/lib)
target_include_directories(${NAME} PRIVATE  /usr/local/include)
