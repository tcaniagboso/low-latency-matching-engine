#include <stdexcept>

#include "parser.hpp"

namespace gateway {
    inline void to_lower(std::string &s) {
        for (char &c: s) {
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
    }

    inline void to_upper(std::string &s) {
        for (char &c: s) {
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        }
    }

    inline types::Side parse_side(std::string &side) {
        gateway::to_lower(side);
        if (side == "buy") return types::Side::BUY;
        if (side == "sell") return types::Side::SELL;
        throw std::invalid_argument("Invalid Order Side: " + side);
    }

    inline types::OrderType parse_order_type(std::string &order_type) {
        gateway::to_lower(order_type);
        if (order_type == "limit") return types::OrderType::LIMIT;
        if (order_type == "market") return types::OrderType::MARKET;
        throw std::invalid_argument("Invalid Order Type: " + order_type);
    }

    inline types::Quantity parse_quantity(const std::string &quantity) {
        try {
            long long qty = std::stoll(quantity);
            if (qty <= 0) {
                throw std::invalid_argument("Quantity must be positive: " + quantity);
            }
            return static_cast<types::Quantity>(qty);
        } catch (...) {
            throw;
        }
    }

    inline types::PriceT parse_price(const std::string &price) {
        try {
            long long price_t = std::stoll(price);
            return static_cast<types::PriceT>(price_t);
        } catch (...) {
            throw;
        }
    }

} // namespace gateway