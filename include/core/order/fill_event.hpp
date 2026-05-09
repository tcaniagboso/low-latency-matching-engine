#pragma once

#include "types.hpp"
#include "../time_utils/time.hpp"

namespace core::order {
    struct FillEvent {
        OrderId buy_order_id_{};
        OrderId sell_order_id_{};
        Quantity quantity_{};
        PriceT price_{};
        TimePt timestamp_{core::time_utils::now()};

        explicit FillEvent() = default;

        FillEvent(OrderId buy_order_id, OrderId sell_order_id, Quantity quantity, PriceT price)
                : buy_order_id_{buy_order_id},
                  sell_order_id_{sell_order_id},
                  quantity_{quantity},
                  price_{price},
                  timestamp_{core::time_utils::now()} {}
    };
} // namespace core::order