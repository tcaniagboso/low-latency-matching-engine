#pragma once

#include "../core/order/fill_event.hpp"
#include "../core/order/resting_order.hpp"
#include "../core/types/types.hpp"
#include "../instruments/instrument_config.hpp"

namespace order_book {
    using FillEvent = core::order::FillEvent;
    using InstrumentConfig = instruments::InstrumentConfig;
    using OrderId = core::types::OrderId;
    using PriceT = core::types::PriceT;
    using Quantity = core::types::Quantity;
    using RestingOrder = core::order::RestingOrder;
    using Side = core::types::Side;
    using SymbolId = core::types::SymbolId;
} // namespace order_book