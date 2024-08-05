cmake_minimum_required(VERSION 3.3)
project(ov_core)

# Include libraries (if we don't have opencv 4, then fallback to opencv 3)
# The OpenCV version needs to match the one used by cv_bridge otherwise you will get a segmentation fault!
find_package(Eigen3 REQUIRED)
find_package(OpenCV 3 QUIET)
if(NOT OpenCV_FOUND)
  find_package(OpenCV 4 REQUIRED)
endif()
find_package(Boost REQUIRED COMPONENTS system filesystem thread date_time)
message(STATUS "OPENCV: " ${OpenCV_VERSION} " | BOOST: " ${Boost_VERSION})

# If we will compile with aruco support
option(ENABLE_ARUCO_TAGS "Enable or disable aruco tag (disable if no contrib modules)" ON)
if(NOT ENABLE_ARUCO_TAGS)
  add_definitions(-DENABLE_ARUCO_TAGS=0)
  message(WARNING "DISABLING ARUCOTAG TRACKING!")
else()
  add_definitions(-DENABLE_ARUCO_TAGS=1)
endif()

# check if we have our python libs files (will search for python3 then python2 installs)
# sudo apt-get install python-matplotlib python-numpy python-dev
# https://cmake.org/cmake/help/v3.10/module/FindPythonLibs.html
find_package(PythonLibs QUIET)
option(DISABLE_MATPLOTLIB "Disable or enable matplotlib plot scripts in ov_eval" OFF)
if(PYTHONLIBS_FOUND AND NOT DISABLE_MATPLOTLIB)
  add_definitions(-DHAVE_PYTHONLIBS=1)
  message(STATUS "PYTHON VERSION: " ${PYTHONLIBS_VERSION_STRING})
  message(STATUS "PYTHON INCLUDE: " ${PYTHON_INCLUDE_DIRS})
  message(STATUS "PYTHON LIBRARIES: " ${PYTHON_LIBRARIES})
  include_directories(${PYTHON_INCLUDE_DIRS})
  list(APPEND thirdparty_libraries ${PYTHON_LIBRARIES}) # 向列表添加元素
endif()

# We need c++14 for ROS2, thus just require it for everybody
# NOTE: To future self, hope this isn't an issue...
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable compile optimizations
# -fsee: 启用分支预测的优化；-fomit-frame-pointer：这个标志指示编译器在可能的情况下省略帧指针的保存，这可以减少函数调用的开销，但可能会影响调试器的功能
# -fno-signed-zeros：这个标志告诉编译器在浮点运算中忽略正零和负零之间的区别，这可能会影响浮点运算的精度
# -fno-math-errno：禁用在数学库函数调用出错时设置errno的行为，默认情况下，数学库函数（例如sqrt、log等）在遇到错误（例如对负数取平方根）时会设置全局变量errno。
# 这对于调试有用，但会导致一些性能损耗。-funroll-loops：启用循环展开优化，默认情况下，编译器不会自动展开循环。
# 这意味着对于固定次数的循环迭代，编译器会将循环体复制多次，以减少循环控制的开销。这可以提高某些循环密集型代码的性能，尤其是当循环体较小且循环迭代次数已知时
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fsee -fomit-frame-pointer -fno-signed-zeros -fno-math-errno -funroll-loops")

# Enable debug flags (use if you want to debug in gdb)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -Wall -Wuninitialized -Wmaybe-uninitialized -fno-omit-frame-pointer")

# Find our ROS version!
# NOTE: Default to using the ROS1 package if both are in our environment
# NOTE: https://github.com/romainreignier/share_ros1_ros2_lib_demo
find_package(catkin QUIET COMPONENTS roscpp)
find_package(ament_cmake QUIET)
if(catkin_FOUND)
  message(STATUS "ROS *1* version found, building ROS1.cmake")
  include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/ROS1.cmake)
elseif(ament_cmake_FOUND)
  message(STATUS "ROS *2* version found, building ROS2.cmake")
  include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/ROS2.cmake)
else()
  message(STATUS "No ROS versions found, building ROS1.cmake")
  include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/ROS1.cmake)
endif()




