#include "testing.hpp"
#include "testing_event.hpp"

using namespace std::literals;

SNITCH_WARNING_PUSH
SNITCH_WARNING_DISABLE_UNREACHABLE

TEST_CASE("skip", "[test macros]") {
    mock_framework framework;
    framework.setup_reporter();

    SECTION("no skip") {
        framework.test_case.func = []() { SNITCH_FAIL_CHECK("trigger"); };

        framework.run_test();
        CHECK(framework.get_num_skips() == 0u);
    }

    SECTION("only skip") {
        framework.test_case.func = []() { SNITCH_SKIP("hello"); };

        framework.run_test();
        CHECK(framework.get_num_skips() == 1u);
    }

    SECTION("skip failure") {
        framework.test_case.func = []() {
            SNITCH_SKIP("hello");
            SNITCH_FAIL_CHECK("trigger");
        };

        framework.run_test();
        CHECK(framework.get_num_skips() == 1u);
        CHECK(framework.get_num_failures() == 0u);
    }

    SECTION("skip section") {
        framework.test_case.func = []() {
            SNITCH_SECTION("section 1") {
                SNITCH_SKIP("hello");
            }
            SNITCH_SECTION("section 2") {
                SNITCH_FAIL_CHECK("trigger");
            }
        };

        framework.run_test();
        CHECK(framework.get_num_skips() == 1u);
        CHECK(framework.get_num_failures() == 0u);
    }
};

SNITCH_WARNING_POP
