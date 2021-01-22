
set(NAME nuklear_impl)
add_library(${NAME} SHARED contrib/nuklear/nuklear_impl.c)
target_include_directories(${NAME} PRIVATE /usr/local/include)
target_link_directories(${NAME} PRIVATE /usr/local/lib)
target_link_libraries(${NAME} PRIVATE glfw GLEW GL m)


set(NAME test1)
add_executable(${NAME} ${NAME}.c )
target_include_directories(${NAME} PRIVATE /usr/local/include)
target_link_directories(${NAME} PRIVATE /usr/local/lib)
target_link_libraries(${NAME} PRIVATE csg_core )


set(NAME test2)
add_executable(${NAME} ${NAME}.c )
target_include_directories(${NAME} PRIVATE /usr/local/include)
target_link_directories(${NAME} PRIVATE /usr/local/lib)
target_link_libraries(${NAME} PRIVATE csg_core glfw GLEW GL m nuklear_impl)

set(NAME test3)
add_executable(${NAME} ${NAME}.c glfw3_adapter_ops.c )
target_include_directories(${NAME} PRIVATE /usr/local/include)
target_link_directories(${NAME} PRIVATE /usr/local/lib)
target_link_libraries(${NAME} PRIVATE csg_gui GLEW GL glfw )

set(NAME test4)
add_executable(${NAME} ${NAME}.c test4_ui.c )
target_include_directories(${NAME} PRIVATE /usr/local/include)
target_link_directories(${NAME} PRIVATE /usr/local/lib)
target_link_libraries(${NAME} PRIVATE csg_core csg_gui csg_gui_glfw3 nuklear_impl GLEW GL glfw )
