#pragma once

#include <chrono>
#include <cstdint>

namespace types {
    enum class Side {BUY, SELL};

    enum class OrderType{LIMIT, MARKET};

    using Symbol = uint32_t;

    using TradeId = uint64_t;
    using OrderId = uint64_t;
    using Quantity = uint64_t;
    using PriceT = int64_t;

    using TimePt = std::chrono::system_clock::time_point;
} // namespace types