#include "ExceptionContainsWhat.h"
#include "FrontEnd.h"
#include "ParseError.h"
#include "ParserHelper.h"

#include <catch2/catch_test_macros.hpp>


TEST_CASE("SIP Parser: Conditionals positive case", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var iter, limit, answer;
        iter = 0;
        limit = 50;
        answer = 0;
        while (iter <= limit) {
          answer = answer + iter;
          if (answer + iter < 100) {
            answer = answer - 2;
          }
          if (answer + iter > 4) {
            answer = answer + 8;
          }
          if (answer + iter >= 24) {
            answer = answer * iter;
          }
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
        if ( <= 50) {
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

TEST_CASE("SIP Parser: Incrementor/Decrementor Positive Case", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      incorrectSquare() {
        var number, square, counter;
        number = 5;
        square = 0;
        counter = number;
        while (counter < number) {
          square = square + number;
          counter++;
        }
        square--;
        return square;
      }
    )";

  REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: Incrementor/Decrementor Bad Syntax Case", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
        var x, y, z;
        x = 0;
        y = -1;
        z = 400;
        --x;
        ++x;
      }
    )";

  REQUIRE_FALSE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: Incrementor/Decrementor Expression Case", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
        var x, y, z;
        x = 0;
        y = -1;
        z = 400;
        return x+y++;
      }
    )";

  REQUIRE_FALSE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: Increment must be statement", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(main() { var x; x = 1; return (x++) * 6; })";
  REQUIRE_FALSE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: Ternary Operator Positive Case", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
        var x, y, z;
        x = 9;
        y = -1;
        z = 400;
        return x > y ? y + 3 : y * 54;
      }
    )";

  REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: Nested Ternary Operators", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
        var x, y, z;
        x = 9;
        y = -1;
        z = 400;
        return x > y ?
                y > z ? y - z : y + z
                : x <= z ? z * x : x + y + z * z;
      }
    )";

  REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: Modulus Positive Case", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
        var x, y;
        x = 5;
        y = 2;
        return (x + 5) % (y + 6);
      }
    )";
  REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: Arithmetic Negation Positive Case", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
        var x, y;
        x = 5;
        y = 2;
        return -((x + 5) % (y + 6));
      }
    )";
  REQUIRE(ParserHelper::is_parsable(stream));
}

/* These tests checks for operator precedence.
 * They access the parse tree and ensure that the higher precedence
 * operator is nested more deeply than the lower precedence operator.
 */
TEST_CASE("SIP Parser: Modulus higher precedence than ternary", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(main() { var x; x = 1; return x % x ? 2 : 6; })";
  std::string expected = "(expr (expr (expr x) % (expr x)) ? (expr 2) : (expr 6))";
  std::string tree = ParserHelper::parsetree(stream);
  REQUIRE(tree.find(expected) != std::string::npos);
}

TEST_CASE("SIP Parser: Modulus higher precedence than add", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(main() { return 1 + 2 % 3; })";
  std::string expected = "(expr (expr 1) + (expr (expr 2) % (expr 3)))";
  std::string tree = ParserHelper::parsetree(stream);
  REQUIRE(tree.find(expected) != std::string::npos);
}

TEST_CASE("SIP Parser: Modulus same precedence as Multiply", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(main() { return 1 * 2 % 3; })";
  //multiply expression is higher because it comes first here
  std::string expected = "(expr (expr (expr 1) * (expr 2)) % (expr 3))";
  std::string tree = ParserHelper::parsetree(stream);
  REQUIRE(tree.find(expected) != std::string::npos);
}

TEST_CASE("SIP Parser: Arithmetic negation higher precedence than modulus", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(main() { var x; x = 3; return -x % 3; })";
  std::string expected = "(expr (expr - (expr x)) % (expr 3))";
  std::string tree = ParserHelper::parsetree(stream);
  REQUIRE(tree.find(expected) != std::string::npos);
}