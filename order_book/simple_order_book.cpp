#include "simple_order_book.hpp"
#include "../core/time.hpp"

namespace order_book {
    AskMap SimpleOrderBook::empty_asks_;
    BidMap SimpleOrderBook::empty_bids_;

    SimpleOrderBook::SimpleOrderBook()
            : live_orders_{},
              asks_{},
              bids_{} {}

    void SimpleOrderBook::add_order(types::Symbol symbol, types::OrderId id, types::PriceT price,
                                    types::Quantity quantity, types::Side side) {
        types::TimePt timestamp = time_utils::now();
        std::list<RestingOrder> *queue;
        if (side == types::Side::BUY) {
            queue = &bids_[symbol][price];
        } else {
            queue = &asks_[symbol][price];
        }
        queue->emplace_back(id, quantity, timestamp);
        Locator locator{symbol, side, price, std::prev(queue->end())};
        live_orders_.emplace(id, locator);

    }

    void SimpleOrderBook::add_limit_buy(types::Symbol symbol, types::OrderId id, types::PriceT price,
                                        types::Quantity quantity) {

        add_order(symbol, id, price, quantity, types::Side::BUY);
    }

    void SimpleOrderBook::add_limit_sell(types::Symbol symbol, types::OrderId id, types::PriceT price,
                                         types::Quantity quantity) {
        add_order(symbol, id, price, quantity, types::Side::SELL);
    }

    bool SimpleOrderBook::cancel(types::OrderId id) {
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

    void SimpleOrderBook::erase_locator(types::OrderId id) {
        live_orders_.erase(id);
    }

    AskMap::iterator SimpleOrderBook::remove_asks_level(
            types::Symbol symbol,
            AskMap::iterator it
    ) {
        auto symbol_it = asks_.find(symbol);
        return symbol_it->second.erase(it);
    }

    BidMap::iterator SimpleOrderBook::remove_bids_level(
            types::Symbol symbol,
            BidMap::iterator it
    ) {
        auto symbol_it = bids_.find(symbol);
        return symbol_it->second.erase(it);
    }

    bool SimpleOrderBook::has_asks(types::Symbol symbol) const {
        return asks_.find(symbol) != asks_.end();
    }

    bool SimpleOrderBook::has_bids(types::Symbol symbol) const {
        return bids_.find(symbol) != bids_.end();
    }

    AskMap::iterator SimpleOrderBook::asks_begin(types::Symbol symbol) {
        auto it = asks_.find(symbol);
        if (it == asks_.end()) return empty_asks_.end();
        return it->second.begin();
    }

    AskMap::iterator SimpleOrderBook::asks_end(types::Symbol symbol) {
        auto it = asks_.find(symbol);
        if (it == asks_.end()) return empty_asks_.end();
        return it->second.end();
    }

    BidMap::iterator SimpleOrderBook::bids_begin(types::Symbol symbol) {
        auto it = bids_.find(symbol);
        if (it == bids_.end()) return empty_bids_.end();
        return it->second.begin();
    }

    BidMap::iterator SimpleOrderBook::bids_end(types::Symbol symbol) {
        auto it = bids_.find(symbol);
        if (it == bids_.end()) return empty_bids_.end();
        return it->second.end();
    }
} // namespace order_book