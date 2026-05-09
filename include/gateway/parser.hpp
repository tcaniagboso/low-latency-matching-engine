#include <string>

#include "types.hpp"
#include "../core/types/types.hpp"

namespace gateway {

    void to_lower(std::string &s);

    void to_upper(std::string &s);

    Side parse_side(std::string &side);

    OrderType parse_order_type(std::string &order_type);

    Quantity parse_quantity(const std::string &quantity);

    PriceT parse_price(const std::string &price);
} // namespace gateway