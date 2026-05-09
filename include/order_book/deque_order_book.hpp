#pragma once

#include <deque>
#include <functional>
#include <map>
#include <unordered_map>
#include <vector>

#include "types.hpp"

namespace order_book {

    // Price Time Priority Order Book (std::deque)
    class DequeOrderBook {
    public:
        using Queue = std::deque<RestingOrder>;

        struct Level {
            Queue queue_;
        };

        using AskMap = std::map<PriceT, Level, std::less<>>;
        using BidMap = std::map<PriceT, Level, std::greater<>>;

        explicit DequeOrderBook(const InstrumentConfig &);

        void add_limit_buy(OrderId order_id, PriceT price, Quantity quantity);

        void add_limit_sell(OrderId order_id, PriceT price, Quantity quantity);

        bool cancel(OrderId order_id);

        AskMap::iterator remove_asks_level(AskMap::iterator it);

        BidMap::iterator remove_bids_level(BidMap::iterator it);

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
                std::vector<FillEvent> &fills
        );

    private:
        std::unordered_map<OrderId, RestingOrder *> live_orders_;
        AskMap asks_;
        BidMap bids_;
    };

} // namespace order_book