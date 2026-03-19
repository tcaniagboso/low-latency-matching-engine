#include <string>

#include "../core/types.hpp"

namespace gateway {

    void to_lower(std::string &s);

    void to_upper(std::string &s);

    types::Side parse_side(std::string &side);

    types::OrderType parse_order_type(std::string &order_type);

    types::Quantity parse_quantity(const std::string &quantity);

    types::PriceT parse_price(const std::string &price);
} // namespace gateway