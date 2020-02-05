cmake_minimum_required(VERSION 3.13)
project(udp_firmware_uploader C)

set(CMAKE_C_STANDARD 99)



#add_executable(${CMAKE_PROJECT_NAME} ${source_files})
add_executable(udp_firmware_uploader main.c src/packet.c include/packet.h src/socket.c include/socket.h include/main.h)

target_link_libraries(${CMAKE_PROJECT_NAME} ws2_32)

if(WIN32)
    target_link_libraries(${CMAKE_PROJECT_NAME} wsock32 ws2_32)
endif()