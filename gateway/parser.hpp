#include <string>

#include "../types/types.hpp"

namespace gateway {

    inline void to_lower(std::string &s);

    inline void to_upper(std::string &s);

    inline types::Side parse_side(std::string &side);

    inline types::OrderType parse_order_type(std::string &order_type);

    inline types::Quantity parse_quantity(const std::string &quantity);

    inline types::PriceT parse_price(const std::string &price);
} // namespace gateway