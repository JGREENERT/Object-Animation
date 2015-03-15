cmake_minimum_required(VERSION 2.8.4)
project(Project2)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories("C:/GLFW/include" "C:/glm" "C:/glew/include")
link_directories("C:/glew/lib/Release/x64" "C:/GLFW/lib-mingw" )

set(SOURCE_FILES "main.cpp"
                 "Simple Objects/Cylinder.cpp" "Simple Objects/Sphere.cpp" "Simple Objects/Torus.cpp"
                 "Complex Objects/Raindrop.cpp" "Complex Objects/MerryGoRound.cpp"  "Complex Objects/Lamp.cpp")
add_executable(Project2 ${SOURCE_FILES})
  target_link_libraries (Project2 ${GLFW3_LIBRARY} ${OPENGL_LIBRARIES} ${COCOA_LIBRARY}
     ${COREVID_LIBRARY} ${IOKIT_LIBRARY} ${GLEW_LIBRARY})

target_link_libraries (Project2 ${OPENGL_LIBRARIES} glfw3 glew32s glu32 opengl32)
find_package (PkgConfig REQUIRED)
find_package (OpenGL REQUIRED)