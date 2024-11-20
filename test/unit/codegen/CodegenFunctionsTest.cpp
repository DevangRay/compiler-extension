#include "AST.h"
#include "ASTNodeHelpers.h"
#include "InternalError.h"
#include "ParserHelper.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("CodegenFunction: ASTDeclNode throws InternalError on codegen",
          "[CodegenFunctions]") {
  ASTDeclNode node("foo");
  REQUIRE_THROWS_AS(node.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTAssignsStmt throws InternalError on LHS codegen "
          "nullptr",
          "[CodegenFunctions]") {
  ASTAssignStmt assignStmt(std::make_shared<nullcodegen::MockASTExpr>(),
                           std::make_shared<ASTInputExpr>());
  REQUIRE_THROWS_AS(assignStmt.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTAssignsStmt throws InternalError on RHS codegen "
          "nullptr",
          "[CodegenFunctions]") {
  ASTAssignStmt assignStmt(std::make_shared<ASTInputExpr>(),
                           std::make_shared<nullcodegen::MockASTExpr>());
  REQUIRE_THROWS_AS(assignStmt.codegen(), InternalError);
}

TEST_CASE(
    "CodegenFunction: ASTIfStmt throws InternalError on COND codegen nullptr",
    "[CodegenFunctions]") {
  ASTIfStmt ifStmt(
      std::make_shared<nullcodegen::MockASTExpr>(),
      std::make_shared<ASTReturnStmt>(std::make_shared<ASTNumberExpr>(42)),
      std::make_shared<ASTReturnStmt>(std::make_shared<ASTNumberExpr>(42)));
  REQUIRE_THROWS_AS(ifStmt.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTBinaryExpr throws InternalError on LHS codegen "
          "nullptr",
          "[CodegenFunctions]") {
  ASTBinaryExpr binaryExpr("+", std::make_shared<nullcodegen::MockASTExpr>(),
                           std::make_shared<ASTInputExpr>());
  REQUIRE_THROWS_AS(binaryExpr.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTBinaryExpr throws InternalError on RHS codegen "
          "nullptr",
          "[CodegenFunctions]") {
  ASTBinaryExpr binaryExpr("+", std::make_shared<ASTInputExpr>(),
                           std::make_shared<nullcodegen::MockASTExpr>());
  REQUIRE_THROWS_AS(binaryExpr.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTBinaryExpr throws InternalError on bad OP",
          "[CodegenFunctions]") {
  ASTBinaryExpr binaryExpr("ADDITION", std::make_shared<ASTInputExpr>(),
                           std::make_shared<ASTInputExpr>());
  REQUIRE_THROWS_AS(binaryExpr.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTOutputStmt throws InternalError on ARG codegen "
          "nullptr",
          "[CodegenFunctions]") {
  ASTOutputStmt outputStmt(std::make_shared<nullcodegen::MockASTExpr>());
  REQUIRE_THROWS_AS(outputStmt.codegen(), InternalError);
}

TEST_CASE(
    "CodegenFunction: ASTErrorStmt throws InternalError on ARG codegen nullptr",
    "[CodegenFunctions]") {
  ASTErrorStmt errorStmt(std::make_shared<nullcodegen::MockASTExpr>());
  REQUIRE_THROWS_AS(errorStmt.codegen(), InternalError);
}

TEST_CASE(
    "CodegenFunction: ASTVariableExpr throws InternalError on unknown NAME",
    "[CodegenFunctions]") {
  ASTVariableExpr variableExpr("foobar");
  REQUIRE_THROWS_AS(variableExpr.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTAllocExpr throws InternalError on INIT codegen "
          "nullptr",
          "[CodegenFunctions]") {
  ASTAllocExpr allocExpr(std::make_shared<nullcodegen::MockASTExpr>());
  REQUIRE_THROWS_AS(allocExpr.codegen(), InternalError);
}

TEST_CASE(
    "CodegenFunction: ASTRefExpr throws InternalError on VAR codegen nullptr",
    "[CodegenFunctions]") {
  ASTRefExpr refExpr(std::make_shared<nullcodegen::MockASTExpr>());
  REQUIRE_THROWS_AS(refExpr.codegen(), InternalError);
}

TEST_CASE(
    "CodegenFunction: ASTDeRefExpr throws InternalError on VAR codegen nullptr",
    "[CodegenFunctions]") {
  ASTDeRefExpr deRefExpr(std::make_shared<nullcodegen::MockASTExpr>());
  REQUIRE_THROWS_AS(deRefExpr.codegen(), InternalError);
}

TEST_CASE(
    "CodegenFunction: ASTAccessExpr throws InternalError on nonexistent field",
    "[CodegenFunctions]") {
  ASTAccessExpr accessExpr(std::make_shared<nullcodegen::MockASTExpr>(),
                           "foobar");
  REQUIRE_THROWS_AS(accessExpr.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTFunAppExpr throws InternalError on FUN codegen "
          "nullptr",
          "[CodegenFunctions]") {
  std::vector<std::shared_ptr<ASTExpr>> actuals;
  ASTFunAppExpr funAppExpr(std::make_shared<nullcodegen::MockASTExpr>(),
                           actuals);
  REQUIRE_THROWS_AS(funAppExpr.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTForRangeStmt throws InternalError on improper for range (E2, E3 or E4) (E1 : E2 .. E3 by E4) "
          "nullptr",
          "[CodegenFunctions]") {
  ASTForRangeStmt forRangeStmt(std::make_shared<nullcodegen::MockASTExpr>(),
                           std::make_shared<nullcodegen::MockASTExpr>(),
                           std::make_shared<nullcodegen::MockASTExpr>(),
                           std::make_shared<nullcodegen::MockASTExpr>(),
                           std::make_shared<nullcodegen::MockASTStmt>());
  REQUIRE_THROWS_AS(forRangeStmt.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTForRangeStmt throws InternalError on improper E1 "
          "nullptr",
          "[CodegenFunctions]") {
    ASTForRangeStmt forRangeStmt(std::make_shared<ASTNumberExpr>(42),
                             std::make_shared<ASTNumberExpr>(42),
                             std::make_shared<ASTNumberExpr>(42),
                             std::make_shared<nullcodegen::MockASTExpr>(),
                             std::make_shared<nullcodegen::MockASTStmt>());
    REQUIRE_THROWS_AS(forRangeStmt.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTForRangeStmt throws InternalError on improper S "
          "nullptr",
          "[CodegenFunctions]") {
    ASTForRangeStmt forRangeStmt(std::make_shared<ASTNumberExpr>(42),
                             std::make_shared<ASTNumberExpr>(42),
                             std::make_shared<ASTNumberExpr>(42),
                             std::make_shared<ASTVariableExpr>("x"),
                             std::make_shared<nullcodegen::MockASTStmt>());
    REQUIRE_THROWS_AS(forRangeStmt.codegen(), InternalError);
}

TEST_CASE(
    "CodegenFunction: ASTNegExpr throws InternalError codegen nullptr",
    "[CodegenFunctions]") {
    ASTNegExpr negExpr(std::make_shared<nullcodegen::MockASTExpr>());
    REQUIRE_THROWS_AS(negExpr.codegen(), InternalError);
}

TEST_CASE(
    "CodegenFunction: ASTLogicalNotExpr throws InternalError codegen nullptr",
    "[CodegenFunctions]") {
    ASTLogicalNotExpr notExpr(std::make_shared<nullcodegen::MockASTExpr>());
    REQUIRE_THROWS_AS(notExpr.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTTernaryExpr throws InternalError on improper cond "
          "nullptr",
          "[CodegenFunctions]") {
    ASTTernaryExpr ternExpr(std::make_shared<nullcodegen::MockASTExpr>(),
                             std::make_shared<ASTNumberExpr>(42),
                             std::make_shared<ASTNumberExpr>(42));
    REQUIRE_THROWS_AS(ternExpr.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTTernaryExpr throws InternalError on improper if "
          "nullptr",
          "[CodegenFunctions]") {
    ASTTernaryExpr ternExpr(std::make_shared<ASTTrueExpr>(),
                             std::make_shared<nullcodegen::MockASTExpr>(),
                             std::make_shared<ASTNumberExpr>(42));
    REQUIRE_THROWS_AS(ternExpr.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTTernaryExpr throws InternalError on improper then "
          "nullptr",
          "[CodegenFunctions]") {
    ASTTernaryExpr ternExpr(std::make_shared<ASTTrueExpr>(),
                             std::make_shared<ASTNumberExpr>(42),
                             std::make_shared<nullcodegen::MockASTExpr>());
    REQUIRE_THROWS_AS(ternExpr.codegen(), InternalError);
}

TEST_CASE(
    "CodegenFunction: ASTDecrementExpr throws InternalError codegen nullptr",
    "[CodegenFunctions]") {
    ASTDecrementStmt decExpr(std::make_shared<nullcodegen::MockASTExpr>());
    REQUIRE_THROWS_AS(decExpr.codegen(), InternalError);
}

TEST_CASE(
    "CodegenFunction: ASTIncrementExpr throws InternalError codegen nullptr",
    "[CodegenFunctions]") {
    ASTIncrementStmt incExpr(std::make_shared<nullcodegen::MockASTExpr>());
    REQUIRE_THROWS_AS(incExpr.codegen(), InternalError);
}