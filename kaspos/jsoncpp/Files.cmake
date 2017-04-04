
ADD_DEFINITIONS(-DJSON_IS_AMALGAMATION)

SET(JSONCPP_HDRS
    jsoncpp/json/json.h  
)

SET(JSONCPP_SRCS
    jsoncpp/jsoncpp.cpp
)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp
    ${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp/json
)

