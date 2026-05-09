#pragma once

#include <vector>

#include "instrument_config.hpp"
#include "symbol_table.hpp"

namespace instruments {

    struct InstrumentData {
        std::vector<InstrumentConfig> configs_;
        SymbolTable symbol_table_;
    };
}
