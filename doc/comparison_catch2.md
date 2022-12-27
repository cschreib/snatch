# Feature comparison with _Catch2_

| Feature in _Catch2_                                 | In _snitch_   | On roadmap   |
| ----------------------------------------------------| ------------- | ------------ |
| **Test cases**                                      |               |              |
| - `TEST_CASE`                                       | Yes (1)       | Done         |
| - `TEMPLATE_TEST_CASE`                              | Yes           | Done         |
| - `TEMPLATE_LIST_TEST_CASE`                         | Yes           | Done         |
| - `TEMPLATE_PRODUCT_TEST_CASE`                      | No            | Maybe        |
| - `TEMPLATE_TEST_CASE_SIG`                          | No            | Maybe        |
| - `TEMPLATE_PRODUCT_TEST_CASE_SIG`                  | No            | Maybe        |
| - `METHOD_AS_TEST_CASE`                             | No            | Unlikely     |
| - `REGISTER_TEST_CASE`                              | No            | Unlikely     |
| - `TEST_CASE_METHOD`                                | No            | Unlikely     |
| - `TEMPLATE_TEST_CASE_METHOD_SIG`                   | No            | Unlikely     |
| - `TEMPLATE_PRODUCT_TEST_CASE_METHOD_SIG`           | No            | Unlikely     |
| - `TEMPLATE_LIST_TEST_CASE_METHOD`                  | No            | Unlikely     |
| **Tags**                                            |               |              |
| - Special tags                                      | Yes (2)       | Done         |
| - `REGISTER_TAG_ALIAS`                              | No            | Maybe        |
| **Control statements**                              |               |              |
| - `SECTION`                                         | Yes           | Done         |
| - `DYNAMIC_SECTION`                                 | No            | Maybe (8)    |
| - `CHECKED_IF`                                      | No            | Maybe        |
| - `CHECKED_ELSE`                                    | No            | Maybe        |
| **Assertion macros**                                |               |              |
| - `REQUIRE` / `CHECK`                               | Yes           | Done         |
| - `REQUIRE_FALSE` / `CHECK_FALSE`                   | Yes           | Done         |
| - `REQUIRE_THAT` / `CHECK_THAT`                     | Yes           | Done         |
| - `REQUIRE_THROWS` / `CHECK_THROWS`                 | No            | Maybe        |
| - `REQUIRE_THROWS_AS` / `CHECK_THROWS_AS`           | Yes           | Done         |
| - `REQUIRE_THROWS_WITH` / `CHECK_THROWS_WITH`       | No            | Maybe        |
| - `REQUIRE_THROWS_MATCHES` / `CHECK_THROWS_MATCHES` | Yes           | Done         |
| - `REQUIRE_NOTHROW` / `CHECK_NOTHROW`               | No            | Maybe        |
| - `STATIC_REQUIRE` / `STATIC_CHECK`                 | No            | Maybe        |
| - `CHECK_NOFAIL`                                    | No            | Maybe        |
| - `SUCCEED`                                         | No            | Maybe        |
| - `FAIL` / `FAIL_CHECK`                             | Yes (3)       | Done         |
| - `WARN`                                            | No            | Maybe        |
| **Logging and context**                             |               |              |
| - `CAPTURE`                                         | Yes           | Done         |
| - `INFO`                                            | Yes (3)       | Done         |
| - `UNSCOPED_INFO`                                   | No            | Unlikely     |
| **BDD-style macros**                                |               |              |
| - `SCENARIO`                                        | No            | No           |
| - `GIVEN` / `AND_GIVEN`                             | No            | No           |
| - `WHEN` / `AND_WHEN`                               | No            | No           |
| - `THEN` / `AND_THEN`                               | No            | No           |
| **Matchers**                                        |               |              |
| - Custom matchers                                   | Yes (4)       | Done         |
| - Combining matchers                                | No            | Yes          |
| **Float matchers**                                  |               |              |
| - `WithinAbs`                                       | No            | Maybe        |
| - `WithinRel`                                       | No            | Maybe        |
| - `WithinULP`                                       | No            | Maybe        |
| - `Approx`                                          | No            | No           |
| **String matchers**                                 |               |              |
| - `StartsWith`                                      | No            | Maybe        |
| - `EndsWith`                                        | No            | Maybe        |
| - `ContainsSubstring`                               | Yes (5)       | Done         |
| - `Equals`                                          | No            | Maybe        |
| - `Matches`                                         | No            | Unlikely     |
| **Vector matchers**                                 |               |              |
| - `Contains`                                        | No            | No           |
| - `VectorContains`                                  | No            | No           |
| - `Equals`                                          | No            | No           |
| - `UnorderedEquals`                                 | No            | No           |
| - `Approx`                                          | No            | No           |
| **Generic range matchers**                          |               |              |
| - `IsEmpty`                                         | No            | Maybe        |
| - `SizeIs`                                          | No            | Maybe        |
| - `Contains`                                        | No            | Maybe        |
| - `AllMatch`                                        | No            | Maybe        |
| - `NoneMatch`                                       | No            | Maybe        |
| - `AnyMatch`                                        | No            | Maybe        |
| - `AllTrue`                                         | No            | Maybe        |
| - `NoneTrue`                                        | No            | Maybe        |
| - `AnyTrue`                                         | No            | Maybe        |
| **Other matchers**                                  |               |              |
| - `Predicate`                                       | No            | Unlikely     |
| - `Message`                                         | Yes (6)       | Done         |
| **Miscelaneous**                                    |               |              |
| - `BENCHMARK`                                       | No            | No           |
| - `BENCHMARK_ADVANCED`                              | No            | No           |
| - `GENERATE`                                        | No            | No           |
| - `REGISTER_LISTENER`                               | Yes (7)       | Done         |

**Notes:**
 1. Tags are not optional in _snitch_. This may be fixed later, if requested.
 2. Support for hidden tests (`[.]` or `[.tag]`), `[!mayfail]`, and `[!shouldfail]` only.
 3. No streaming in _snitch_. For example, `INFO("the number is " << i)` is not supported. Supporting this is not on the roadmap.
 4. See [the README](README.md#matchers) for differences between _Catch2_ and _snitch_ matchers.
 5. Spelled `snitch::matchers::contains_substring`.
 6. Spelled `snitch::matchers::with_what_contains`, and does substring matching (not exact matching). It does not require the exception type to inherit from `std::exception`, just to have a member function `what()` that returns an object convertible to `std::string_view`.
 7. Supported only in a limited form; use `registry::report_callback` and set it up to call all your event listeners, as needed. Improving this is not on the roadmap.
 8. If supported, it will not use the streaming syntax.

**Roadmap:**
 - "Yes" is something that we want to eventually support in _snitch_, even if it comes at a cost (at run time or compile time). Contributions are welcome.
 - "Maybe" is something that we have no objection against supporting, but for which we have no immediate need. It may be refused if the cost is significant. Contributions are welcome, but expect some extra scrutiny, and perhaps push back.
 - "Unlikely" is something we would want to support, but is expected to have too high cost, or seems impossible without violating other requirements of _snitch_ (e.g., no heap allocation). This can be included in _snitch_ if the cost ends up beeing negligible. We recommend discussing with the maintainers before contributing.
 - "No" is something we explicitly do not want to support. This can still be included in _snitch_ if well motivated, but please argue your case with the maintainers before contributing.
