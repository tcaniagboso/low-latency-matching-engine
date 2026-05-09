#pragma once

#include <cstdint>
#include <optional>

#include "types.hpp"
#include "../core/time_utils/time.hpp"
#include "../core/types/types.hpp"

namespace gateway {
    struct NewOrderRequest {
        OrderId order_id_;
        Quantity quantity_;
        std::optional<PriceT> price_;
        SymbolId symbol_id_;
        OrderType type_;
        Side side_;

        // Constructor for Market Order
        NewOrderRequest(OrderId order_id, Quantity quantity, SymbolId symbol_id, OrderType type, Side side)
                : order_id_{order_id},
                  quantity_{quantity},
                  symbol_id_{symbol_id},
                  type_{type},
                  side_{side}{}

        // Constructor for LIMIT order
        NewOrderRequest(OrderId order_id, Quantity quantity, PriceT price, SymbolId symbol_id, OrderType type, Side side)
                : NewOrderRequest(order_id, quantity, symbol_id, type, side) {
            price_ = price;
        }
    };

    struct CancelOrderRequest {
        OrderId id_;

        explicit CancelOrderRequest(OrderId id) : id_{id} {}
    };
} // namespace gateway
