#include "ExceptionContainsWhat.h"
#include "FrontEnd.h"
#include "ParseError.h"
#include "ParserHelper.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("SIP Parser: conditionals", "[SIP Parser]") {
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

// start boolean test cases
TEST_CASE("SIP Parser: booleans", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x, y;
        x = true;
        y = false;
        return x;
      }
    )";

  REQUIRE(ParserHelper::is_parsable(stream));
}
/*
TEST_CASE("SIP Parser: wrong booleans", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x, y;
        x = False;
        y = True;
        return x;
      }
    )";

  REQUIRE_FALSE(ParserHelper::is_parsable(stream));
}
should be part of a test case later
 */

TEST_CASE("SIP Parser: boolean operators", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x, y;
        x = true;
        y = false;
        if ( not x and not ( y or x )) {
           y = x or y;
        }
        return true;
      }
    )";

  REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: logical not higher precedence than and. not first item", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(main() { return not x and p; })";
  std::string expected = "(expr (expr not (expr x)) and (expr p))";
  std::string tree = ParserHelper::parsetree(stream);
  REQUIRE(tree.find(expected) != std::string::npos);
}

TEST_CASE("SIP Parser: logical not higher precedence than and. not second item", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(main() { return x and not p; })";
  std::string expected = "(expr (expr x) and (expr not (expr p)))";
  std::string tree = ParserHelper::parsetree(stream);
  REQUIRE(tree.find(expected) != std::string::npos);
}

TEST_CASE("SIP Parser: logical not higher precedence than or. not first item", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(main() { return not x or p; })";
  std::string expected = "(expr (expr not (expr x)) or (expr p))";
  std::string tree = ParserHelper::parsetree(stream);
  REQUIRE(tree.find(expected) != std::string::npos);
}

TEST_CASE("SIP Parser: logical not higher precedence than or. not second item", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(main() { return x or not p; })";
  std::string expected = "(expr (expr x) or (expr not (expr p)))";
  std::string tree = ParserHelper::parsetree(stream);
  REQUIRE(tree.find(expected) != std::string::npos);
}

TEST_CASE("SIP Parser: 'and' logical precedence same as 'or', 'and' first", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(main() { return x and p or y; })";
  std::string expected = "(expr (expr (expr x) and (expr p)) or (expr y))";
  std::string tree = ParserHelper::parsetree(stream);
  REQUIRE(tree.find(expected) != std::string::npos);
}

TEST_CASE("SIP Parser: 'and' logical precedence same as 'or', 'or' first", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(main() { return x or p and y; })";
  std::string expected = "(expr (expr (expr x) or (expr p)) and (expr y))";
  std::string tree = ParserHelper::parsetree(stream);
  REQUIRE(tree.find(expected) != std::string::npos);
}

// end boolean cases

// start for loop tests
TEST_CASE("SIP Parser: for loop, iterator type", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x, i;
        for ( x : 32) {
          x = x;
        }
        for ( 32 : 32){
          while (x > 20){
             x = x;
          }
        }
        for (x : 12 ){
          for ( x : i ){
              x = x;
           }
        }
      return x;
      }
    )";

  REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: for loop, range type", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x, i;
        for ( x : i + 8 .. i by 2){
          x = i;
        }
        return x;
       }
    )";

  REQUIRE(ParserHelper::is_parsable(stream));
}