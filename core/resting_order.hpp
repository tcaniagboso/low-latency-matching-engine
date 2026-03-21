#pragma once

#include "types.hpp"

namespace core {
    struct RestingOrder {
        types::OrderId id_;
        types::Quantity remaining_;
        types::TimePt timestamp_;
        bool active_;

        RestingOrder(types::OrderId id, types::Quantity remaining, types::TimePt timestamp)
                : id_{id},
                  remaining_{remaining},
                  timestamp_{timestamp},
                  active_{true} {}
    };
}