#pragma once

#include <atomic>
#include <cstdint>

namespace id {

    struct alignas(64) PaddedAtomic {
        std::atomic<uint64_t> value_;
    };
    static_assert(sizeof(PaddedAtomic) == 64);

    class OrderIdGenerator {
    private:
        PaddedAtomic next_id_{1};

    public:
        uint64_t next() {
            return next_id_.value_.fetch_add(1, std::memory_order_relaxed);
        }
    };

    class TradeIdGenerator {
    private:
        PaddedAtomic next_id_{1};

    public:
        uint64_t next() {
            return next_id_.value_.fetch_add(1, std::memory_order_relaxed);
        }

    };
} // namespace id