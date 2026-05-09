#include "../../include/order_book/deque_order_book.hpp"

namespace order_book {

    DequeOrderBook::DequeOrderBook(const instruments::InstrumentConfig &)
            : live_orders_{},
              asks_{},
              bids_{} {}

    void DequeOrderBook::add_limit_buy(OrderId order_id, PriceT price, Quantity quantity) {

        auto timestamp = core::time_utils::now();
        auto &queue = bids_[price].queue_;
        queue.emplace_back(order_id, quantity, timestamp);
        live_orders_.emplace(order_id, &queue.back());
    }

    void DequeOrderBook::add_limit_sell(OrderId order_id, PriceT price, Quantity quantity) {
        auto timestamp = core::time_utils::now();
        auto &queue = asks_[price].queue_;
        queue.emplace_back(order_id, quantity, timestamp);
        live_orders_.emplace(order_id, &queue.back());
    }

    bool DequeOrderBook::cancel(OrderId order_id) {
        auto it = live_orders_.find(order_id);
        if (it == live_orders_.end()) return false;
        it->second->active_ = false;
        live_orders_.erase(it);
        return true;
    }

    DequeOrderBook::AskMap::iterator DequeOrderBook::remove_asks_level(
            DequeOrderBook::AskMap::iterator it
    ) {
        return asks_.erase(it);
    }

    DequeOrderBook::BidMap::iterator DequeOrderBook::remove_bids_level(DequeOrderBook::BidMap::iterator it) {
        return bids_.erase(it);
    }

    DequeOrderBook::AskMap::iterator DequeOrderBook::asks_begin() {
        return asks_.begin();
    }

    DequeOrderBook::AskMap::iterator DequeOrderBook::asks_end() {
        return asks_.end();
    }

    DequeOrderBook::BidMap::iterator DequeOrderBook::bids_begin() {
        return bids_.begin();
    }

    DequeOrderBook::BidMap::iterator DequeOrderBook::bids_end() {
        return bids_.end();
    }

    bool DequeOrderBook::level_empty(const DequeOrderBook::Level &level) {
        return level.queue_.empty();
    }

    void DequeOrderBook::consume_level(
            OrderId incoming_order_id,
            Side incoming_side,
            PriceT execution_price,
            Quantity &quantity,
            Level &level,
            std::vector<FillEvent> &fills) {

        auto &queue = level.queue_;
        while (quantity > 0 && !queue.empty()) {
            auto &resting = queue.front();
            if (!resting.active_) {
                queue.pop_front();
            } else {
                auto match_qty = (quantity < resting.remaining_) ? quantity : resting.remaining_;
                quantity -= match_qty;
                resting.remaining_ -= match_qty;

                FillEvent fill{};
                fill.quantity_ = match_qty;
                fill.price_ = execution_price;

                switch(incoming_side) {
                    case Side::BUY:
                        fill.buy_order_id_ = incoming_order_id;
                        fill.sell_order_id_ = resting.id_;
                        break;
                    case Side::SELL:
                        fill.buy_order_id_ = resting.id_;
                        fill.sell_order_id_ = incoming_order_id;
                        break;
                }

                fills.push_back(fill);

                if (resting.remaining_ == 0) {
                    live_orders_.erase(resting.id_);
                    queue.pop_front();
                }
            }
        }
    }
}