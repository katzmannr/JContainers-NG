set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
include(FetchContent)

FetchContent_Declare(
  Boost
  URL https://github.com/boostorg/boost/releases/download/boost-1.83.0/boost-1.83.0.tar.gz
)

FetchContent_MakeAvailable(Boost)

# Enable only needed Boost libraries
set(BOOST_INCLUDE_LIBRARIES filesystem thread serialization date_time)
#add_subdirectory(${boost_SOURCE_DIR} ${boost_BINARY_DIR} EXCLUDE_FROM_ALL)
unset(CMAKE_POLICY_VERSION_MINIMUM)
