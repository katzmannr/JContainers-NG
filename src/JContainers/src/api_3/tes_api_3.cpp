#include <sys/types.h>
#include <sys/stat.h>

#include "util/to_underlying.hpp"
#include "RE/Skyrim.h"

#include <boost/optional.hpp>
#include <assert.h>
#include "collections/bind_traits.h"
#include "reflection/detail/type_traits.hpp"
#include "tes_object.h"
#include "jcontainers_constants.h"
#include "tes_form_db.h"
#include "tes_jcontainers.h"
// Required for string.h
#include <filesystem>
#include <gtest/gtest.h>
#include "common/IPrefix.h"
#include <shlobj.h>

namespace collections {

}

bool tes_api_3::log_api_calls = true;