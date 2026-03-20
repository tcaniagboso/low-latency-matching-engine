#include "list_order_book.hpp"
#include "../core/time.hpp"

namespace order_book {
    ListOrderBook::AskMap ListOrderBook::empty_asks_;
    ListOrderBook::BidMap ListOrderBook::empty_bids_;

    ListOrderBook::ListOrderBook()
            : live_orders_{},
              asks_{},
              bids_{} {}

//    void ListOrderBook::erase_locator(types::OrderId id) {
//        live_orders_.erase(id);
//    }

//    void ListOrderBook::add_order(types::Symbol symbol, types::OrderId id, types::PriceT price,
//                                  types::Quantity quantity, types::Side side) {
//        types::TimePt timestamp = time_utils::now();
//        ListOrderBook::Queue *queue;
//        if (side == types::Side::BUY) {
//            queue = &bids_[symbol][price];
//        } else {
//            queue = &asks_[symbol][price];
//        }
//        queue->emplace_back(id, quantity, timestamp);
//        Locator locator{symbol, side, price, std::prev(queue->end())};
//        live_orders_.emplace(id, locator);
//
//    }

    void ListOrderBook::add_limit_buy(types::Symbol symbol, types::OrderId id, types::PriceT price,
                                      types::Quantity quantity) {
        auto timestamp = time_utils::now();
        auto &queue = bids_[symbol][price];
        queue.emplace_back(id, quantity, timestamp);
        Locator locator{symbol, types::Side::BUY, price, std::prev(queue.end())};
        live_orders_.emplace(id, locator);
    }

    void ListOrderBook::add_limit_sell(types::Symbol symbol, types::OrderId id, types::PriceT price,
                                       types::Quantity quantity) {
        auto timestamp = time_utils::now();
        auto &queue = asks_[symbol][price];
        queue.emplace_back(id, quantity, timestamp);
        Locator locator{symbol, types::Side::SELL, price, std::prev(queue.end())};
        live_orders_.emplace(id, locator);
    }

    bool ListOrderBook::cancel(types::OrderId id) {
        auto it = live_orders_.find(id);
        if (it == live_orders_.end()) return false;
        const auto &locator = it->second;

        if (locator.side_ == types::Side::BUY) {
            auto symbol_it = bids_.find(locator.symbol_);
            if (symbol_it == bids_.end()) return false;
            auto price_it = symbol_it->second.find(locator.price_);
            if (price_it == symbol_it->second.end()) return false;
            price_it->second.erase(locator.position_);
            if (price_it->second.empty()) symbol_it->second.erase(price_it);
        } else {
            auto symbol_it = asks_.find(locator.symbol_);
            if (symbol_it == asks_.end()) return false;
            auto price_it = symbol_it->second.find(locator.price_);
            if (price_it == symbol_it->second.end()) return false;
            price_it->second.erase(locator.position_);
            if (price_it->second.empty()) symbol_it->second.erase(price_it);
        }

        live_orders_.erase(it);
        return true;
    }

    ListOrderBook::AskMap::iterator ListOrderBook::remove_asks_level(
            types::Symbol symbol,
            AskMap::iterator it
    ) {
        auto symbol_it = asks_.find(symbol);
        return symbol_it->second.erase(it);
    }

    ListOrderBook::BidMap::iterator ListOrderBook::remove_bids_level(
            types::Symbol symbol,
            BidMap::iterator it
    ) {
        auto symbol_it = bids_.find(symbol);
        return symbol_it->second.erase(it);
    }

    bool ListOrderBook::level_empty(Queue &queue) {
        return queue.empty();
    }

//    bool ListOrderBook::has_asks(types::Symbol symbol) const {
//        return asks_.find(symbol) != asks_.end();
//    }
//
//    bool ListOrderBook::has_bids(types::Symbol symbol) const {
//        return bids_.find(symbol) != bids_.end();
//    }

    ListOrderBook::AskMap::iterator ListOrderBook::asks_begin(types::Symbol symbol) {
        auto it = asks_.find(symbol);
        if (it == asks_.end()) return empty_asks_.end();
        return it->second.begin();
    }

    ListOrderBook::AskMap::iterator ListOrderBook::asks_end(types::Symbol symbol) {
        auto it = asks_.find(symbol);
        if (it == asks_.end()) return empty_asks_.end();
        return it->second.end();
    }

    ListOrderBook::BidMap::iterator ListOrderBook::bids_begin(types::Symbol symbol) {
        auto it = bids_.find(symbol);
        if (it == bids_.end()) return empty_bids_.end();
        return it->second.begin();
    }

    ListOrderBook::BidMap::iterator ListOrderBook::bids_end(types::Symbol symbol) {
        auto it = bids_.find(symbol);
        if (it == bids_.end()) return empty_bids_.end();
        return it->second.end();
    }

    types::Quantity ListOrderBook::consume_level(types::Quantity &quantity, ListOrderBook::Queue &queue) {
        types::Quantity traded{0};
        while (quantity > 0 && !queue.empty()) {
            auto &order = queue.front();
            auto min_qty = (quantity < order.remaining_) ? quantity : order.remaining_;
            quantity -= min_qty;
            order.remaining_ -= min_qty;
            traded += min_qty;

            if (order.remaining_ == 0) {
                live_orders_.erase(order.id_);
                queue.pop_front();
            }
        }

        return traded;
    }
} // namespace order_book