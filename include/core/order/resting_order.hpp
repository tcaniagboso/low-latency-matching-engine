#pragma once

#include "types.hpp"

namespace core::order {
    struct RestingOrder {
        OrderId id_;
        Quantity remaining_;
        TimePt timestamp_;
        bool active_;

        RestingOrder(OrderId id, Quantity remaining, TimePt timestamp)
                : id_{id},
                  remaining_{remaining},
                  timestamp_{timestamp},
                  active_{true} {}
    };
} // namespace core::order