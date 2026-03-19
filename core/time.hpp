#pragma once

#include <chrono>
#include <iomanip>

#include "types.hpp"

namespace time_utils {

    inline types::TimePt now() {
        return std::chrono::steady_clock::now();
    }

    inline std::string time_point_to_string(types::TimePt tp) {
        std::time_t t = std::chrono::system_clock::to_time_t(tp);

        std::tm tm = *std::localtime(&t);

        // Use stringstream to format the output
        std::stringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

        auto since_epoch = tp.time_since_epoch();
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(since_epoch);
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch - seconds);

        // Append milliseconds with leading zeros
        ss << '.' << std::setfill('0') << std::setw(3) << milliseconds.count();

        return ss.str();
    }

} // namespace time_utils