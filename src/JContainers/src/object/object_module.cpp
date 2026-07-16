#include <jansson.h>

#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#define TEST_COMPILATION_ENABLED 1
#define BG_WORKER_ENABLED 1

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/export.hpp>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/deque.hpp>
#include <boost/serialization/unordered_set.hpp>
#include <boost/serialization/unordered_map.hpp>

#include <boost/serialization/split_member.hpp>
#include <boost/serialization/version.hpp>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <gtest/gtest.h>

#include "object_base.hpp"

namespace collections
{
}
