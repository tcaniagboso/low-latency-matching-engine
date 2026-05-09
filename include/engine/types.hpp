#pragma once

#include <optional>
#include <vector>

#include "../core/order/fill_event.hpp"
#include "../core/order/trade.hpp"
#include "../core/types/types.hpp"
#include "../instruments/instrument_config.hpp"

namespace engine {
    using FillEvent = core::order::FillEvent;
    using InstrumentConfig = instruments::InstrumentConfig;
    using OrderId = core::types::OrderId;
    using OrderType = core::types::OrderType;
    using PriceT = core::types::PriceT;
    using Quantity = core::types::Quantity;
    using Side = core::types::Side;
    using SymbolId = core::types::SymbolId;
    using TimePt = core::types::TimePt;
    using Trade = core::order::Trade;
    using TradeId = core::types::TradeId;

    struct MatchResult {
        Quantity remaining_{};
        std::vector<FillEvent> fills_{};

        MatchResult() = default;
        MatchResult(Quantity remaining, std::vector<FillEvent> fills)
                : remaining_{remaining},
                  fills_{std::move(fills)} {}
    };

    enum class SubmitOrderStatus {
        REJECTED,
        ACCEPTED,
        FULLY_FILLED,
        PARTIALLY_FILLED_AND_RESTED,
        PARTIALLY_FILLED_AND_CANCELED
    };

    enum class SubmitRejectReason : uint8_t {
        INVALID_SYMBOL,
        INVALID_QUANTITY,
        INVALID_TICK,
        PRICE_TOO_LOW_TO_MATCH,
        PRICE_TOO_HIGH_TO_MATCH,
        REMAINDER_OUT_OF_BAND
    };

    enum class CancelOrderStatus {
        SUCCESS,
        FAILED
    };

    enum class CancelRejectReason {
        ORDER_NOT_FOUND
    };

    struct ExecutionSummary {
        PriceT price_;
        Quantity quantity_;
    };

    struct SubmitOrderResult {
        SubmitOrderStatus status_;
        Quantity filled_quantity_;
        Quantity remaining_quantity_;
        std::vector<ExecutionSummary> executions_;
        std::optional<SubmitRejectReason> reject_reason_;
    };

    struct CancelOrderResult {
        CancelOrderStatus status_;
        std::optional<CancelRejectReason> reject_reason_;
    };


} // namespace engine