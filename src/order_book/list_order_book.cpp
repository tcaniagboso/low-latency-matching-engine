#include "../../include/order_book/list_order_book.hpp"
#include "../../include/core/time_utils/time.hpp"

namespace order_book {
    ListOrderBook::ListOrderBook(const InstrumentConfig &)
            : live_orders_{},
              asks_{},
              bids_{} {}

    void ListOrderBook::add_limit_buy(OrderId order_id, PriceT price, Quantity quantity) {
        auto timestamp = core::time_utils::now();
        auto &queue = bids_[price].queue_;
        queue.emplace_back(order_id, quantity, timestamp);
        Locator locator{price, Side::BUY, std::prev(queue.end())};
        live_orders_.emplace(order_id, locator);
    }

    void ListOrderBook::add_limit_sell(OrderId order_id, PriceT price, Quantity quantity) {
        auto timestamp = core::time_utils::now();
        auto &queue = asks_[price].queue_;
        queue.emplace_back(order_id, quantity, timestamp);
        Locator locator{price, Side::SELL, std::prev(queue.end())};
        live_orders_.emplace(order_id, locator);
    }

    bool ListOrderBook::cancel(OrderId order_id) {
        auto it = live_orders_.find(order_id);
        if (it == live_orders_.end()) return false;
        const auto &locator = it->second;

        if (locator.side_ == Side::BUY) {
            auto price_it = bids_.find(locator.price_);
            if (price_it == bids_.end()) return false;
            price_it->second.queue_.erase(locator.position_);
        } else {
            auto price_it = asks_.find(locator.price_);
            if (price_it == asks_.end()) return false;
            price_it->second.queue_.erase(locator.position_);
        }

        live_orders_.erase(it);
        return true;
    }

    ListOrderBook::AskMap::iterator ListOrderBook::remove_asks_level(AskMap::iterator it) {
        return asks_.erase(it);
    }

    ListOrderBook::BidMap::iterator ListOrderBook::remove_bids_level(BidMap::iterator it) {
        return bids_.erase(it);
    }

    ListOrderBook::AskMap::iterator ListOrderBook::asks_begin() {
        return asks_.begin();
    }

    ListOrderBook::AskMap::iterator ListOrderBook::asks_end() {
        return asks_.end();
    }

    ListOrderBook::BidMap::iterator ListOrderBook::bids_begin() {
        return bids_.begin();
    }

    ListOrderBook::BidMap::iterator ListOrderBook::bids_end() {
        return bids_.end();
    }

    bool ListOrderBook::level_empty(const ListOrderBook::Level &level) {
        return level.queue_.empty();
    }

    void ListOrderBook::consume_level(
            OrderId incoming_order_id,
            Side incoming_side,
            PriceT execution_price,
            Quantity &quantity,
            Level &level,
            std::vector<FillEvent> &fills) {

        auto &queue = level.queue_;
        while (quantity > 0 && !queue.empty()) {
            auto &resting = queue.front();
            auto match_qty = (quantity < resting.remaining_) ? quantity : resting.remaining_;
            quantity -= match_qty;
            resting.remaining_ -= match_qty;

            FillEvent fill{};
            fill.quantity_ = match_qty;
            fill.price_ = execution_price;

            switch (incoming_side) {
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
} // namespace order_book