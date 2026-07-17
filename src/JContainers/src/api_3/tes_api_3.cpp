#include <sys/types.h>
#include <sys/stat.h>
#include <gtest/gtest.h>

#include <boost/optional.hpp>
#include <boost/filesystem.hpp>
#include "collections/bind_traits.h"
#include "reflection/detail/type_traits.hpp"
#include "tes_object.h"
#include "tes_form_db.h"
#include "tes_jcontainers.h"
// Required for string.h
#include "RE/Skyrim.h"
#include <shlobj.h>

namespace collections {

}

bool tes_api_3::log_api_calls = false;