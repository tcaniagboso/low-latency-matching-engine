
#include "engine/matching_engine.hpp"
#include "order_book/deque_order_book.hpp"
#include "order_book/list_order_book.hpp"

int main(int argc, char* argv[]) {

    engine::MatchingEngine<order_book::ListOrderBook> list_matching_engine{};
    engine::MatchingEngine<order_book::DequeOrderBook> deque_matching_engine{};

    return 0;
}