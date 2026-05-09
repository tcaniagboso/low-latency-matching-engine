#pragma once

#include <iostream>
#include <optional>

#include "types.hpp"

namespace engine {

    template<typename OrderBookT>
    class MatchingEngine {
    public:
        explicit MatchingEngine() = default;

        MatchResult limit_buy(OrderBookT &book, OrderId order_id, PriceT price, Quantity quantity) const;

        MatchResult limit_sell(OrderBookT &book, OrderId order_id, PriceT price, Quantity quantity) const;

        MatchResult market_buy(OrderBookT &book, OrderId order_id, Quantity quantity) const;

        MatchResult market_sell(OrderBookT &book, OrderId order_id, Quantity quantity) const;

        static bool cancel(OrderBookT &book, OrderId order_id);

    private:
        static MatchResult
        match_buy(OrderBookT &book, OrderType type, OrderId order_id, PriceT price, Quantity quantity);

        static MatchResult
        match_sell(OrderBookT &book, OrderType type, OrderId order_id, PriceT price, Quantity quantity);
    };

    template<typename OrderBookT>
    MatchResult MatchingEngine<OrderBookT>::match_buy(
            OrderBookT &book,
            OrderType type,
            OrderId order_id,
            PriceT price,
            Quantity quantity
    ) {
        if (quantity == 0) return {};

        bool is_market = (type == OrderType::MARKET);
        constexpr Side side{Side::BUY};

        std::vector<FillEvent> fills{};

        auto it = book.asks_begin();
        if (is_market) {
            while (it != book.asks_end() && quantity > 0) {
                auto &[level_price, level] = *it;
                book.consume_level(order_id, side, level_price, quantity, level, fills);

                if (book.level_empty(level)) {
                    it = book.remove_asks_level(it);
                }
            }
        } else {
            while (it != book.asks_end() && quantity > 0 && it->first <= price) {
                auto &[level_price, level] = *it;
                book.consume_level(order_id, side, level_price, quantity, level, fills);

                if (book.level_empty(level)) {
                    it = book.remove_asks_level(it);
                }
            }
        }

        return {quantity, fills};
    }

    template<typename OrderBookT>
    MatchResult MatchingEngine<OrderBookT>::match_sell(
            OrderBookT &book,
            OrderType type,
            OrderId order_id,
            PriceT price,
            Quantity quantity
    ) {
        if (quantity == 0) return {};

        bool is_market = (type == OrderType::MARKET);
        constexpr Side side{Side::SELL};

        std::vector<FillEvent> fills{};

        auto it = book.bids_begin();

        if (is_market) {
            while (it != book.bids_end() && quantity > 0) {
                auto &[level_price, level] = *it;
                book.consume_level(order_id, side, level_price, quantity, level, fills);

                if (book.level_empty(level)) {
                    it = book.remove_bids_level(it);
                }
            }
        } else {
            while (it != book.bids_end() && quantity > 0 && it->first >= price) {
                auto &[level_price, level] = *it;
                book.consume_level(order_id, side, level_price, quantity, level, fills);

                if (book.level_empty(level)) {
                    it = book.remove_bids_level(it);
                }
            }
        }

        return {quantity, fills};
    }

    template<typename OrderBookT>
    MatchResult
    MatchingEngine<OrderBookT>::limit_buy(OrderBookT &book, OrderId order_id, PriceT price, Quantity quantity) const {
        return match_buy(book, OrderType::LIMIT, order_id, price, quantity);
    }

    template<typename OrderBookT>
    MatchResult
    MatchingEngine<OrderBookT>::limit_sell(OrderBookT &book, OrderId order_id, PriceT price, Quantity quantity) const {
        return match_sell(book, OrderType::LIMIT, order_id, price, quantity);
    }

    template<typename OrderBookT>
    MatchResult MatchingEngine<OrderBookT>::market_buy(OrderBookT &book, OrderId order_id, Quantity quantity) const {
        return match_buy(book, OrderType::MARKET, order_id, PriceT{}, quantity);
    }

    template<typename OrderBookT>
    MatchResult MatchingEngine<OrderBookT>::market_sell(OrderBookT &book, OrderId order_id, Quantity quantity) const {
        return match_sell(book, OrderType::MARKET, order_id, PriceT{}, quantity);
    }

    template<typename OrderBookT>
    bool MatchingEngine<OrderBookT>::cancel(OrderBookT &book, OrderId order_id) {
        return book.cancel(order_id);
    }
} // namespace engine