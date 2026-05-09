#pragma once

#include <chrono>
#include <cstdint>

namespace core::types {
    enum class Side : uint8_t {
        BUY,
        SELL
    };

    enum class OrderType : uint8_t{
        LIMIT,
        MARKET
    };

    using SymbolId = uint32_t;

    using TradeId = uint64_t;
    using OrderId = uint64_t;
    using Quantity = uint64_t;
    using PriceT = int64_t;

    using TimePt = std::chrono::system_clock::time_point;
} // namespace core::types