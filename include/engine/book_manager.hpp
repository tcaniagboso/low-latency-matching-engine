#pragma once

#include "types.hpp"

namespace engine {

    template<typename OrderBookT>
    class BookManager {
    public:
        explicit BookManager(size_t num_symbols, std::vector<InstrumentConfig> configs);

        OrderBookT& get_order_book(SymbolId id);
        const OrderBookT& get_order_book(SymbolId id) const;

        const InstrumentConfig &get_config(SymbolId id) const;

        bool can_enter_matching(const InstrumentConfig &config, PriceT price, Side side);

        bool can_rest_on_book(const InstrumentConfig &config, PriceT price);

    private:
        std::vector<OrderBookT> books_;
        std::vector<InstrumentConfig> configs_;

        bool tick_validation(const InstrumentConfig &config, PriceT price);
    };

    template<typename OrderBookT>
    BookManager<OrderBookT>::BookManager(size_t num_symbols, std::vector<InstrumentConfig> configs)
            : books_{},
              configs_{std::move(configs)} {
        books_.reserve(num_symbols);

        for (size_t symbol_id = 0; symbol_id < num_symbols; symbol_id++) {
            books_.emplace_back(configs_[symbol_id]);
        }
    }

    template<typename OrderBookT>
    OrderBookT &BookManager<OrderBookT>::get_order_book(SymbolId id) {
        return books_[id];
    }

    template<typename OrderBookT>
    const OrderBookT &BookManager<OrderBookT>::get_order_book(SymbolId id) const {
        return books_[id];
    }

    template<typename OrderBookT>
    const InstrumentConfig &BookManager<OrderBookT>::get_config(engine::SymbolId id) const {
        return configs_[id];
    }

    template<typename OrderBookT>
    bool BookManager<OrderBookT>::tick_validation(const InstrumentConfig &config, PriceT price) {
        PriceT delta = price - config.min_price_;

        return (delta % config.tick_size_units_) == 0;
    }

    template<typename OrderBookT>
    bool BookManager<OrderBookT>::can_enter_matching(const InstrumentConfig &config, PriceT price, Side side) {
        if (price >= config.min_price_ && price <= config.max_price_ && !tick_validation(config, price)) {
            return false;
        }

        switch (side) {
            case Side::BUY:
                return price >= config.min_price_;
            case Side::SELL:
                return price <= config.max_price_;
            default:
                return false;
        }
    }

    template<typename OrderBookT>
    bool BookManager<OrderBookT>::can_rest_on_book(const InstrumentConfig &config, PriceT price) {
        return price >= config.min_price_ && price <= config.max_price_ && tick_validation(config, price);
    }
} // namespace engine