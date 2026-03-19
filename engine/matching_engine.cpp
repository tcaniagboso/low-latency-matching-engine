#include "matching_engine.hpp"

namespace engine {

    MatchingEngine::MatchingEngine()
            : order_book_{},
              fulfilled_{} {}

    void MatchingEngine::match(
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
                    types::Quantity traded = consume_level(order_book_, quantity, queue);

                    if (traded > 0) {
                        fulfilled_.emplace_back(trade_id_gen_.next(), type, symbol, traded, level_price);
                    }

                    if (queue.empty()) {
                        it = order_book_.remove_asks_level(symbol, it);
                    }
                }
            } else {
                while (it != end && quantity > 0 && it->first <= price) {
                    auto &[level_price, queue] = *it;
                    types::Quantity traded = consume_level(order_book_, quantity, queue);

                    if (traded > 0) {
                        fulfilled_.emplace_back(trade_id_gen_.next(), type, symbol, traded, level_price);
                    }

                    if (queue.empty()) {
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
                    types::Quantity traded = consume_level(order_book_, quantity, queue);

                    if (traded > 0) {
                        fulfilled_.emplace_back(trade_id_gen_.next(), type, symbol, traded, level_price);
                    }

                    if (queue.empty()) {
                        it = order_book_.remove_bids_level(symbol, it);
                    }

                    if (quantity == 0) break;
                }
            } else {
                while (it != end && quantity > 0 && it->first >= price) {
                    auto &[level_price, queue] = *it;
                    types::Quantity traded = consume_level(order_book_, quantity, queue);

                    if (traded > 0) {
                        fulfilled_.emplace_back(trade_id_gen_.next(), type, symbol, traded, level_price);
                    }

                    if (queue.empty()) {
                        it = order_book_.remove_bids_level(symbol, it);
                    }
                }

                if (quantity > 0) {
                    order_book_.add_limit_sell(symbol, order_id_gen_.next(), price, quantity);
                }
            }
        }
    }

    void MatchingEngine::limit_buy(types::Symbol symbol, types::PriceT price, types::Quantity quantity) {
        match(types::OrderType::LIMIT, symbol, price, quantity, types::Side::BUY);
    }

    void MatchingEngine::limit_sell(types::Symbol symbol, types::PriceT price, types::Quantity quantity) {
        match(types::OrderType::LIMIT, symbol, price, quantity, types::Side::SELL);
    }

    void MatchingEngine::market_buy(types::Symbol symbol, types::Quantity quantity) {
        match(types::OrderType::MARKET, symbol, 0, quantity, types::Side::BUY);
    }

    void MatchingEngine::market_sell(types::Symbol symbol, types::Quantity quantity) {
        match(types::OrderType::MARKET, symbol, 0, quantity, types::Side::SELL);
    }

    bool MatchingEngine::cancel(types::OrderId id) {
        return order_book_.cancel(id);
    }
} // namespace engine