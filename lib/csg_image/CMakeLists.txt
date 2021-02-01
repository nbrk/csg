set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)


set(SRCS
    include/csg/image.h

    src/internal_stb_image.h
    src/c_image.c
    src/c_stb_image_impl.c
  )

set(NAME csg_image)

add_library(${NAME} SHARED ${SRCS})
target_include_directories(${NAME} PUBLIC include/)
target_link_libraries(${NAME} PRIVATE csg_core m)
target_link_directories(${NAME} PRIVATE /usr/local/lib)
target_include_directories(${NAME} PRIVATE  /usr/local/include)
