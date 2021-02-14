CMAKE_MINIMUM_REQUIRED ( VERSION 3.0 )

CMAKE_POLICY ( VERSION 3.0 )

PROJECT ( sspender )

SET (CMAKE_CXX_STANDARD 11)

find_library(LIBCONFIG_PATH config++)
find_package (Threads)

ADD_EXECUTABLE ( sspender
  src/sspender.cpp
  src/config_parser.cpp
  src/cpu.cpp
  src/disk.cpp
  src/device.cpp
  src/manager.cpp
  src/monitor.cpp
  src/partition_table.cpp
  src/watch_dog.cpp
  src/utils.cpp
  )

target_link_libraries(sspender "${LIBCONFIG_PATH}" ${CMAKE_THREAD_LIBS_INIT})

INSTALL (TARGETS sspender DESTINATION /usr/local/bin)
