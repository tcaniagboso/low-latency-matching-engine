#include "engine/matching_engine.hpp"
#include "order_book/deque_order_book.hpp"
#include "order_book/list_order_book.hpp"
#include "tests/tests.hpp"

int main(int argc, char* argv[]) {

    tests::run_tests_suite<engine::MatchingEngine<order_book::ListOrderBook>>();
    tests::run_tests_suite<engine::MatchingEngine<order_book::DequeOrderBook>>();

    return 0;
}