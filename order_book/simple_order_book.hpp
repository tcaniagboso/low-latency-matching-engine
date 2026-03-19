#pragma once

#include <list>
#include <map>
#include <unordered_map>

#include "../core/types.hpp"

namespace order_book {
    struct RestingOrder {
        types::OrderId id_;
        types::Quantity remaining_;
        types::TimePt timestamp_;

        RestingOrder(types::OrderId id, types::Quantity remaining, types::TimePt timestamp)
                : id_{id},
                  remaining_{remaining},
                  timestamp_{timestamp} {}
    };

    using Queue = std::list<RestingOrder>;
    using AskMap = std::map<types::PriceT, Queue, std::less<>>;
    using BidMap = std::map<types::PriceT, Queue, std::greater<>>;

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

    // Price Time Priority Order Book
    class SimpleOrderBook {
    private:
        std::unordered_map<types::OrderId, Locator> live_orders_;
        std::unordered_map<types::Symbol, std::map<types::PriceT, Queue, std::less<>>> asks_;
        std::unordered_map<types::Symbol, std::map<types::PriceT, Queue, std::greater<>>> bids_;
        static AskMap empty_asks_;
        static BidMap empty_bids_;

        void add_order(types::Symbol symbol, types::OrderId id, types::PriceT price,
                       types::Quantity quantity, types::Side side
        );

    public:
        SimpleOrderBook();

        // inserts
        void add_limit_buy(
                types::Symbol symbol,
                types::OrderId id, types::PriceT price,
                types::Quantity quantity
        );

        void add_limit_sell(
                types::Symbol symbol,
                types::OrderId id,
                types::PriceT price,
                types::Quantity quantity
        );

        // cancels
        bool cancel(types::OrderId id);

        void erase_locator(types::OrderId id);

        AskMap::iterator remove_asks_level(types::Symbol symbol, AskMap::iterator it);

        BidMap::iterator remove_bids_level(types::Symbol symbol, BidMap::iterator it);

        // Queries
        bool has_asks(types::Symbol symbol) const;

        bool has_bids(types::Symbol symbol) const;

        AskMap::iterator asks_begin(types::Symbol symbol);

        AskMap::iterator asks_end(types::Symbol symbol);

        BidMap::iterator bids_begin(types::Symbol symbol);

        BidMap::iterator bids_end(types::Symbol symbol);
    };
} // namespace order_book
