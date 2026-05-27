set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(BOOST_INCLUDE_LIBRARIES filesystem thread serialization date_time)
set(BOOST_EXCLUDE_LIBRARIES "")  # optional

include(FetchContent)

FetchContent_Declare(
  Boost
  URL https://github.com/boostorg/boost/releases/download/boost-1.83.0/boost-1.83.0.tar.gz
  OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(Boost)

# Enable only needed Boost libraries
set(BOOST_INCLUDE_LIBRARIES filesystem thread serialization date_time)
unset(CMAKE_POLICY_VERSION_MINIMUM)
