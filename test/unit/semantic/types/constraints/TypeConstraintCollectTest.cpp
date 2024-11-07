#include "ASTHelper.h"
#include "TypeHelper.h"
#include "SymbolTable.h"
#include "TypeConstraintCollectVisitor.h"
#include "Unifier.h"
#include "TipFunction.h"
#include "TipRef.h"
#include "TipArray.h"


#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <set>
#include <sstream>

/*
 * Run the front-end on the program, collect the type constraints, solve the constraints
 * and return the unifier storing the inferred types for the variables in the program.
 * This code expects that no type errors are present and throws an exception otherwise.
 */
static std::pair<Unifier, std::shared_ptr<SymbolTable>> collectAndSolve(std::stringstream &program) {
    auto ast = ASTHelper::build_ast(program);
    auto symbols = SymbolTable::build(ast.get());

    TypeConstraintCollectVisitor visitor(symbols.get());
    ast->accept(&visitor);

    auto collected = visitor.getCollectedConstraints();

    Unifier unifier(collected);
    REQUIRE_NOTHROW(unifier.solve());

    return std::make_pair(unifier, symbols);
}
// begin sip boolean tests

TEST_CASE("TypeConstraintVisitor: false, true, binary expressions 'and', and < > != etc",
          "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
            // [[x]] = int, [[y]] = int, [[test]] = () -> int
            test() {
              var x, y;
              x = false;
              y = true;
              x = x != y;
              y = x == y;
              x = 2 < 10;
              x = 2 <= 10;
              x = 2 >= 10;
              x = 2 >= 10;
              y = x and y;
              x = x or y;
              return y;
            }
         )";

  auto unifierSymbols = collectAndSolve(program);
  auto unifier = unifierSymbols.first;
  auto symbols = unifierSymbols.second;

  std::vector<std::shared_ptr<TipType>> empty;

  auto fDecl = symbols->getFunction("test");
  auto fType = std::make_shared<TipVar>(fDecl);
  REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::booleanType()));

  auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
  REQUIRE(*unifier.inferred(xType) == *TypeHelper::booleanType());

  auto yType = std::make_shared<TipVar>(symbols->getLocal("y", fDecl));
  REQUIRE(*unifier.inferred(yType) == *TypeHelper::booleanType());
}

TEST_CASE("TypeConstraintVisitor: more boolean testing, ifs and conditionals",
          "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
            // [[x]] = int, [[y]] = int, [[test]] = () -> int
            test() {
              var x, y;
              x = false;
              y = true;
              if (x)
                y = false;
              while(y)
                x = true;
              return y;
            }
         )";

  auto unifierSymbols = collectAndSolve(program);
  auto unifier = unifierSymbols.first;
  auto symbols = unifierSymbols.second;

  std::vector<std::shared_ptr<TipType>> empty;

  auto fDecl = symbols->getFunction("test");
  auto fType = std::make_shared<TipVar>(fDecl);
  REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::booleanType()));

  auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
  REQUIRE(*unifier.inferred(xType) == *TypeHelper::booleanType());

  auto yType = std::make_shared<TipVar>(symbols->getLocal("y", fDecl));
  REQUIRE(*unifier.inferred(yType) == *TypeHelper::booleanType());
}

TEST_CASE("TypeConstraintVisitor: ternary expressions",
          "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
            // [[x]] = int, [[y]] = int, [[test]] = () -> int
            test() {
              var x, y;
              x = true;
              y = false;
              y = x ? false and true : false or true;
              return y;
            }
         )";

  auto unifierSymbols = collectAndSolve(program);
  auto unifier = unifierSymbols.first;
  auto symbols = unifierSymbols.second;

  std::vector<std::shared_ptr<TipType>> empty;

  auto fDecl = symbols->getFunction("test");
  auto fType = std::make_shared<TipVar>(fDecl);
  REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::booleanType()));

  auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
  REQUIRE(*unifier.inferred(xType) == *TypeHelper::booleanType());

  auto yType = std::make_shared<TipVar>(symbols->getLocal("y", fDecl));
  REQUIRE(*unifier.inferred(yType) == *TypeHelper::booleanType());
}

TEST_CASE("TypeConstraintVisitor: logical not tests",
          "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
            // [[x]] = int, [[y]] = int, [[test]] = () -> int
            test() {
              var x, y;
              x = not true;
              y = not false;
              y = not ( x and false);
              return y;
            }
         )";

  auto unifierSymbols = collectAndSolve(program);
  auto unifier = unifierSymbols.first;
  auto symbols = unifierSymbols.second;

  std::vector<std::shared_ptr<TipType>> empty;

  auto fDecl = symbols->getFunction("test");
  auto fType = std::make_shared<TipVar>(fDecl);
  REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::booleanType()));

  auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
  REQUIRE(*unifier.inferred(xType) == *TypeHelper::booleanType());

  auto yType = std::make_shared<TipVar>(symbols->getLocal("y", fDecl));
  REQUIRE(*unifier.inferred(yType) == *TypeHelper::booleanType());

}

TEST_CASE("TypeConstraintVisitor: array len",
          "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
            test() {
              var x, y;
              x = [ false, true, true, true];
              y = #x;
              return y;
            }
         )";

  auto unifierSymbols = collectAndSolve(program);
  auto unifier = unifierSymbols.first;
  auto symbols = unifierSymbols.second;

  std::vector<std::shared_ptr<TipType>> empty;

  auto fDecl = symbols->getFunction("test");
  auto fType = std::make_shared<TipVar>(fDecl);
  REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::intType()));

  auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
  REQUIRE(*unifier.inferred(xType) == *TypeHelper::arrayType(TypeHelper::booleanType()));

  auto yType = std::make_shared<TipVar>(symbols->getLocal("y", fDecl));
  REQUIRE(*unifier.inferred(yType) == *TypeHelper::intType());

}

TEST_CASE("TypeConstraintVisitor: array ref",
          "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
            test() {
              var x, y;
              x = [ true, false, true];
              y = x[1];
              return y;
            }
         )";

  auto unifierSymbols = collectAndSolve(program);
  auto unifier = unifierSymbols.first;
  auto symbols = unifierSymbols.second;

  std::vector<std::shared_ptr<TipType>> empty;

  auto fDecl = symbols->getFunction("test");
  auto fType = std::make_shared<TipVar>(fDecl);
  REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::booleanType()));

  auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
  REQUIRE(*unifier.inferred(xType) == *TypeHelper::arrayType(TypeHelper::booleanType()));

  auto yType = std::make_shared<TipVar>(symbols->getLocal("y", fDecl));
  REQUIRE(*unifier.inferred(yType) == *TypeHelper::booleanType());

}
//ending sip :(

TEST_CASE("TypeConstraintVisitor: input, const, arithmetic, return type",
          "[TypeConstraintVisitor]") {
    std::stringstream program;
    program << R"(
            // [[x]] = int, [[y]] = int, [[test]] = () -> int
            test() {
              var x, y;
              x = input;
              y = 3 + x;
              return y;
            }
         )";

    auto unifierSymbols = collectAndSolve(program);
    auto unifier = unifierSymbols.first;
    auto symbols = unifierSymbols.second;

    std::vector<std::shared_ptr<TipType>> empty;

    auto fDecl = symbols->getFunction("test");
    auto fType = std::make_shared<TipVar>(fDecl);
    REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::intType()));

    auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
    REQUIRE(*unifier.inferred(xType) == *TypeHelper::intType());

    auto yType = std::make_shared<TipVar>(symbols->getLocal("y", fDecl));
    REQUIRE(*unifier.inferred(yType) == *TypeHelper::intType());
}

TEST_CASE("TypeConstraintVisitor: alloc, deref, assign through ptr",
          "[TypeConstraintVisitor]") {
    std::stringstream program;
    program << R"(
            // [[x]] = int, [[y]] = ptr to int, [[test]] = () -> ptr to int
            test() {
                var x,y,z;
                x = input;
                y = alloc x;
                *y = x;
                return y;
            }
         )";

    auto unifierSymbols = collectAndSolve(program);
    auto unifier = unifierSymbols.first;
    auto symbols = unifierSymbols.second;

    std::vector<std::shared_ptr<TipType>> empty;

    auto fDecl = symbols->getFunction("test");
    auto fType = std::make_shared<TipVar>(fDecl);
    REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::ptrType(TypeHelper::intType())));

    auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
    REQUIRE(*unifier.inferred(xType) == *TypeHelper::intType());

    auto yType = std::make_shared<TipVar>(symbols->getLocal("y", fDecl));
    REQUIRE(*unifier.inferred(yType) == *TypeHelper::ptrType(TypeHelper::intType()));
}



TEST_CASE("TypeConstraintVisitor: function reference, address of",
          "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
      // [[foo]] = [[x]] = () -> int), [[y]] = ptr to () -> int
      foo() {
        var x, y;
        x = foo;
        y = &x;
        return 13;
      }
    )";

    auto unifierSymbols = collectAndSolve(program);
    auto unifier = unifierSymbols.first;
    auto symbols = unifierSymbols.second;

    std::vector<std::shared_ptr<TipType>> empty;

    auto fDecl = symbols->getFunction("foo");
    auto fType = std::make_shared<TipVar>(fDecl);
    REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::intType()));

    auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
    REQUIRE(*unifier.inferred(xType) == *unifier.inferred(fType));

    auto yType = std::make_shared<TipVar>(symbols->getLocal("y", fDecl));
    REQUIRE(*unifier.inferred(yType) == *TypeHelper::ptrType(TypeHelper::funType(empty, TypeHelper::intType())));

}

TEST_CASE("TypeConstraintVisitor: relop, if ", "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
      // [[x]] = int, [[y]] = int, [[test]] = (int) -> int
      test(x) {
        var y;
        if (x > 0) {
          y = 0;
        } else {
          y = 1;
        }
        return y;
      }
    )";

    auto unifierSymbols = collectAndSolve(program);
    auto unifier = unifierSymbols.first;
    auto symbols = unifierSymbols.second;

    std::vector<std::shared_ptr<TipType>> oneInt{TypeHelper::intType()};

    auto fDecl = symbols->getFunction("test");
    auto fType = std::make_shared<TipVar>(fDecl);
    REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(oneInt, TypeHelper::intType()));

    auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
    REQUIRE(*unifier.inferred(xType) == *TypeHelper::intType());

    auto yType = std::make_shared<TipVar>(symbols->getLocal("y", fDecl));
    REQUIRE(*unifier.inferred(yType) == *TypeHelper::intType());
}

TEST_CASE("TypeConstraintVisitor: while ", "[TypeConstraintVisitor]") {
    std::stringstream program;
    program << R"(
      // [[x]] = int, [[test]] = (int) -> int
      test(x) {
        while (x > 0) {
          x = x - 1;
        }
        return x;
      }
    )";

    auto unifierSymbols = collectAndSolve(program);
    auto unifier = unifierSymbols.first;
    auto symbols = unifierSymbols.second;

    std::vector<std::shared_ptr<TipType>> oneInt{TypeHelper::intType()};

    auto fDecl = symbols->getFunction("test");
    auto fType = std::make_shared<TipVar>(fDecl);
    REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(oneInt, TypeHelper::intType()));

    auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
    REQUIRE(*unifier.inferred(xType) == *TypeHelper::intType());
}

TEST_CASE("TypeConstraintVisitor: error, output", "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
      // [[x]] = int, [[y]] = int, [[test]] = (int,int) -> int
      test(x, y) {
        output x;
        error y;
        return 0;
      }
    )";

    auto unifierSymbols = collectAndSolve(program);
    auto unifier = unifierSymbols.first;
    auto symbols = unifierSymbols.second;

    std::vector<std::shared_ptr<TipType>> twoInt{TypeHelper::intType(), TypeHelper::intType()};

    auto fDecl = symbols->getFunction("test");
    auto fType = std::make_shared<TipVar>(fDecl);
    REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(twoInt, TypeHelper::intType()));

    auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
    REQUIRE(*unifier.inferred(xType) == *TypeHelper::intType());

    auto yType = std::make_shared<TipVar>(symbols->getLocal("y", fDecl));
    REQUIRE(*unifier.inferred(yType) == *TypeHelper::intType());
}

TEST_CASE("TypeConstraintVisitor: funs with params",
          "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
      // [[x]] = int, [[foo]] = (int) -> int
      foo(x) {
        return x;
      }
      // [[bar]] = ()->int
      bar() {
        return foo(7);
      }
    )";

    auto unifierSymbols = collectAndSolve(program);
    auto unifier = unifierSymbols.first;
    auto symbols = unifierSymbols.second;

    std::vector<std::shared_ptr<TipType>> oneInt{TypeHelper::intType()};
    std::vector<std::shared_ptr<TipType>> empty;

    auto fooDecl = symbols->getFunction("foo");
    auto fooType = std::make_shared<TipVar>(fooDecl);
    REQUIRE(*unifier.inferred(fooType) == *TypeHelper::funType(oneInt, TypeHelper::intType()));

    auto barDecl = symbols->getFunction("bar");
    auto barType = std::make_shared<TipVar>(barDecl);
    REQUIRE(*unifier.inferred(barType) == *TypeHelper::funType(empty, TypeHelper::intType()));

    auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fooDecl));
    REQUIRE(*unifier.inferred(xType) == *TypeHelper::intType());
}


TEST_CASE("TypeConstraintVisitor: main", "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
      // [[x]] = int, [[foo]] = (int) -> int
      main(x) {
        return x;
      }
    )";

    auto unifierSymbols = collectAndSolve(program);
    auto unifier = unifierSymbols.first;
    auto symbols = unifierSymbols.second;

    std::vector<std::shared_ptr<TipType>> oneInt{TypeHelper::intType()};

    auto fDecl = symbols->getFunction("main");
    auto fType = std::make_shared<TipVar>(fDecl);
    REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(oneInt, TypeHelper::intType()));

    auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
    REQUIRE(*unifier.inferred(xType) == *TypeHelper::intType());
}

TEST_CASE("TypeConstraintVisitor: polymorphic type inference",
          "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
      // [[p]] = ptr to \alpha, [[deref]] = (ptr to \alpha) -> \alpha
      deref(p) {
        return *p;
      }
    )";

    auto unifierSymbols = collectAndSolve(program);
    auto unifier = unifierSymbols.first;
    auto symbols = unifierSymbols.second;

    auto fDecl = symbols->getFunction("deref");
    auto pDecl = symbols->getLocal("p", fDecl);

    std::vector<std::shared_ptr<TipType>> onePtrToAlpha{TypeHelper::ptrType(TypeHelper::alphaType(pDecl))};


    // Equality on alpha type variables considers the actual AST node used to generate the alpha, but we
    // only want to check that the types are some alpha -- we don't care which one.  This is a bit clunky.
    auto fType = std::make_shared<TipVar>(fDecl);
    auto funType = std::dynamic_pointer_cast<TipFunction>(unifier.inferred(fType));
    REQUIRE(funType != nullptr); // needs to be a function type

    // return type is an alpha
    auto returnType = funType->getReturnType();
    REQUIRE(Unifier::isAlpha(returnType));

    // argument type is pointer to an alpha
    auto refType = std::dynamic_pointer_cast<TipRef>(funType->getParamTypes()[0]);
    REQUIRE(refType != nullptr);
    REQUIRE(Unifier::isAlpha(refType->getReferencedType()));

    // Now we want the argument p to have the same type as the parameter type
    auto pType = std::make_shared<TipVar>(pDecl);
    REQUIRE(*unifier.inferred(pType) == *refType);

}



TEST_CASE("TypeConstraintVisitor: access expr", "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
      // [[main]] = ()->int, [[r]] = {f:int, g:int}
      main() {
          var r;
          r = {f: 4, g: 13};
          return r.g;
      }

    )";

    auto unifierSymbols = collectAndSolve(program);
    auto unifier = unifierSymbols.first;
    auto symbols = unifierSymbols.second;

    std::vector<std::shared_ptr<TipType>> empty;

    auto fDecl = symbols->getFunction("main");
    auto fType = std::make_shared<TipVar>(fDecl);
    REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::intType()));

    std::vector<std::shared_ptr<TipType>> twoInts{TypeHelper::intType(), TypeHelper::intType()};
    std::vector<std::string> twoNames{"f", "g"};

    auto xType = std::make_shared<TipVar>(symbols->getLocal("r", fDecl));
    REQUIRE(*unifier.inferred(xType) == *TypeHelper::recType(twoInts, twoNames));

}

TEST_CASE("TypeConstraintVisitor: global record", "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
      // [[foo]] = ()->int, [[r1]] = {f:int, g:int, n:absent}, [[r2]] = {f:int, g:absent, n: ptr to int}
      foo() {
          var r1, r2;
          r1 = {f: 4, g: 13};
          r2 = {n: alloc 3, f: 13};
          return 0;
      }

    )";


    auto unifierSymbols = collectAndSolve(program);
    auto unifier = unifierSymbols.first;
    auto symbols = unifierSymbols.second;

    std::vector<std::shared_ptr<TipType>> empty;

    auto fDecl = symbols->getFunction("foo");
    auto fType = std::make_shared<TipVar>(fDecl);
    REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::intType()));

    std::vector<std::shared_ptr<TipType>> twoIntsAbsent{TypeHelper::intType(), TypeHelper::intType(), TypeHelper::absentType()};
    std::vector<std::string> threeNames{"f", "g", "n"};

    auto xType = std::make_shared<TipVar>(symbols->getLocal("r1", fDecl));
    REQUIRE(*unifier.inferred(xType) == *TypeHelper::recType(twoIntsAbsent, threeNames));

    std::vector<std::shared_ptr<TipType>> intAbsentPtrInt{TypeHelper::intType(), TypeHelper::absentType(), TypeHelper::ptrType(TypeHelper::intType())};

    auto r1Type = std::make_shared<TipVar>(symbols->getLocal("r2", fDecl));
    REQUIRE(*unifier.inferred(r1Type) == *TypeHelper::recType(intAbsentPtrInt, threeNames));

}

TEST_CASE("TypeConstraintVisitor: record2", "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
// [[main]] = ()->int, [[n]] = ptr to record(p:int,q:int), [[r1]] = int
main() {
    var n, r1;
    n = alloc {p: 4, q: 2};
    *n = {p:5, q: 6};
    r1 = (*n).p;
    output r1;
    return 0;
}
    )";

    auto unifierSymbols = collectAndSolve(program);
    auto unifier = unifierSymbols.first;
    auto symbols = unifierSymbols.second;

    std::vector<std::shared_ptr<TipType>> empty;

    auto fDecl = symbols->getFunction("main");
    auto fType = std::make_shared<TipVar>(fDecl);
    REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::intType()));

    std::vector<std::shared_ptr<TipType>> twoInts{TypeHelper::intType(), TypeHelper::intType()};
    std::vector<std::string> twoNames{"p", "q"};
    auto nType = std::make_shared<TipVar>(symbols->getLocal("n", fDecl));
    REQUIRE(*unifier.inferred(nType) == *TypeHelper::ptrType(TypeHelper::recType(twoInts, twoNames)));

    auto r1Type = std::make_shared<TipVar>(symbols->getLocal("r1", fDecl));
    REQUIRE(*unifier.inferred(r1Type) == *TypeHelper::intType());
}

TEST_CASE("TypeConstraintVisitor: record4", "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
// [[main]] = ()->int
// [[n]] = record(a:absent, b: absent, c: ptr(record(a:int,b:int,c:absent,d:absent), d:int)
// [[k]] = record(a:int,b:int,c:absent,d:absent)
// [[r1]] = int
main() {
    var n, k, r1;
    k = {a: 1, b: 2};
    n = {c: &k, d: 4};
    r1 = ((*(n.c)).a); // output 1
    output r1;
    return 0;
}
    )";

    auto unifierSymbols = collectAndSolve(program);
    auto unifier = unifierSymbols.first;
    auto symbols = unifierSymbols.second;

    std::vector<std::shared_ptr<TipType>> empty;

    auto fDecl = symbols->getFunction("main");
    auto fType = std::make_shared<TipVar>(fDecl);
    REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::intType()));

    std::vector<std::shared_ptr<TipType>> twoIntsTwoAbsent{TypeHelper::intType(), TypeHelper::intType(), TypeHelper::absentType(), TypeHelper::absentType()};
    std::vector<std::string> fieldNames{"a", "b", "c", "d"};
    auto kType = std::make_shared<TipVar>(symbols->getLocal("k", fDecl));
    REQUIRE(*unifier.inferred(kType) == *TypeHelper::recType(twoIntsTwoAbsent, fieldNames));

    std::vector<std::shared_ptr<TipType>> twoAbsentsPtrRecInt{TypeHelper::absentType(), TypeHelper::absentType(),
                                                              TypeHelper::ptrType(TypeHelper::recType(twoIntsTwoAbsent, fieldNames)),
                                                              TypeHelper::intType()};
    auto nType = std::make_shared<TipVar>(symbols->getLocal("n", fDecl));
    REQUIRE(*unifier.inferred(nType) == *TypeHelper::recType(twoAbsentsPtrRecInt, fieldNames));

    auto r1Type = std::make_shared<TipVar>(symbols->getLocal("r1", fDecl));
    REQUIRE(*unifier.inferred(r1Type) == *TypeHelper::intType());
}

//SIP Extension
TEST_CASE("TypeConstraintVisitor: ASTNegExpr (-E)",
           "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
    //[[x]] = int, [[y]] = int, [[z]] = int, [[test]] = () -> int
    test() {
      var x, y;
      x = 4;
      y = -x;
      return y;
    }
    )";

    auto unifierSymbols = collectAndSolve(program);
    auto unifier = unifierSymbols.first;
    auto symbols = unifierSymbols.second;

    std::vector<std::shared_ptr<TipType>> empty;

    auto fDecl = symbols->getFunction("test");
    auto fType = std::make_shared<TipVar>(fDecl);
//    std::cout << *unifier.inferred(fType) << "\n";
    REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::intType()));

    auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
    REQUIRE(*unifier.inferred(xType) == *TypeHelper::intType());

    auto yType = std::make_shared<TipVar>(symbols->getLocal("y", fDecl));
    REQUIRE(*unifier.inferred(yType) == *TypeHelper::intType());
}

TEST_CASE("TypeConstraintVisitor: ASTIncrementStmt (E++)",
           "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
    //[[x]] = int, [[test]] = () -> int
    test() {
      var x;
      x = 4;
      x++;
      return x;
    }
    )";

  auto unifierSymbols = collectAndSolve(program);
  auto unifier = unifierSymbols.first;
  auto symbols = unifierSymbols.second;

  std::vector<std::shared_ptr<TipType>> empty;

  auto fDecl = symbols->getFunction("test");
  auto fType = std::make_shared<TipVar>(fDecl);
  //    std::cout << *unifier.inferred(fType) << "\n";
  REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::intType()));

  auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
  REQUIRE(*unifier.inferred(xType) == *TypeHelper::intType());
}

TEST_CASE("TypeConstraintVisitor: ASTDecrementStmt (E--)",
           "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
    //[[x]] = int, [[test]] = () -> int
    test() {
      var x;
      x = 4;
      x--;
      return x;
    }
    )";

  auto unifierSymbols = collectAndSolve(program);
  auto unifier = unifierSymbols.first;
  auto symbols = unifierSymbols.second;

  std::vector<std::shared_ptr<TipType>> empty;

  auto fDecl = symbols->getFunction("test");
  auto fType = std::make_shared<TipVar>(fDecl);
  REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::intType()));

  auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
  REQUIRE(*unifier.inferred(xType) == *TypeHelper::intType());
}

TEST_CASE("TypeConstraintVisitor: ASTArrayExpr ([E1, E2, ..., En]) int array",
           "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
    //[[x]] = int array, [[test]] = () -> int
    test() {
      var x;
      x = [1, 2, 3, 4, 5];
      return 0;
    }
    )";

  auto unifierSymbols = collectAndSolve(program);
  auto unifier = unifierSymbols.first;
  auto symbols = unifierSymbols.second;

  std::vector<std::shared_ptr<TipType>> empty;

  auto fDecl = symbols->getFunction("test");
  auto fType = std::make_shared<TipVar>(fDecl);
  REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::intType()));

  auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
  REQUIRE(*unifier.inferred(xType) == *TypeHelper::arrayType(TypeHelper::intType()));
}

TEST_CASE("TypeConstraintVisitor: ASTArrayExpr ([E1, E2, ..., En]) boolean array",
           "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
    //[[x]] = bool array, [[test]] = () -> int
    test() {
      var x;
      x = [true, false, true, true, false];
      return 0;
    }
    )";

  auto unifierSymbols = collectAndSolve(program);
  auto unifier = unifierSymbols.first;
  auto symbols = unifierSymbols.second;

  std::vector<std::shared_ptr<TipType>> empty;

  auto fDecl = symbols->getFunction("test");
  auto fType = std::make_shared<TipVar>(fDecl);
  REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::intType()));

  auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
  REQUIRE(*unifier.inferred(xType) == *TypeHelper::arrayType(TypeHelper::booleanType()));
}

TEST_CASE("TypeConstraintVisitor: ASTArrayExpr ([E1, E2, ..., En]) null array",
           "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
    //[[x]] = null array, [[test]] = () -> int
    test() {
      var x;
      x = [];
      return 0;
    }
    )";

  auto unifierSymbols = collectAndSolve(program);
  auto unifier = unifierSymbols.first;
  auto symbols = unifierSymbols.second;

  std::vector<std::shared_ptr<TipType>> empty;

  auto fDecl = symbols->getFunction("test");
  auto fType = std::make_shared<TipVar>(fDecl);
  REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::intType()));

  auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
  auto arrayType = std::dynamic_pointer_cast<TipArray>(unifier.inferred(xType));
  REQUIRE(Unifier::isAlpha(arrayType->getArrayType()));
}

TEST_CASE("TypeConstraintVisitor: ASTArrayRepExpr ([E1 of E2]) int array",
           "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
    //[[x]] = int array, [[num]] = int, [[type]] = int, [[test]] = () -> int
    test() {
      var x, num, type;
      num = 10;
      type = -1;
      x = [num of type];
      return 0;
    }
    )";

  auto unifierSymbols = collectAndSolve(program);
  auto unifier = unifierSymbols.first;
  auto symbols = unifierSymbols.second;

  std::vector<std::shared_ptr<TipType>> empty;

  auto fDecl = symbols->getFunction("test");
  auto fType = std::make_shared<TipVar>(fDecl);
  REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::intType()));

  auto numType = std::make_shared<TipVar>(symbols->getLocal("num", fDecl));
  REQUIRE(*unifier.inferred(numType) == *TypeHelper::intType());

  auto typeType = std::make_shared<TipVar>(symbols->getLocal("type", fDecl));
  REQUIRE(*unifier.inferred(typeType) == *TypeHelper::intType());

  auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
  REQUIRE(*unifier.inferred(xType) == *TypeHelper::arrayType(TypeHelper::intType()));
}

TEST_CASE("TypeConstraintVisitor: ASTArrayRepExpr ([E1 of E2]) bool array",
           "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
    //[[x]] = bool array, [[num]] = int, [[value]] = bool, [[test]] = () -> int
    test() {
      var x, num, value;
      num = 1023;
      value = false;
      x = [num of value];
      return 0;
    }
    )";

  auto unifierSymbols = collectAndSolve(program);
  auto unifier = unifierSymbols.first;
  auto symbols = unifierSymbols.second;

  std::vector<std::shared_ptr<TipType>> empty;

  auto fDecl = symbols->getFunction("test");
  auto fType = std::make_shared<TipVar>(fDecl);
  REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::intType()));

  auto numType = std::make_shared<TipVar>(symbols->getLocal("num", fDecl));
  REQUIRE(*unifier.inferred(numType) == *TypeHelper::intType());

  auto valueType = std::make_shared<TipVar>(symbols->getLocal("value", fDecl));
  REQUIRE(*unifier.inferred(valueType) == *TypeHelper::booleanType());

  auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
  REQUIRE(*unifier.inferred(xType) == *TypeHelper::arrayType(TypeHelper::booleanType()));
}

TEST_CASE("TypeConstraintVisitor: ForItrStmt (for (E1 : E2) S)",
           "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
    //[[item]] = int, [[items]] = array, [[answer]] = int, [[test]] = () -> int
    test() {
      var item, items, answer;
      items = [0, 1, 2, 3, 4, 5, 6];
      for (item : items) {
        answer = answer + item;
      }
      return answer;
    }
    )";

  auto unifierSymbols = collectAndSolve(program);
  auto unifier = unifierSymbols.first;
  auto symbols = unifierSymbols.second;

  std::vector<std::shared_ptr<TipType>> empty;

  auto fDecl = symbols->getFunction("test");
  auto fType = std::make_shared<TipVar>(fDecl);
  REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::intType()));

  auto itemType = std::make_shared<TipVar>(symbols->getLocal("item", fDecl));
  REQUIRE(*unifier.inferred(itemType) == *TypeHelper::intType());

  auto itemsType = std::make_shared<TipVar>(symbols->getLocal("items", fDecl));
  REQUIRE(*unifier.inferred(itemsType) == *TypeHelper::arrayType(TypeHelper::intType()));

  auto answerType = std::make_shared<TipVar>(symbols->getLocal("answer", fDecl));
  REQUIRE(*unifier.inferred(answerType) == *TypeHelper::intType());
}

TEST_CASE("TypeConstraintVisitor: ForRangeStmt with \"by\" (for (E1 : E2 .. E3 by E4) S)",
           "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
    //[[item]] = int, [[i]] = array, [[j]] = int, [[step]] = int, [[answer]] = int, [[test]] = () -> int
    test() {
      var item, i, j, step, answer;
      i = 0;
      j = 10;
      step = 2;
      for (item : i .. j by step) {
        answer = answer + item;
      }
      return answer;
    }
    )";

  auto unifierSymbols = collectAndSolve(program);
  auto unifier = unifierSymbols.first;
  auto symbols = unifierSymbols.second;

  std::vector<std::shared_ptr<TipType>> empty;

  auto fDecl = symbols->getFunction("test");
  auto fType = std::make_shared<TipVar>(fDecl);
  REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::intType()));

  auto itemType = std::make_shared<TipVar>(symbols->getLocal("item", fDecl));
  REQUIRE(*unifier.inferred(itemType) == *TypeHelper::intType());

  auto iType = std::make_shared<TipVar>(symbols->getLocal("i", fDecl));
  REQUIRE(*unifier.inferred(iType) == *TypeHelper::intType());

  auto jType = std::make_shared<TipVar>(symbols->getLocal("j", fDecl));
  REQUIRE(*unifier.inferred(jType) == *TypeHelper::intType());

  auto stepType = std::make_shared<TipVar>(symbols->getLocal("step", fDecl));
  REQUIRE(*unifier.inferred(stepType) == *TypeHelper::intType());

  auto answerType = std::make_shared<TipVar>(symbols->getLocal("answer", fDecl));
  REQUIRE(*unifier.inferred(answerType) == *TypeHelper::intType());
}

TEST_CASE("TypeConstraintVisitor: ForRangeStmt without \"by\" (for (E1 : E2 .. E3) S)",
           "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
    //[[item]] = int, [[i]] = array, [[j]] = int, [[answer]] = int, [[test]] = () -> int
    test() {
      var item, i, j, answer;
      i = 0;
      j = 10;
      for (item : i .. j) {
        answer = answer + item;
      }
      return answer;
    }
    )";

  auto unifierSymbols = collectAndSolve(program);
  auto unifier = unifierSymbols.first;
  auto symbols = unifierSymbols.second;

  std::vector<std::shared_ptr<TipType>> empty;

  auto fDecl = symbols->getFunction("test");
  auto fType = std::make_shared<TipVar>(fDecl);
  REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::intType()));

  auto itemType = std::make_shared<TipVar>(symbols->getLocal("item", fDecl));
  REQUIRE(*unifier.inferred(itemType) == *TypeHelper::intType());

  auto iType = std::make_shared<TipVar>(symbols->getLocal("i", fDecl));
  REQUIRE(*unifier.inferred(iType) == *TypeHelper::intType());

  auto jType = std::make_shared<TipVar>(symbols->getLocal("j", fDecl));
  REQUIRE(*unifier.inferred(jType) == *TypeHelper::intType());

  auto answerType = std::make_shared<TipVar>(symbols->getLocal("answer", fDecl));
  REQUIRE(*unifier.inferred(answerType) == *TypeHelper::intType());
}

TEST_CASE("TypeConstraintVisitor: \"true\" binary variable statement", "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
    //[[x]] = boolean, [[main]] = () -> int
    main() {
      var x;
      x = true;
      return 2;
    }
  )";

  auto unifierSymbols = collectAndSolve(program);
  auto unifier = unifierSymbols.first;
  auto symbols = unifierSymbols.second;

  std::vector<std::shared_ptr<TipType>> empty;

  auto fDecl = symbols->getFunction("main");
  auto fType = std::make_shared<TipVar>(fDecl);
  REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::intType()));

  auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
  REQUIRE(*unifier.inferred(xType) == *TypeHelper::booleanType());
}

TEST_CASE("TypeConstraintVisitor: nested conditionals with boolean expressions",
           "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
            // [[x]] = bool, [[y]] = bool, [[test]] = () -> bool
            test() {
              var x, y;
              x = true;
              y = false;
              if (x and (y or not x)) {
                x = y or true;
              } else {
                y = x and false;
              }
              return x;
            }
         )";

  auto unifierSymbols = collectAndSolve(program);
  auto unifier = unifierSymbols.first;
  auto symbols = unifierSymbols.second;

  std::vector<std::shared_ptr<TipType>> empty;

  auto fDecl = symbols->getFunction("test");
  auto fType = std::make_shared<TipVar>(fDecl);
  REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::booleanType()));

  auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
  REQUIRE(*unifier.inferred(xType) == *TypeHelper::booleanType());

  auto yType = std::make_shared<TipVar>(symbols->getLocal("y", fDecl));
  REQUIRE(*unifier.inferred(yType) == *TypeHelper::booleanType());
}

TEST_CASE("TypeConstraintVisitor: integer arithmetic in mixed expressions",
           "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
            // [[x]] = int, [[y]] = int, [[z]] = int, [[test]] = () -> int
            test() {
              var x, y, z;
              x = 5;
              y = 10;
              z = x * (y + 3);
              x = x - y;
              return z;
            }
         )";

  auto unifierSymbols = collectAndSolve(program);
  auto unifier = unifierSymbols.first;
  auto symbols = unifierSymbols.second;

  std::vector<std::shared_ptr<TipType>> empty;

  auto fDecl = symbols->getFunction("test");
  auto fType = std::make_shared<TipVar>(fDecl);
  REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::intType()));

  auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
  REQUIRE(*unifier.inferred(xType) == *TypeHelper::intType());

  auto yType = std::make_shared<TipVar>(symbols->getLocal("y", fDecl));
  REQUIRE(*unifier.inferred(yType) == *TypeHelper::intType());

  auto zType = std::make_shared<TipVar>(symbols->getLocal("z", fDecl));
  REQUIRE(*unifier.inferred(zType) == *TypeHelper::intType());
}

TEST_CASE("TypeConstraintVisitor: mixed-type boolean and integer expressions",
           "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
            // [[x]] = int, [[y]] = bool, [[result]] = int, [[test]] = () -> int
            test() {
              var x, y, result;
              x = 5;
              y = x > 2;
              result = y ? x + 10 : x - 10;
              return result;
            }
         )";

  auto unifierSymbols = collectAndSolve(program);
  auto unifier = unifierSymbols.first;
  auto symbols = unifierSymbols.second;

  std::vector<std::shared_ptr<TipType>> empty;

  auto fDecl = symbols->getFunction("test");
  auto fType = std::make_shared<TipVar>(fDecl);
  REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::intType()));

  auto xType = std::make_shared<TipVar>(symbols->getLocal("x", fDecl));
  REQUIRE(*unifier.inferred(xType) == *TypeHelper::intType());

  auto yType = std::make_shared<TipVar>(symbols->getLocal("y", fDecl));
  REQUIRE(*unifier.inferred(yType) == *TypeHelper::booleanType());

  auto resultType = std::make_shared<TipVar>(symbols->getLocal("result", fDecl));
  REQUIRE(*unifier.inferred(resultType) == *TypeHelper::intType());
}

TEST_CASE("TypeConstraintVisitor: function with array element assignment",
           "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
            // [[arr]] = int array, [[index]] = int, [[test]] = () -> int
            test() {
              var arr, index;
              arr = [1, 2, 3];
              index = 1;
              arr[index] = 10;
              return arr[0];
            }
         )";

  auto unifierSymbols = collectAndSolve(program);
  auto unifier = unifierSymbols.first;
  auto symbols = unifierSymbols.second;

  std::vector<std::shared_ptr<TipType>> empty;

  auto fDecl = symbols->getFunction("test");
  auto fType = std::make_shared<TipVar>(fDecl);
  REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::intType()));

  auto arrType = std::make_shared<TipVar>(symbols->getLocal("arr", fDecl));
  REQUIRE(*unifier.inferred(arrType) == *TypeHelper::arrayType(TypeHelper::intType()));

  auto indexType = std::make_shared<TipVar>(symbols->getLocal("index", fDecl));
  REQUIRE(*unifier.inferred(indexType) == *TypeHelper::intType());
}

TEST_CASE("TypeConstraintVisitor: nested arrays",
           "[TypeConstraintVisitor]") {
  std::stringstream program;
  program << R"(
            // [[arr]] = int array array, [[test]] = () -> int
            test() {
              var arr;
              arr = [[1, 2], [3, 4]];
              return (arr[0])[1];
            }
         )";

  auto unifierSymbols = collectAndSolve(program);
  auto unifier = unifierSymbols.first;
  auto symbols = unifierSymbols.second;

  std::vector<std::shared_ptr<TipType>> empty;

  auto fDecl = symbols->getFunction("test");
  auto fType = std::make_shared<TipVar>(fDecl);
  REQUIRE(*unifier.inferred(fType) == *TypeHelper::funType(empty, TypeHelper::intType()));

  auto arrType = std::make_shared<TipVar>(symbols->getLocal("arr", fDecl));
  REQUIRE(*unifier.inferred(arrType) == *TypeHelper::arrayType(TypeHelper::arrayType(TypeHelper::intType())));
}

//END SIP Extension
