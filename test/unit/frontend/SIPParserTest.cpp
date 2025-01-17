#include "ExceptionContainsWhat.h"
#include "FrontEnd.h"
#include "ParseError.h"
#include "ParserHelper.h"

#include <catch2/catch_test_macros.hpp>

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

TEST_CASE("SIP Parser: 'and' logical precedence higher than 'or', 'and' first", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(main() { return x and p or y; })";
  std::string expected = "(expr (expr (expr x) and (expr p)) or (expr y))";
  std::string tree = ParserHelper::parsetree(stream);
  REQUIRE(tree.find(expected) != std::string::npos);
}

TEST_CASE("SIP Parser: 'and' logical precedence higher than 'or', 'or' first", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(main() { return x or p and y; })";
  std::string expected = "(expr (expr x) or (expr (expr p) and (expr y)))";
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


//start conditionals testing without precedence
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
// end consitional testing without precedence


//start incrementer testing without precedence
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

TEST_CASE("SIP Parser: Repeated Incrementor/Decrementor", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
        var x;
        x = 0;
        x++ ++;
        return x;
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
//end conditionals testing without precedence

//start ternary operator testing without precedence
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
//end ternary operator testing without precedence

//start modulus testing without precedence
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

//end modulus testing without precedence

//start arithmetic negation testing without precedence
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

TEST_CASE("SIP Parser: Repeated Arithmetic Negation", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
        var x;
        x = - - 3;
        return x == 3;
      }
    )";
  REQUIRE(ParserHelper::is_parsable(stream));
}
//end arithmetic negation testing without precedence

//start array testing without precedence
TEST_CASE("SIP Parser: Array explicit constructor Positive Case", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
        var x;
        x = [0, 1, 2, 3, 4, 5];
        return x[1];
      }
    )";
  REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: Array explicit constructor no elements Positive Case", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
        var x;
        x = [];
        return 10;
      }
    )";
  REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: Array explicit constructor single element Positive Case", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
        var x, y;
        x = [123];
        return x[0];
      }
    )";
  REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: Array explicit constructor hanging comma case", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
        var x;
        x = [0, 1, 2, 3, 4, 5,];
        return x[1];
      }
    )";
  REQUIRE_FALSE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: Array range constructor Positive Case", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
        var x, y;
        x = [y of 12];
        return x[1];
      }
    )";
  REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: Variable with array constructor of keyword", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
        var x, of;
        of = 1;
        x = [y of 12];
        return of;
      }
    )";
  REQUIRE_FALSE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: Array range constructor Negative Case", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
        var x, y;
        x = [y-- of 12];
        return x[1];
      }
    )";
  REQUIRE_FALSE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: 2-D array range constructor", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
        var x, y;
        x = [[y of 12], [y of 12]];
        return x[1];
      }
    )";
  REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: multidimensional array explicit constructor", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
        var x;
        x = [[0, 1, 2], [], [0], [0, 5, [8, 90]]];
        return x[1];
      }
    )";
  REQUIRE(ParserHelper::is_parsable(stream));
}
//end array testing without precedence

//start unary array length operator testing without precedence
TEST_CASE("SIP Parser: Unary array length operator positive Case", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
        var x;
        x = [0, 1, 2, 3];
        return #x;
      }
    )";
  REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: Unary array length operator on explicit array", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
        var x;
        x = #[3];
        return x ? 1 : 0;
      }
    )";
  REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: Unary array length operator on empty array", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
        var x, checker;
        x = #[] == 0;
        checker = x == true;
        return checker;
      }
    )";
  REQUIRE(ParserHelper::is_parsable(stream));
}
//end unary array length operator testing without precedence

//start array reference operator testing without precedence
TEST_CASE("SIP Parser: Basic Array Element Reference Operator", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
         var x;
         x = [1, 2, 3, 5, -15];
         return x[3];
      }
    )";
  REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: Nested Array Element Reference Operator", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
         var x;
         x = [[0, 1, 2], [], [0], [0, 5, [8, 90]]];
         return x[3][0][2][1];
      }
    )";
  REQUIRE(ParserHelper::is_parsable(stream));
}
//end array reference operator testing without precedence

//start invalid operator testing
TEST_CASE("SIP Parser: Invalid Operator Right Shift", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
         var x;
         x = 2;
         return x >> 8;
      }
    )";
  REQUIRE_FALSE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: Invalid Operator XOR", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
         var x, y;
         x = 2;
         y = ^x;
         return y;
      }
    )";
  REQUIRE_FALSE(ParserHelper::is_parsable(stream));
}
//end invalid operator testing

//start invalid variable name testing
TEST_CASE("SIP Parser: Invalid Variable Name #", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
         var #;
         # = 3 + 4;
         return #;
      }
    )";
  REQUIRE_FALSE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: Invalid Variable Name true", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(
      func() {
         var true;
         true = false;
         return true;
      }
    )";
  REQUIRE_FALSE(ParserHelper::is_parsable(stream));
}
//end invalid variable name testing

/* START operator precedence testing
 */
TEST_CASE("SIP Parser: Addition higher precedence than conditionals", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(main() { var x, y; x = 1; y = 4; if (x + 4 >= y) x = x - 4; return x; })";
  std::string expected = "(expr (expr (expr x) + (expr 4)) >= (expr y))";
  std::string tree = ParserHelper::parsetree(stream);
  REQUIRE(tree.find(expected) != std::string::npos);
}

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

TEST_CASE("SIP Parser: Modulus same precedence as Multiply, multiply first", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(main() { return 1 * 2 % 3; })";
  //multiply expression is higher because it comes first here
  std::string expected = "(expr (expr (expr 1) * (expr 2)) % (expr 3))";
  std::string tree = ParserHelper::parsetree(stream);
  REQUIRE(tree.find(expected) != std::string::npos);
}

TEST_CASE("SIP Parser: Modulus same precedence as Multiply, mod first", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(main() { return 1 % 2 * 3; })";
  //multiple expression is lower because it comes second here
  std::string expected = "(expr (expr (expr 1) % (expr 2)) * (expr 3))";
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

TEST_CASE("SIP Parser: Prefix length operator higher precedence than modulus", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(main() { var x; x = [4]; return #x % 2;})";
  std::string expected = "(expr (expr # (expr x)) % (expr 2))";
  std::string tree = ParserHelper::parsetree(stream);
  REQUIRE(tree.find(expected) != std::string::npos);
}

TEST_CASE("SIP Parser: Prefix length operator lower precedence to array subscript", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(main() {
                var x;
                x = [4, 5, [6, 7, 8, 9]];
                return #x[2];
              })";
  std::string expected = "(expr # (expr (expr x) [ (expr 2) ]))";
  std::string tree = ParserHelper::parsetree(stream);
  REQUIRE(tree.find(expected) != std::string::npos);
}

TEST_CASE("SIP Parser: Array subscripting higher precedence than decrementing", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(main() { var x; x = [4]; x[0]++; return x[0];})";
  std::string expected = "(expr (expr x) [ (expr 0) ]) ++ ;)";
  std::string tree = ParserHelper::parsetree(stream);
  REQUIRE(tree.find(expected) != std::string::npos);
}

TEST_CASE("SIP Parser: Array subscripting higher precedence than arithmetic negation", "[SIP Parser]") {
  std::stringstream stream;
  stream << R"(main() { var x; x = [4]; return -x[0]; })";
  std::string expected = "(expr - (expr (expr x) [ (expr 0) ]))";
  //inner most parenthesis has array access, and then it goes out to negation
  std::string tree = ParserHelper::parsetree(stream);
  REQUIRE(tree.find(expected) != std::string::npos);
}
/* END operator precedence testing
 */