#include "testing.hpp"
#include "testing_event.hpp"

#include <string>

using namespace std::literals;

SNITCH_WARNING_PUSH
SNITCH_WARNING_DISABLE_UNREACHABLE

TEST_CASE("capture", "[test macros]") {
    mock_framework framework;
    framework.setup_reporter();

    SECTION("literal int") {
        framework.test_case.func = []() {
            SNITCH_CAPTURE(1);
            SNITCH_FAIL("trigger");
        };

        framework.run_test();
        CHECK_CAPTURES("1 := 1");
    }

    SECTION("literal string") {
        framework.test_case.func = []() {
            SNITCH_CAPTURE("hello");
            SNITCH_FAIL("trigger");
        };

        framework.run_test();
        CHECK_CAPTURES("\"hello\" := hello");
    }

    SECTION("variable int") {
        framework.test_case.func = []() {
            int i = 1;
            SNITCH_CAPTURE(i);
            SNITCH_FAIL("trigger");
        };

        framework.run_test();
        CHECK_CAPTURES("i := 1");
    }

    SECTION("variable string") {
        framework.test_case.func = []() {
            std::string s = "hello";
            SNITCH_CAPTURE(s);
            SNITCH_FAIL("trigger");
        };

        framework.run_test();
        CHECK_CAPTURES("s := hello");
    }

    SECTION("expression int") {
        framework.test_case.func = []() {
            int i = 1;
            SNITCH_CAPTURE(2 * i + 1);
            SNITCH_FAIL("trigger");
        };

        framework.run_test();
        CHECK_CAPTURES("2 * i + 1 := 3");
    }

    SECTION("expression string") {
        framework.test_case.func = []() {
            std::string s = "hello";
            SNITCH_CAPTURE(s + ", 'world' (string),)(");
            SNITCH_FAIL("trigger");
        };

        framework.run_test();
        CHECK_CAPTURES("s + \", 'world' (string),)(\" := hello, 'world' (string),)(");
    }

    SECTION("expression function call & char") {
        framework.test_case.func = []() {
            std::string s = "hel\"lo";
            SNITCH_CAPTURE(s.find_first_of('e'));
            SNITCH_CAPTURE(s.find_first_of('"'));
            SNITCH_FAIL("trigger");
        };

        framework.run_test();
        CHECK_CAPTURES("s.find_first_of('e') := 1", "s.find_first_of('\"') := 3");
    }

    SECTION("two variables") {
        framework.test_case.func = []() {
            int i = 1;
            int j = 2;
            SNITCH_CAPTURE(i, j);
            SNITCH_FAIL("trigger");
        };

        framework.run_test();
        CHECK_CAPTURES("i := 1", "j := 2");
    }

    SECTION("three variables different types") {
        framework.test_case.func = []() {
            int         i = 1;
            int         j = 2;
            std::string s = "hello";
            SNITCH_CAPTURE(i, j, s);
            SNITCH_FAIL("trigger");
        };

        framework.run_test();
        CHECK_CAPTURES("i := 1", "j := 2", "s := hello");
    }

    SECTION("scoped out") {
        framework.test_case.func = []() {
            {
                int i = 1;
                SNITCH_CAPTURE(i);
            }
            SNITCH_FAIL("trigger");
        };

        framework.run_test();
        CHECK_NO_CAPTURE;
    }

    SECTION("scoped out multiple capture") {
        framework.test_case.func = []() {
            int i = 1;
            SNITCH_CAPTURE(i);

            {
                int j = 2;
                SNITCH_CAPTURE(j);
            }

            SNITCH_FAIL("trigger");
        };

        framework.run_test();
        CHECK_CAPTURES("i := 1");
    }

    SECTION("multiple failures") {
        framework.test_case.func = []() {
            int i = 1;
            SNITCH_CAPTURE(i);
            SNITCH_FAIL_CHECK("trigger1");
            SNITCH_FAIL_CHECK("trigger2");
        };

        framework.run_test();
        REQUIRE(framework.get_num_failures() == 2u);
        CHECK_CAPTURES_FOR_FAILURE(0u, "i := 1");
        CHECK_CAPTURES_FOR_FAILURE(1u, "i := 1");
    }

    SECTION("multiple failures interleaved") {
        framework.test_case.func = []() {
            int i = 1;
            SNITCH_CAPTURE(i);
            SNITCH_FAIL_CHECK("trigger1");
            SNITCH_CAPTURE(2 * i);
            SNITCH_FAIL_CHECK("trigger2");
        };

        framework.run_test();
        REQUIRE(framework.get_num_failures() == 2u);
        CHECK_CAPTURES_FOR_FAILURE(0u, "i := 1");
        CHECK_CAPTURES_FOR_FAILURE(1u, "i := 1", "2 * i := 2");
    }
};

TEST_CASE("info", "[test macros]") {
    mock_framework framework;
    framework.setup_reporter();

    SECTION("literal int") {
        framework.test_case.func = []() {
            SNITCH_INFO(1);
            SNITCH_FAIL("trigger");
        };

        framework.run_test();
        CHECK_CAPTURES("1");
    }

    SECTION("literal string") {
        framework.test_case.func = []() {
            SNITCH_INFO("hello");
            SNITCH_FAIL("trigger");
        };

        framework.run_test();
        CHECK_CAPTURES("hello");
    }

    SECTION("variable int") {
        framework.test_case.func = []() {
            int i = 1;
            SNITCH_INFO(i);
            SNITCH_FAIL("trigger");
        };

        framework.run_test();
        CHECK_CAPTURES("1");
    }

    SECTION("variable string") {
        framework.test_case.func = []() {
            std::string s = "hello";
            SNITCH_INFO(s);
            SNITCH_FAIL("trigger");
        };

        framework.run_test();
        CHECK_CAPTURES("hello");
    }

    SECTION("expression int") {
        framework.test_case.func = []() {
            int i = 1;
            SNITCH_INFO(2 * i + 1);
            SNITCH_FAIL("trigger");
        };

        framework.run_test();
        CHECK_CAPTURES("3");
    }

    SECTION("expression string") {
        framework.test_case.func = []() {
            std::string s = "hello";
            SNITCH_INFO(s + ", 'world'");
            SNITCH_FAIL("trigger");
        };

        framework.run_test();
        CHECK_CAPTURES("hello, 'world'");
    }

    SECTION("multiple") {
        framework.test_case.func = []() {
            int         i = 1;
            int         j = 2;
            std::string s = "hello";
            SNITCH_INFO(i, " and ", j);
            SNITCH_FAIL("trigger");
        };

        framework.run_test();
        CHECK_CAPTURES("1 and 2");
    }

    SECTION("scoped out") {
        framework.test_case.func = []() {
            {
                int i = 1;
                SNITCH_INFO(i);
            }
            SNITCH_FAIL("trigger");
        };

        framework.run_test();
        CHECK_NO_CAPTURE;
    }

    SECTION("scoped out multiple") {
        framework.test_case.func = []() {
            int i = 1;
            SNITCH_INFO(i);

            {
                int j = 2;
                SNITCH_INFO(j);
            }

            SNITCH_FAIL("trigger");
        };

        framework.run_test();
        CHECK_CAPTURES("1");
    }

    SECTION("multiple failures") {
        framework.test_case.func = []() {
            int i = 1;
            SNITCH_INFO(i);
            SNITCH_FAIL_CHECK("trigger1");
            SNITCH_FAIL_CHECK("trigger2");
        };

        framework.run_test();
        REQUIRE(framework.get_num_failures() == 2u);
        CHECK_CAPTURES_FOR_FAILURE(0u, "1");
        CHECK_CAPTURES_FOR_FAILURE(1u, "1");
    }

    SECTION("multiple failures interleaved") {
        framework.test_case.func = []() {
            int i = 1;
            SNITCH_INFO(i);
            SNITCH_FAIL_CHECK("trigger1");
            SNITCH_INFO(2 * i);
            SNITCH_FAIL_CHECK("trigger2");
        };

        framework.run_test();
        REQUIRE(framework.get_num_failures() == 2u);
        CHECK_CAPTURES_FOR_FAILURE(0u, "1");
        CHECK_CAPTURES_FOR_FAILURE(1u, "1", "2");
    }

    SECTION("mixed with capture") {
        framework.test_case.func = []() {
            int i = 1;
            SNITCH_INFO(i);
            SNITCH_CAPTURE(i);
            SNITCH_FAIL("trigger");
        };

        framework.run_test();
        CHECK_CAPTURES("1", "i := 1");
    }
};

SNITCH_WARNING_POP
