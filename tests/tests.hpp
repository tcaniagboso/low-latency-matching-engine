#pragma once

#include <cassert>

#include "engine/matching_engine.hpp"
#include "core/types.hpp"

namespace tests {
// =========================
// Helper
// =========================
    inline void assert_trade(
            const engine::Trade &trade,
            types::Symbol symbol,
            types::Quantity qty,
            types::PriceT price,
            types::OrderType type
    ) {
        assert(trade.symbol_ == symbol);
        assert(trade.quantity_ == qty);
        assert(trade.price_ == price);
        assert(trade.type_ == type);
    }

// =========================
// Limit Orders
// =========================
    template<typename EngineT>
    void test_simple_limit_buy() {
        EngineT engine{};

        engine.limit_sell(1, 100, 10);
        engine.limit_buy(1, 100, 10);

        const auto &trades = engine.get_trades();

        assert(trades.size() == 1);
        assert_trade(trades[0], 1, 10, 100, types::OrderType::LIMIT);
    }

    template<typename EngineT>
    void test_simple_limit_sell() {
        EngineT engine{};

        engine.limit_buy(1, 100, 10);
        engine.limit_sell(1, 100, 10);

        const auto &trades = engine.get_trades();

        assert(trades.size() == 1);
        assert_trade(trades[0], 1, 10, 100, types::OrderType::LIMIT);
    }

    template<typename EngineT>
    void test_partial_limit_buy_fill() {
        EngineT engine{};

        engine.limit_sell(1, 100, 10);
        engine.limit_buy(1, 100, 5);

        const auto &trades = engine.get_trades();

        assert(trades.size() == 1);
        assert_trade(trades[0], 1, 5, 100, types::OrderType::LIMIT);

        engine.limit_buy(1, 100, 10);

        const auto &trades2 = engine.get_trades();

        assert(trades2.size() == 2);
        assert_trade(trades2[1], 1, 5, 100, types::OrderType::LIMIT);
    }

    template<typename EngineT>
    void test_partial_limit_sell_fill() {
        EngineT engine{};

        engine.limit_buy(1, 100, 10);
        engine.limit_sell(1, 100, 5);

        const auto &trades = engine.get_trades();

        assert(trades.size() == 1);
        assert_trade(trades[0], 1, 5, 100, types::OrderType::LIMIT);

        engine.limit_sell(1, 100, 10);

        const auto &trades2 = engine.get_trades();

        assert(trades2.size() == 2);
        assert_trade(trades2[1], 1, 5, 100, types::OrderType::LIMIT);
    }

// =========================
// Market Orders
// =========================
    template<typename EngineT>
    void test_simple_market_buy() {
        EngineT engine{};

        engine.limit_sell(1, 100, 10);
        engine.market_buy(1, 10);

        const auto &trades = engine.get_trades();

        assert(trades.size() == 1);
        assert_trade(trades[0], 1, 10, 100, types::OrderType::MARKET);
    }

    template<typename EngineT>
    void test_simple_market_sell() {
        EngineT engine{};

        engine.limit_buy(1, 100, 10);
        engine.market_sell(1, 10);

        const auto &trades = engine.get_trades();

        assert(trades.size() == 1);
        assert_trade(trades[0], 1, 10, 100, types::OrderType::MARKET);
    }

    template<typename EngineT>
    void test_partial_market_buy() {
        EngineT engine{};

        engine.limit_sell(1, 100, 10);
        engine.market_buy(1, 5);

        const auto &trades = engine.get_trades();

        assert(trades.size() == 1);
        assert_trade(trades[0], 1, 5, 100, types::OrderType::MARKET);
    }

    template<typename EngineT>
    void test_partial_market_sell() {
        EngineT engine{};

        engine.limit_buy(1, 100, 10);
        engine.market_sell(1, 5);

        const auto &trades = engine.get_trades();

        assert(trades.size() == 1);
        assert_trade(trades[0], 1, 5, 100, types::OrderType::MARKET);
    }

    template<typename EngineT>
    void test_empty_market_order() {
        EngineT engine{};

        engine.market_buy(1, 10);
        engine.market_sell(1, 10);

        const auto &trades = engine.get_trades();
        assert(trades.empty());
    }

// =========================
// Symbol Isolation
// =========================
    template<typename EngineT>
    void test_diff_symbols() {
        EngineT engine{};

        engine.limit_sell(1, 100, 10);
        engine.limit_buy(2, 100, 10);

        const auto &trades = engine.get_trades();
        assert(trades.empty());

        engine.market_buy(2, 10);

        const auto &trades2 = engine.get_trades();
        assert(trades2.empty());
    }

// =========================
// Cancel
// =========================
    template<typename EngineT>
    void test_cancel() {
        EngineT engine{};

        auto id = engine.limit_sell(1, 100, 10);
        engine.cancel(id);

        engine.limit_buy(1, 100, 10);

        assert(engine.get_trades().empty());
    }

    template<typename EngineT>
    void test_cancel_partial_book() {
        EngineT engine{};

        auto id1 = engine.limit_sell(1, 100, 5);
        engine.limit_sell(1, 100, 5);

        engine.cancel(id1);

        engine.limit_buy(1, 100, 10);

        const auto &trades = engine.get_trades();

        assert(trades.size() == 1);
        assert(trades[0].quantity_ == 5);
    }

// =========================
// Priority (CRITICAL)
// =========================
    template<typename EngineT>
    void test_level_aggregation() {
        EngineT engine{};

        engine.limit_sell(1, 100, 5);
        engine.limit_sell(1, 100, 5);

        engine.limit_buy(1, 100, 10);

        const auto &trades = engine.get_trades();

        assert(trades.size() == 1);
        assert(trades[0].quantity_ == 10);
    }

    template<typename EngineT>
    void test_multi_level_match() {
        EngineT engine{};

        engine.limit_sell(1, 100, 5);
        engine.limit_sell(1, 101, 5);

        engine.limit_buy(1, 101, 10);

        const auto &trades = engine.get_trades();

        assert(trades.size() == 2);
        assert(trades[0].price_ == 100);
        assert(trades[1].price_ == 101);
    }

    template<typename EngineT>
    void test_complex_matching() {
        EngineT engine{};

        engine.limit_sell(1, 100, 5);
        engine.limit_sell(1, 100, 5);
        engine.limit_sell(1, 101, 5);

        engine.limit_buy(1, 101, 12);

        const auto &trades = engine.get_trades();

        assert(trades.size() == 2);

        assert_trade(trades[0], 1, 10, 100, types::OrderType::LIMIT);
        assert_trade(trades[1], 1, 2, 101, types::OrderType::LIMIT);
    }

// =========================
// Runner
// =========================
    template<typename EngineT>
    void run_tests_suite() {
        test_simple_limit_buy<EngineT>();
        test_simple_limit_sell<EngineT>();
        test_partial_limit_buy_fill<EngineT>();
        test_partial_limit_sell_fill<EngineT>();
        test_simple_market_buy<EngineT>();
        test_simple_market_sell<EngineT>();
        test_partial_market_buy<EngineT>();
        test_partial_market_sell<EngineT>();
        test_empty_market_order<EngineT>();
        test_diff_symbols<EngineT>();
        test_cancel<EngineT>();
        test_cancel_partial_book<EngineT>();
        test_level_aggregation<EngineT>();
        test_multi_level_match<EngineT>();
        test_complex_matching<EngineT>();
    }
} // namespace tests
