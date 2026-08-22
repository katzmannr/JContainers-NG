set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(BOOST_INCLUDE_LIBRARIES filesystem thread serialization date_time)
set(BOOST_EXCLUDE_LIBRARIES "")  # optional

include(FetchContent)

# 1.84.0 has std_variant compatibilty layer added which we need for item class and collections
# Rolling back to 1.83.0 means we need to implement our own compatibility internally.
FetchContent_Declare(
  Boost
  URL https://github.com/boostorg/boost/releases/download/boost-1.84.0/boost-1.84.0.tar.gz
  OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(Boost)

# Enable only needed Boost libraries
set(BOOST_INCLUDE_LIBRARIES filesystem thread serialization date_time)
unset(CMAKE_POLICY_VERSION_MINIMUM)
