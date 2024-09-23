#include "ExceptionContainsWhat.h"
#include "FrontEnd.h"
#include "ParseError.h"
#include "ParserHelper.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("SIP Parser: conditionals", "[TIP Parser]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var iter, limit, answer;
        iter = 0;
        limit = 50;
        answer = 0;
        while ( i <= 50) {
          answer = answer + iter;
        }
        return answer;
      }
    )";

  REQUIRE(ParserHelper::is_parsable(stream));
}
