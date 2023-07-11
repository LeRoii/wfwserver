cmake_minimum_required(VERSION 3.22.0)

set(BITSFLOW_HOME "/app/prog/software/bitsflow")
set(BITSFLOW_HL_HOME "/app/prog/software/bitsflow")
set(BITSFLOW_3RD_EX_HOME "/app/prog/software/bitsflow/3rdparty_extends")


set(CMAKE_CXX_COMPILER "g++")

set(PROJECT_NAME ServerA)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 11)

add_compile_options(-std=c++11 -fno-stack-protector -fPIC)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -pthread -DBITSFLOW_MAJOR=2 -DBITSFLOW_MINOR=27 -DBITSFLOW_MAINTENANCE=34 -DBITSFLOW_APPEND=0 -g -ggdb3 -DDEBUG -Wall -W -Wno-deprecated -D__LINUX__ -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE  -D__LINUX_NEOKYLIN_5__ -D_GLIBCXX_USE_CXX11_ABI=1")


include_directories(thirdparty/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-Bsymbolic")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bsymbolic")

set(BITSFLOW++ ${BITSFLOW_HOME}/lib/libbitsflow++.so)
set(YOYOCORE++ ${BITSFLOW_HOME}/lib/libyoyocore++.so)
set(BITSFLOW ${BITSFLOW_HOME}/lib/libbitsflow.so)
set(YOYOCORE_REGEX ${BITSFLOW_HOME}/lib/libyoyocore_regex.so)
set(YOYOCORE_STORAGE ${BITSFLOW_HOME}/lib/libyoyocore_storage.so)
set(YOYOCORE_EVENT ${BITSFLOW_HOME}/lib/libyoyocore_event.so)
set(YOYOCORE ${BITSFLOW_HOME}/lib/libyoyocore.so)
set(YOYODEFS ${BITSFLOW_HOME}/lib/libyoyodefs.so)
set(BITSFLOW_HL ${BITSFLOW_HOME}/lib/libbitsflow-hl++.so)
set(AVRO ${BITSFLOW_3RD_EX_HOME}/lib/libavro.so.23)
set(LIBREGISTER /usr/lib64/libregister.so)
set(LIBETCDCLIENT /usr/lib64/libetcdClient.so)
set(LIBREMOTE /usr/lib64/libRemotePlugin.so)

file(GLOB_RECURSE SRC_LIST "./src/*.cpp")
add_executable(${PROJECT_NAME} ${SRC_LIST})


# ���Ӳ���
target_link_libraries(${PROJECT_NAME} ${BITSFLOW++})
target_link_libraries(${PROJECT_NAME} ${YOYOCORE++})
target_link_libraries(${PROJECT_NAME} ${BITSFLOW})
target_link_libraries(${PROJECT_NAME} ${YOYOCORE_REGEX})
target_link_libraries(${PROJECT_NAME} ${YOYOCORE_STORAGE})
target_link_libraries(${PROJECT_NAME} ${YOYOCORE_EVENT})
target_link_libraries(${PROJECT_NAME} ${YOYOCORE})
target_link_libraries(${PROJECT_NAME} ${YOYODEFS})
target_link_libraries(${PROJECT_NAME} ${BITSFLOW_HL})
target_link_libraries(${PROJECT_NAME} ${AVRO})
target_link_libraries(${PROJECT_NAME} ${LIBREMOTE})
target_link_libraries(${PROJECT_NAME} ${LIBETCDCLIENT})
target_link_libraries(${PROJECT_NAME} ${LIBREGISTER})