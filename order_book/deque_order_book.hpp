#pragma once

#include <deque>
#include <map>
#include <unordered_map>

#include "../core/types.hpp"
#include "../core/resting_order.hpp"

namespace order_book {

    class DequeOrderBook {
        using Queue = std::deque<core::RestingOrder>;
        using AskMap = std::map<types::PriceT, DequeOrderBook::Queue, std::less<>>;
        using BidMap = std::map<types::PriceT, DequeOrderBook::Queue, std::greater<>>;

    private:
        std::unordered_map<types::OrderId, core::RestingOrder*> live_orders_;
        std::unordered_map<types::Symbol, DequeOrderBook::AskMap> asks_;
        std::unordered_map<types::Symbol, DequeOrderBook::BidMap> bids_;
        static DequeOrderBook::AskMap empty_asks_;
        static DequeOrderBook::BidMap empty_bids_;

//        void erase_ptr(types::OrderId id);

//        void add_order(
//                types::Symbol symbol,
//                types::OrderId id,
//                types::PriceT price,
//                types::Quantity quantity,
//                types::Side side
//        );

    public:
        DequeOrderBook();

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

        DequeOrderBook::AskMap::iterator remove_asks_level(
                types::Symbol symbol,
                DequeOrderBook::AskMap::iterator it
        );

        DequeOrderBook::BidMap::iterator remove_bids_level(
                types::Symbol symbol,
                DequeOrderBook::BidMap::iterator it
        );

        // Queries
//        bool has_asks(types::Symbol symbol) const;
//
//        bool has_bids(types::Symbol symbol) const;

        bool level_empty(Queue &queue);

        DequeOrderBook::AskMap::iterator asks_begin(types::Symbol symbol);

        DequeOrderBook::AskMap::iterator asks_end(types::Symbol symbol);

        DequeOrderBook::BidMap::iterator bids_begin(types::Symbol symbol);

        DequeOrderBook::BidMap::iterator bids_end(types::Symbol symbol);

        types::Quantity consume_level(
                types::Quantity &quantity,
                DequeOrderBook::Queue &queue
        );
    };
}