#set(NAME test1)
#add_executable(${NAME} ${NAME}.c )
#target_include_directories(${NAME} PRIVATE /usr/local/include)
#target_link_directories(${NAME} PRIVATE /usr/local/lib)
#target_link_libraries(${NAME} PRIVATE csg_core )


#set(NAME test2)
#add_executable(${NAME} ${NAME}.c )
#target_include_directories(${NAME} PRIVATE /usr/local/include)
#target_link_directories(${NAME} PRIVATE /usr/local/lib)
#target_link_libraries(${NAME} PRIVATE csg_core csg_gui csg_gui_glfw3 glfw GLEW GL )

#set(NAME test3)
#add_executable(${NAME} ${NAME}.c )
#target_include_directories(${NAME} PRIVATE /usr/local/include)
#target_link_directories(${NAME} PRIVATE /usr/local/lib)
#target_link_libraries(${NAME} PRIVATE csg_gui csg_gui_glfw3 GLEW GL glfw )

#set(NAME test4)
#add_executable(${NAME} ${NAME}.c test4_ui.c )
#target_include_directories(${NAME} PRIVATE /usr/local/include)
#target_link_directories(${NAME} PRIVATE /usr/local/lib)
#target_link_libraries(${NAME} PRIVATE csg_core csg_gui csg_gui_glfw3 GLEW GL)

set(NAME test5)
add_executable(${NAME} ${NAME}.c )
target_include_directories(${NAME} PRIVATE /usr/local/include)
target_link_directories(${NAME} PRIVATE /usr/local/lib)
target_link_libraries(${NAME} PRIVATE csg_core csg_gui csg_gui_glfw3)


set(NAME test6)
add_executable(${NAME} ${NAME}.c )
target_include_directories(${NAME} PRIVATE /usr/local/include)
target_link_directories(${NAME} PRIVATE /usr/local/lib)
target_link_libraries(${NAME} PRIVATE csg_core csg_gui csg_wavefront csg_gui_glfw3)

set(NAME test7)
add_executable(${NAME} ${NAME}.c )
target_include_directories(${NAME} PRIVATE /usr/local/include)
target_link_directories(${NAME} PRIVATE /usr/local/lib)
target_link_libraries(${NAME} PRIVATE csg_core csg_gui csg_gui_glfw3)
