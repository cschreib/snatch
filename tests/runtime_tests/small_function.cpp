#include "testing.hpp"

#include <tuple>

std::size_t test_object_instances = 0u;
bool        function_called       = false;
int         return_value          = 0u;

struct test_object {
    test_object() noexcept {
        ++test_object_instances;
    }
    test_object(const test_object&) noexcept {
        ++test_object_instances;
    }
    test_object(test_object&&) noexcept {
        ++test_object_instances;
    }
};

using function_0_void = void() noexcept;
using function_0_int  = int() noexcept;
using function_2_void = void(int, test_object) noexcept;
using function_2_int  = int(int, test_object) noexcept;

template<typename S>
struct test_class;

template<typename R, typename... Args>
struct test_class<R(Args...) noexcept> {
    R method(Args...) noexcept {
        function_called = true;
        if constexpr (!std::is_same_v<R, void>) {
            return 42;
        }
    }

    R method_const(Args...) const noexcept {
        function_called = true;
        if constexpr (!std::is_same_v<R, void>) {
            return 43;
        }
    }

    static R method_static(Args...) noexcept {
        function_called = true;
        if constexpr (!std::is_same_v<R, void>) {
            return 44;
        }
    }
};

// Needed for GCC.
template struct test_class<function_0_void>;
template struct test_class<function_0_int>;
template struct test_class<function_2_void>;
template struct test_class<function_2_int>;

template<typename T>
struct type_holder {};

// MSVC has a bug that prevents us from writing the test nicely. Work around it.
// https://developercommunity.visualstudio.com/t/Parameter-pack-argument-is-not-recognize/10191888
template<typename R, typename... Args>
void call_function(snitch::small_function<R(Args...) noexcept>& f) {
    if constexpr (std::is_same_v<R, void>) {
        std::apply(f, std::tuple<Args...>{});
    } else {
        return_value = std::apply(f, std::tuple<Args...>{});
    }
}

TEMPLATE_TEST_CASE(
    "small function",
    "[utility]",
    function_0_void,
    function_0_int,
    function_2_void,
    function_2_int) {

    [&]<typename R, typename... Args>(type_holder<R(Args...) noexcept>) {
        snitch::small_function<TestType> f;

        test_object_instances                    = 0u;
        return_value                             = 0u;
        function_called                          = false;
        constexpr std::size_t expected_instances = sizeof...(Args) > 0 ? 3u : 0u;

        CHECK(f.empty());

        SECTION("from free function") {
            f = &test_class<TestType>::method_static;
            CHECK(!f.empty());

            call_function(f);

            CHECK(function_called);
            if (!std::is_same_v<R, void>) {
                CHECK(return_value == 44);
            }
            CHECK(test_object_instances <= expected_instances);
        }

        SECTION("from non-const member function") {
            test_class<TestType> obj;
            f = {obj, snitch::constant<&test_class<TestType>::method>{}};
            CHECK(!f.empty());

            call_function(f);

            CHECK(function_called);
            if (!std::is_same_v<R, void>) {
                CHECK(return_value == 42);
            }
            CHECK(test_object_instances <= expected_instances);
        }

        SECTION("from const member function") {
            const test_class<TestType> obj;
            f = {obj, snitch::constant<&test_class<TestType>::method_const>{}};
            CHECK(!f.empty());

            call_function(f);

            CHECK(function_called);
            if (!std::is_same_v<R, void>) {
                CHECK(return_value == 43);
            }
            CHECK(test_object_instances <= expected_instances);
        }

        SECTION("from stateless lambda") {
            f = snitch::small_function<TestType>{[](Args...) noexcept -> R {
                function_called = true;
                if constexpr (!std::is_same_v<R, void>) {
                    return 45;
                }
            }};
            CHECK(!f.empty());

            call_function(f);

            CHECK(function_called);
            if (!std::is_same_v<R, void>) {
                CHECK(return_value == 45);
            }
            CHECK(test_object_instances <= expected_instances);
        }

        SECTION("from stateful lambda") {
            int  answer = 46;
            auto lambda = [&](Args...) noexcept -> R {
                function_called = true;
                if constexpr (!std::is_same_v<R, void>) {
                    return answer;
                }
            };

            f = snitch::small_function<TestType>{lambda};
            CHECK(!f.empty());

            call_function(f);

            CHECK(function_called);
            if (!std::is_same_v<R, void>) {
                CHECK(return_value == 46);
            }
            CHECK(test_object_instances <= expected_instances);
        }
    }(type_holder<TestType>{});
};
