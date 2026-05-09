#pragma once

#include "types.hpp"

#include "book_manager.hpp"
#include "matching_engine.hpp"

#include "../../include/gateway/order_entry.hpp"

namespace engine {

    template<typename OrderBookT>
    class TradeEngine {
    public:
        explicit TradeEngine(BookManager<OrderBookT> book_manager);
        SubmitOrderResult submit_order(const gateway::NewOrderRequest& request);
        CancelOrderResult cancel_order(const gateway::CancelOrderRequest& request);
        static void print_trades(const std::vector<Trade>& trades);

    private:
        BookManager<OrderBookT> book_manager_;
        MatchingEngine<OrderBookT> matching_engine_;
        std::vector<Trade> trades_;

        std::vector<ExecutionSummary> get_execution_summary(const std::vector<FillEvent>& fills);
    };
} // namespace engine