cmake_minimum_required(VERSION 3.0.2)

project(IOT_API)

set(CMAKE_CXX_FLAGS "-std=c++11")

include (jsoncpp/Files.cmake)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(IOTAPI_HEADERS 
    ${JSONCPP_HDRS}
    IOT_WriteData.h
    IOT_ReadData.h
    IOT_ReadDataFilter.h
    IOT_defines.h
    IOT_RegDevice.h
    IOT_GetDevice.h
    IOT_RestClient.h
    IOT_Base64.h
    IOT_Quota.h
    IOT_QuotaDevice.h
    IOT_API.h
)

set(IOTAPI_SOURCES
    ${JSONCPP_SRCS}
    IOT_WriteData.cpp
    IOT_ReadData.cpp
    IOT_ReadDataFilter.cpp
    IOT_RegDevice.cpp
    IOT_GetDevice.cpp
    IOT_RestClient.cpp
    IOT_Base64.cpp
    IOT_Quota.cpp
    IOT_QuotaDevice.cpp
    IOT_API.cpp
)

# Shared library
add_library(IOT_API SHARED ${IOTAPI_SOURCES})
set_target_properties(IOT_API PROPERTIES VERSION ${IOTAPI_VERSION} SOVERSION ${IOTAPI_MAJOR})
target_link_libraries(IOT_API ${Utils_LIBRARIES} curl -lpthread)

install(TARGETS IOT_API
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

install(FILES ${IOTAPI_HEADERS} DESTINATION include/IOT_API)

configure_file(FindIOT_API.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/FindIOT_API.cmake @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/FindIOT_API.cmake DESTINATION share/cmake/modules)


project (Kaspos)
include_directories(include)


file(GLOB SOURCES "src/*.cpp")

add_executable(Kaspos ${SOURCES})

add_subdirectory (src)

target_link_libraries(Kaspos ${LIBS})


set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_work")
FIND_PACKAGE(Config++ REQUIRED)

include_directories(${MOSQUITTO_INCLUDE_DIRS})
FIND_PACKAGE(Mosquitto REQUIRED)

FIND_PACKAGE(Mosquittopp REQUIRED)
include_directories(${MOSQUITTOPP_INCLUDE_DIRS})

FIND_PACKAGE(Modbus REQUIRED)
include_directories(${MODBUS_INCLUDE_DIRS})

target_link_libraries(Kaspos config++ ${MOSQUITTO_LIBRARIES} ${MOSQUITTOPP_LIBRARIES} ${MODBUS_LIBRARIES} -lpthread -lm IOT_API)
