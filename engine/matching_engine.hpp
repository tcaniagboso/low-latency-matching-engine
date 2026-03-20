#pragma once

#include <iostream>
#include <vector>

#include "../core/id_generator.hpp"
#include "../core/types.hpp"
#include "../core/time.hpp"

namespace engine {

    struct Trade {
        types::TradeId id_;
        types::OrderType type_;
        types::Symbol symbol_;
        types::Quantity quantity_;
        types::PriceT price_;
        types::TimePt timestamp_;

        Trade(types::TradeId id, types::OrderType type, types::Symbol symbol, types::Quantity quantity,
              types::PriceT price)
                : id_{id},
                  type_{type},
                  symbol_{symbol},
                  quantity_{quantity},
                  price_{price},
                  timestamp_{time_utils::now()} {}

        void print() const {
            std::cout << "ID: " << id_ << '\n';
            std::cout << "Type: " << ((type_ == types::OrderType::MARKET) ? "Market" : "Limit") << '\n';
            std::cout << "Symbol: " << symbol_ << '\n';
            std::cout << "Price: " << price_ << '\n';
            std::cout << "Quantity: " << quantity_ << '\n';
            std::cout << "Timestamp: " << time_utils::time_point_to_string(timestamp_) << "\n";
        }

    };

    template<typename OrderBookT>
    class MatchingEngine {

    private:
        id::OrderIdGenerator order_id_gen_;
        id::TradeIdGenerator trade_id_gen_;
        OrderBookT order_book_;
        std::vector<Trade> fulfilled_;

        void match(types::OrderType type, types::Symbol symbol, types::PriceT price, types::Quantity quantity,
                   types::Side side);

    public:
        MatchingEngine();

        void limit_buy(types::Symbol symbol, types::PriceT price, types::Quantity quantity);

        void limit_sell(types::Symbol symbol, types::PriceT price, types::Quantity quantity);

        void market_buy(types::Symbol symbol, types::Quantity quantity);

        void market_sell(types::Symbol symbol, types::Quantity quantity);

        bool cancel(types::OrderId id);

        void print_trades();
    };

    template<typename OrderBookT>
    MatchingEngine<OrderBookT>::MatchingEngine()
            : order_book_{},
              fulfilled_{} {}

    template<typename OrderBookT>
    void MatchingEngine<OrderBookT>::match(
            types::OrderType type,
            types::Symbol symbol,
            types::PriceT price,
            types::Quantity quantity,
            types::Side side
    ) {
        if (quantity == 0) return;

        bool is_market = (type == types::OrderType::MARKET);
        bool is_buy = (side == types::Side::BUY);

        if (is_buy) {
            auto it = order_book_.asks_begin(symbol);
            auto end = order_book_.asks_end(symbol);

            if (is_market) {
                while (it != end && quantity > 0) {
                    auto &[level_price, queue] = *it;
                    types::Quantity traded = order_book_.consume_level(quantity, queue);

                    if (traded > 0) {
                        fulfilled_.emplace_back(trade_id_gen_.next(), type, symbol, traded, level_price);
                    }

                    if (order_book_.level_empty(queue)) {
                        it = order_book_.remove_asks_level(symbol, it);
                    }
                }
            } else {
                while (it != end && quantity > 0 && it->first <= price) {
                    auto &[level_price, queue] = *it;
                    types::Quantity traded = order_book_.consume_level(quantity, queue);

                    if (traded > 0) {
                        fulfilled_.emplace_back(trade_id_gen_.next(), type, symbol, traded, level_price);
                    }

                    if (order_book_.level_empty(queue)) {
                        it = order_book_.remove_asks_level(symbol, it);
                    }
                }

                if (quantity > 0) {
                    order_book_.add_limit_buy(symbol, order_id_gen_.next(), price, quantity);
                }
            }
        } else {
            auto it = order_book_.bids_begin(symbol);
            auto end = order_book_.bids_end(symbol);

            if (is_market) {
                while (it != end && quantity > 0) {
                    auto &[level_price, queue] = *it;
                    types::Quantity traded = order_book_.consume_level(quantity, queue);

                    if (traded > 0) {
                        fulfilled_.emplace_back(trade_id_gen_.next(), type, symbol, traded, level_price);
                    }

                    if (order_book_.level_empty(queue)) {
                        it = order_book_.remove_bids_level(symbol, it);
                    }
                }
            } else {
                while (it != end && quantity > 0 && it->first >= price) {
                    auto &[level_price, queue] = *it;
                    types::Quantity traded = order_book_.consume_level(quantity, queue);

                    if (traded > 0) {
                        fulfilled_.emplace_back(trade_id_gen_.next(), type, symbol, traded, level_price);
                    }

                    if (order_book_.level_empty(queue)) {
                        it = order_book_.remove_bids_level(symbol, it);
                    }
                }

                if (quantity > 0) {
                    order_book_.add_limit_sell(symbol, order_id_gen_.next(), price, quantity);
                }
            }
        }
    }

    template<typename OrderBookT>
    void MatchingEngine<OrderBookT>::limit_buy(types::Symbol symbol, types::PriceT price, types::Quantity quantity) {
        match(types::OrderType::LIMIT, symbol, price, quantity, types::Side::BUY);
    }

    template<typename OrderBookT>
    void MatchingEngine<OrderBookT>::limit_sell(types::Symbol symbol, types::PriceT price, types::Quantity quantity) {
        match(types::OrderType::LIMIT, symbol, price, quantity, types::Side::SELL);
    }

    template<typename OrderBookT>
    void MatchingEngine<OrderBookT>::market_buy(types::Symbol symbol, types::Quantity quantity) {
        match(types::OrderType::MARKET, symbol, 0, quantity, types::Side::BUY);
    }

    template<typename OrderBookT>
    void MatchingEngine<OrderBookT>::market_sell(types::Symbol symbol, types::Quantity quantity) {
        match(types::OrderType::MARKET, symbol, 0, quantity, types::Side::SELL);
    }

    template<typename OrderBookT>
    bool MatchingEngine<OrderBookT>::cancel(types::OrderId id) {
        return order_book_.cancel(id);
    }

    template<typename OrderBookT>
    void MatchingEngine<OrderBookT>::print_trades() {
        size_t n = fulfilled_.size();

        std::cout << "TRADE SUMMARY:" << "\n\n";

        for (size_t i = 0; i < n; i++) {
            fulfilled_[i].print();
            if (i < n - 1) std::cout << '\n';
        }

    }
} // namespace engine