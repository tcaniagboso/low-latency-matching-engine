#pragma once

#include <iostream>

#include "types.hpp"
#include "../time_utils/time.hpp"

namespace core::order {
    struct Trade {
        TradeId id_;
        OrderId buy_order_id_;
        OrderId sell_order_id_;
        Quantity quantity_;
        PriceT price_;
        SymbolId symbol_id_;
        TimePt timestamp_;

        Trade(TradeId id, OrderId buy_order_id, OrderId sell_order_id, Quantity quantity,
              PriceT price, SymbolId symbol_id)
                : id_{id},
                  buy_order_id_{buy_order_id},
                  sell_order_id_{sell_order_id},
                  quantity_{quantity},
                  price_{price},
                  symbol_id_{symbol_id},
                  timestamp_{core::time_utils::now()} {}

        void print() const {
            std::cout << "Trade ID: " << id_ << '\n';
            std::cout << "Symbol ID: " << symbol_id_ << '\n';
            std::cout << "Buy Order ID: " << buy_order_id_ << '\n';
            std::cout << "Sell Order ID: " << sell_order_id_ << '\n';
            std::cout << "Price: " << price_ << '\n';
            std::cout << "Quantity: " << quantity_ << '\n';
            std::cout << "Timestamp: " << core::time_utils::time_point_to_string(timestamp_) << "\n";
        }

    };
} // namespace core::order