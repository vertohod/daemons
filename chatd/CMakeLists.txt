cmake_minimum_required(VERSION 2.8)

project(chatd)

find_package(Git REQUIRED)

execute_process(
    COMMAND ${GIT_EXECUTABLE} log HEAD~1..HEAD --pretty=oneline
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_HASH_TEMP
)

string(REGEX MATCH [0-9a-f]+ GIT_HASH "${GIT_HASH_TEMP}")

add_definitions(-DDAEMON=\"chatd\" -DGIT_HASH=\"${GIT_HASH}\")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/../include)
include_directories(${CMAKE_SOURCE_DIR}/swarm)
include_directories(${CMAKE_SOURCE_DIR}/objects)
include_directories(${CMAKE_SOURCE_DIR}/commands)
include_directories(${CMAKE_SOURCE_DIR}/swarm/objects)
include_directories(${CMAKE_SOURCE_DIR}/swarm/commands)

file(MAKE_DIRECTORY "libs")

set(SOURCE_EXE 
    ../include/serialization.cpp
    ../include/shared_mutex.cpp
    ../include/config_base.cpp
    ../include/stat_base.cpp
    ../include/format.cpp
    ../include/stream.cpp
    ../include/config.cpp
    ../include/socket.cpp
    ../include/types.cpp
    ../include/util.cpp
    ../include/log.cpp
	./swarm/commands/get_servers_list.cpp
	./swarm/commands/authentication.cpp
    ./swarm/commands/get_remainder.cpp
	./swarm/commands/response.cpp
    ./swarm/commands/get_salt.cpp
	./swarm/commands/command.cpp
	./swarm/commands/packet.cpp
	./swarm/commands/unlock.cpp
	./swarm/commands/result.cpp
	./swarm/commands/lock.cpp
	./swarm/objects/server_short.cpp
    ./swarm/objects/server.cpp
    ./swarm/objects/record.cpp
    ./swarm/objects/answer.cpp
    ./swarm/objects/empty.cpp
    ./swarm/objects/salt.cpp
	./swarm/connection_store.cpp
    ./swarm/convert_to_oid.cpp
    ./swarm/key_interface.cpp
    ./swarm/converter.cpp
    ./swarm/key_store.cpp
    ./swarm/swarm.cpp
    ./swarm/pusher.cpp
    ./swarm/object.cpp
    ./swarm/router.cpp
    ./swarm/table.cpp
	./swarm/range.cpp
	./commands/changed_nickname.cpp
	./commands/look_for_channel.cpp
	./commands/create_channel.cpp
	./commands/look_for_user.cpp
	./commands/get_user_list.cpp
	./commands/password_hash.cpp
	./commands/get_history.cpp
	./commands/create_user.cpp
	./commands/rename_user.cpp
	./commands/found_user.cpp
	./commands/get_status.cpp
	./commands/come_out.cpp
	./commands/come_in.cpp
	./commands/status.cpp
	./commands/client.cpp
	./commands/logout.cpp
	./commands/inside.cpp
	./commands/login.cpp
	./commands/event.cpp
	./commands/typing.cpp
	./commands/ping.cpp
	./commands/pong.cpp
	./objects/channel_user.cpp
	./objects/message.cpp
	./objects/channel.cpp
	./objects/user.cpp
    service_messages.cpp
    stat_daemon.cpp
	processor.cpp
	service.cpp
)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost COMPONENTS system regex thread date_time filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

#message(STATUS "*****")

# Google test =======================================
if (NOT EXISTS "${CMAKE_SOURCE_DIR}/libs/googletest")
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" clone https://github.com/google/googletest.git
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/libs"
    )
endif()
add_subdirectory(libs/googletest)
include_directories(${CMAKE_SOURCE_DIR}/libs/googletest/googletest/include)
#=================================================

# RapidJSON =======================================
if (NOT EXISTS "${CMAKE_SOURCE_DIR}/libs/rapidjson")
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" clone https://github.com/miloyip/rapidjson.git
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/libs"
    )
endif()
add_subdirectory(libs/rapidjson)
include_directories(${CMAKE_SOURCE_DIR}/libs/rapidjson/include)
#=================================================

# WebSocket++ =======================================
if (NOT EXISTS "${CMAKE_SOURCE_DIR}/libs/websocketpp")
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" clone https://github.com/zaphoyd/websocketpp.git
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/libs"
    )
endif()
add_subdirectory(libs/websocketpp)
include_directories(${CMAKE_SOURCE_DIR}/libs/websocketpp)
#=================================================

# Crypto++ =======================================
if (NOT EXISTS "${CMAKE_SOURCE_DIR}/libs/cryptopp")
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" clone https://github.com/weidai11/cryptopp.git
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/libs"
        )
endif()
#add_subdirectory(libs/cryptopp)
include_directories(${CMAKE_SOURCE_DIR}/libs/cryptopp)
#=================================================

add_executable(${PROJECT_NAME} ${SOURCE_EXE} chatd.cpp)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} cryptopp-static)

add_executable(tests ${SOURCE_EXE} unit_tests.cpp)
target_link_libraries(tests ${Boost_LIBRARIES} gtest cryptopp-static)
