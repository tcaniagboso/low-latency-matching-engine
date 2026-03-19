#pragma once

#include <cstdint>
#include <optional>

#include "../core/time.hpp"
#include "../core/types.hpp"

namespace gateway {

    struct NewOrder {
        types::OrderId id_;
        types::Symbol symbol_;
        types::Side side_;
        types::OrderType type_;
        types::Quantity quantity_;
        std::optional<types::PriceT> price_;
        types::TimePt timestamp_{time_utils::now()};

        // Constructor for Market Order
        NewOrder(uint64_t id, uint32_t symbol, uint8_t side, uint8_t type, uint64_t quantity)
                : id_{id},
                  symbol_{symbol},
                  side_{side},
                  type_{type},
                  quantity_{quantity} {}

        // Constructor for LIMIT order
        NewOrder(uint64_t id, uint32_t symbol, uint8_t side, uint8_t type, uint64_t quantity, int64_t price)
                : NewOrder(id, symbol, side, type, quantity) {
            price_ = price;
        }
    };

    struct CancelOrder {
        types::OrderId id_;
        types::TimePt timestamp_{time_utils::now()};

        explicit CancelOrder(uint64_t id) : id_{id} {}
    };
} // namespace gateway
