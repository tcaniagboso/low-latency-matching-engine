#pragma once

#include <list>
#include <map>
#include <unordered_map>

#include "../core/types.hpp"
#include "../core/resting_order.hpp"

namespace order_book {
    // Price Time Priority Order Book
    class ListOrderBook {
        using Queue = std::list<core::RestingOrder>;
        using AskMap = std::map<types::PriceT, ListOrderBook::Queue, std::less<>>;
        using BidMap = std::map<types::PriceT, ListOrderBook::Queue, std::greater<>>;

    private:
        struct Locator {
            types::Symbol symbol_;
            types::Side side_;
            types::PriceT price_;
            ListOrderBook::Queue::iterator position_;

            Locator(types::Symbol symbol, types::Side side, types::PriceT price, Queue::iterator position)
                    : symbol_{symbol},
                      side_{side},
                      price_{price},
                      position_{position} {}
        };

        std::unordered_map<types::OrderId, Locator> live_orders_;
        std::unordered_map<types::Symbol, ListOrderBook::AskMap> asks_;
        std::unordered_map<types::Symbol, ListOrderBook::BidMap> bids_;
        static ListOrderBook::AskMap empty_asks_;
        static ListOrderBook::BidMap empty_bids_;

//        void erase_locator(types::OrderId id);

//        void add_order(
//                types::Symbol symbol,
//                types::OrderId id,
//                types::PriceT price,
//                types::Quantity quantity,
//                types::Side side
//        );

    public:
        ListOrderBook();

        // inserts
        void add_limit_buy(
                types::Symbol symbol,
                types::OrderId id,
                types::PriceT price,
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

        ListOrderBook::AskMap::iterator remove_asks_level(
                types::Symbol symbol,
                ListOrderBook::AskMap::iterator it
        );

        ListOrderBook::BidMap::iterator remove_bids_level(
                types::Symbol symbol,
                ListOrderBook::BidMap::iterator it
        );

        // Queries
//        bool has_asks(types::Symbol symbol) const;
//
//        bool has_bids(types::Symbol symbol) const;

        bool level_empty(Queue &queue);

        ListOrderBook::AskMap::iterator asks_begin(types::Symbol symbol);

        ListOrderBook::AskMap::iterator asks_end(types::Symbol symbol);

        ListOrderBook::BidMap::iterator bids_begin(types::Symbol symbol);

        ListOrderBook::BidMap::iterator bids_end(types::Symbol symbol);

        types::Quantity consume_level(
                types::Quantity &quantity,
                ListOrderBook::Queue &queue
        );
    };
} // namespace order_book
