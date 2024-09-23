#include "ExceptionContainsWhat.h"
#include "FrontEnd.h"
#include "ParseError.h"
#include "ParserHelper.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("SIP Parser: LTE conditional positive case", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var iter, limit, answer;
        iter = 0;
        limit = 50;
        answer = 0;
        while ( iter <= 50) {
          answer = answer + iter;
          iter = iter + 1;
        }
        return answer;
      }
    )";

  REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: LTE conditional negative case", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x, y, z;
        y = -1;
        z = 1;
        x = 50;
        if (<50) {
          z = y;
        }
        else {
          y = z;
        }
        return y;
      }
    )";

  REQUIRE_FALSE(ParserHelper::is_parsable(stream));
}

