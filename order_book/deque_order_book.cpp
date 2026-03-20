#include "deque_order_book.hpp"
#include "../core/time.hpp"

namespace order_book {
    DequeOrderBook::AskMap DequeOrderBook::empty_asks_;
    DequeOrderBook::BidMap DequeOrderBook::empty_bids_;

    DequeOrderBook::DequeOrderBook()
            : live_orders_{},
              asks_{},
              bids_{} {}

//    void DequeOrderBook::add_order(
//            types::Symbol symbol,
//            types::OrderId id,
//            types::PriceT price,
//            types::Quantity quantity,
//            types::Side side
//            ) {
//
//        types::TimePt timestamp = time_utils::now();
//        DequeOrderBook::Queue* queue = nullptr;
//
//        if (side == types::Side::BUY) {
//            queue = &bids_[symbol][price];
//        } else {
//            queue = &asks_[symbol][price];
//        }
//
//        queue->emplace_back(id, quantity, timestamp);
//        live_orders_.emplace(id, &queue->back());
//    }

    void DequeOrderBook::add_limit_buy(
            types::Symbol symbol,
            types::OrderId id,
            types::PriceT price,
            types::Quantity quantity
    ) {

        auto timestamp = time_utils::now();
        auto &queue = bids_[symbol][price];
        queue.emplace_back(id, quantity, timestamp);
        live_orders_.emplace(id, &queue.back());
    }

    void DequeOrderBook::add_limit_sell(
            types::Symbol symbol,
            types::OrderId id,
            types::PriceT price,
            types::Quantity quantity
    ) {
        auto timestamp = time_utils::now();
        auto &queue = asks_[symbol][price];
        queue.emplace_back(id, quantity, timestamp);
        live_orders_.emplace(id, &queue.back());
    }

    bool DequeOrderBook::cancel(types::OrderId id) {
        auto it = live_orders_.find(id);
        if (it == live_orders_.end()) return false;
        it->second->active_ = false;
        live_orders_.erase(it);
        return true;
    }

    DequeOrderBook::AskMap::iterator DequeOrderBook::remove_asks_level(
            types::Symbol symbol,
            DequeOrderBook::AskMap::iterator it
    ) {
        return asks_[symbol].erase(it);
    }

    DequeOrderBook::BidMap::iterator DequeOrderBook::remove_bids_level(
            types::Symbol symbol,
            DequeOrderBook::BidMap::iterator it
    ) {
        return bids_[symbol].erase(it);
    }

    bool DequeOrderBook::level_empty(order_book::DequeOrderBook::Queue &queue) {
        return queue.empty();
    }

    DequeOrderBook::AskMap::iterator DequeOrderBook::asks_begin(types::Symbol symbol) {
        auto it = asks_.find(symbol);
        if (it == asks_.end()) return empty_asks_.end();
        return it->second.begin();
    }

    DequeOrderBook::AskMap::iterator DequeOrderBook::asks_end(types::Symbol symbol) {
        auto it = asks_.find(symbol);
        if (it == asks_.end()) return empty_asks_.end();
        return it->second.end();
    }

    DequeOrderBook::BidMap::iterator DequeOrderBook::bids_begin(types::Symbol symbol) {
        auto it = bids_.find(symbol);
        if (it == bids_.end()) return empty_bids_.end();
        return it->second.begin();
    }

    DequeOrderBook::BidMap::iterator DequeOrderBook::bids_end(types::Symbol symbol) {
        auto it = bids_.find(symbol);
        if (it == bids_.end()) return empty_bids_.end();
        return it->second.end();
    }

    types::Quantity DequeOrderBook::consume_level(
            types::Quantity &quantity,
            DequeOrderBook::Queue &queue
    ) {
        types::Quantity traded{0};
        while (quantity > 0 && !queue.empty()) {
            auto& order = queue.front();
            if (!order.active_) {
                queue.pop_front();
            } else {
                auto min_qty = (quantity < order.remaining_)? quantity : order.remaining_;
                quantity -= min_qty;
                order.remaining_ -= min_qty;
                traded += min_qty;

                if (order.remaining_ == 0) {
                    live_orders_.erase(order.id_);
                    queue.pop_front();
                }
            }
        }

        return traded;
    }


}