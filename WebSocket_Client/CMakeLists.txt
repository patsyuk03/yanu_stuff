cmake_minimum_required(VERSION 3.10)
project(WebSocket_Client)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Add WebSocket++ library
find_package(WebSocketPP REQUIRED)
include_directories(${WebSocketPP_INCLUDE_DIRS})

# Add JsonCpp library
# find_package(jsoncpp REQUIRED)
# message("FOUND CONFIG: ${jsoncpp_CONFIG}")
# message("INCLUDE DIRS: ${jsoncpp_INCLUDE_DIRS}")
# message("LIBRARIES: ${jsoncpp_LIBRARIES}")
# include_directories(${jsoncpp_INCLUDE_DIRS})
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

# Add the executable
add_executable(websocket_client main.cpp)

# Link libraries
target_link_libraries(websocket_client ${WebSocketPP_LIBRARIES} ${JSONCPP_LINK_LIBRARIES})
