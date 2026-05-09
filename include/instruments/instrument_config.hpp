#pragma once

#include "types.hpp"

namespace instruments {

    struct InstrumentConfig {
        PriceT min_price_;
        PriceT max_price_;
        PriceT tick_size_units_;
    };
}
