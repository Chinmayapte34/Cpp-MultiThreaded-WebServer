cmake_minimum_required(VERSION 3.10)
project(MultiThreadedWebServer)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

# 1. Find the threads package
find_package(Threads REQUIRED)

# 2. Find all source files
file(GLOB SOURCES "src/*.cpp")

# 3. CREATE the target first (The "Phone")
add_executable(WebServer ${SOURCES})

# 4. LINK the library to the target (The "Charger")
target_link_libraries(WebServer PUBLIC Threads::Threads)