#pragma once

#include <list>
#include <map>
#include <unordered_map>
#include <vector>

#include "types.hpp"

namespace order_book {

    // Price Time Priority Order Book (std::list)
    class ListOrderBook {
    public:
        using Queue = std::list<RestingOrder>;

        struct Level {
            Queue queue_;
        };

        using AskMap = std::map<PriceT, Level, std::less<>>;
        using BidMap = std::map<PriceT, Level, std::greater<>>;

        explicit ListOrderBook(const InstrumentConfig &);

        // inserts
        void add_limit_buy(OrderId order_id, PriceT price, Quantity quantity);

        void add_limit_sell(OrderId order_id, PriceT price, Quantity quantity);

        // cancels
        bool cancel(OrderId order_id);

        ListOrderBook::AskMap::iterator remove_asks_level(AskMap::iterator it);

        ListOrderBook::BidMap::iterator remove_bids_level(BidMap::iterator it);

        AskMap::iterator asks_begin();

        AskMap::iterator asks_end();

        BidMap::iterator bids_begin();

        BidMap::iterator bids_end();

        static bool level_empty(const Level &level);

        void consume_level(
                OrderId incoming_order_id,
                Side incoming_side,
                PriceT execution_price,
                Quantity &quantity,
                Level &level,
                std::vector<FillEvent> &fills);

    private:
        struct Locator {
            PriceT price_;
            Side side_;
            Queue::iterator position_;

            Locator(PriceT price, Side side, Queue::iterator position)
                    : price_{price},
                      side_{side},
                      position_{position} {}
        };

        std::unordered_map<OrderId, Locator> live_orders_;
        AskMap asks_;
        BidMap bids_;
    };
} // namespace order_book
