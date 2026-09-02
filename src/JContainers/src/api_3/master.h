#pragma once

#include "reflection/reflection.h"

namespace tes_api_3 {

    template<class T>
    class class_meta : public reflection::class_meta_mixin_t < T > {
    public:
        class_meta() {
            this->metaInfo.version = (uint32_t)collections::consts::api_version;
        }
    };
}
