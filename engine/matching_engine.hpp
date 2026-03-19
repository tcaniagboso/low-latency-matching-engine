#pragma once

#include <iostream>
#include <vector>

#include "../core/id_generator.hpp"
#include "../core/types.hpp"
#include "../core/time.hpp"
#include "../order_book/list_order_book.hpp"

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

    class MatchingEngine {

    private:
        id::OrderIdGenerator order_id_gen_;
        id::TradeIdGenerator trade_id_gen_;
        order_book::ListOrderBook order_book_;
        std::vector<Trade> fulfilled_;

        static inline types::Quantity consume_level(
                order_book::ListOrderBook &order_book,
                types::Quantity &quantity,
                std::list<order_book::RestingOrder> &queue
        ) {
            types::Quantity traded{0};
            while (quantity > 0 && !queue.empty()) {
                auto &res_order = queue.front();
                auto min_qty = std::min(quantity, res_order.remaining_);

                quantity -= min_qty;
                res_order.remaining_ -= min_qty;
                traded += min_qty;

                if (res_order.remaining_ == 0) {
                    order_book.erase_locator(res_order.id_);
                    queue.pop_front();

                }
            }

            return traded;
        }

        void match(types::OrderType type, types::Symbol symbol, types::PriceT price, types::Quantity quantity,
                   types::Side side);

    public:
        MatchingEngine();

        void limit_buy(types::Symbol symbol, types::PriceT price, types::Quantity quantity);

        void limit_sell(types::Symbol symbol, types::PriceT price, types::Quantity quantity);

        void market_buy(types::Symbol symbol, types::Quantity quantity);

        void market_sell(types::Symbol symbol, types::Quantity quantity);

        bool cancel(types::OrderId id);
    };
} // namespace engine