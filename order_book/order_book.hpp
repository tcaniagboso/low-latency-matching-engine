#pragma once

#include <functional>
#include <list>
#include <map>
#include <optional>
#include <unordered_map>
#include <vector>

#include "../types/types.hpp"

namespace order_book {
    struct RestingOrder {
        types::OrderId id_;
        types::Quantity remaining_;
        types::TimePt timestamp_;

        RestingOrder(types::OrderId id, types::Quantity remaining, typesTimePt timestamp)
                : id_{id},
                  remaining_{remaining},
                  timestamp_{timestamp} {}
    };

    using Queue = std::list<RestingOrder>;

    struct Locator {
        types::Symbol symbol_;
        types::Side side_;
        types::PriceT price_;
        Queue::iterator position_;

        Locator(types::Symbol symbol, types::Side side, types::PriceT price, Queue::iterator position)
                : symbol_{symbol},
                  side_{side},
                  price_{price},
                  position_{position} {}
    };

    class OrderBook {
    public:
        OrderBook() = default;

        // inserts
        virtual void
        add_limit_buy(types::Symbol symbol, types::OrderId id, types::PriceT price, types::Quantity quantity) = 0;

        virtual void
        add_limit_sell(types::Symbol symbol, types::OrderId id, types::PriceT price, types::Quantity quantity) = 0;

        // cancels
        virtual bool cancel(types::OrderId id) = 0;

        // Queries
        virtual std::map<types::PriceT, Queue>::iterator asks_begin(types::Symbol symbol) = 0;

        virtual std::map<types::PriceT, Queue>::iterator asks_end(types::Symbol symbol) = 0;

        virtual std::map<types::PriceT, Queue>::iterator bids_begin(types::Symbol symbol) = 0;

        virtual std::map<types::PriceT, Queue>::iterator bids_end(types::Symbol symbol) = 0;
    };
} // namespace order_book
