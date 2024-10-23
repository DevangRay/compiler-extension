#include "PreOrderIterator.h"
#include "ASTHelper.h"
#include "PrettyPrinter.h"
#include "SyntaxTree.h"

#include <catch2/catch_test_macros.hpp>

#include <Iterator.h>
#include <iostream>

//SIP Extension
TEST_CASE("PreOrderIterator: Test ArrayExpr", "[PreOrderIterator]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x;
        x = [1, 0, 4];
        return x;
      }
    )";

  std::shared_ptr<ASTProgram> ast = std::move(ASTHelper::build_ast(stream));

  std::vector<std::string> expected_node_order = {
    "e8b58ccf60a6ff962c70cf0b2c849f86824d1460552e0b55290f1ffbf39b74ff",
    "short() {...}",
    "short",
    "var x;",
    "x",
    "x = [ 1, 0, 4 ];",
    "x",
    "[ 1, 0, 4 ]",
    "1",
    "0",
    "4",
    "return x;",
    "x",
};

  SyntaxTree syntaxTree(ast);
  int i = 0;
  for (auto iter = syntaxTree.begin(""); iter != syntaxTree.end(""); ++iter) {
    std::stringstream actual_node;
    actual_node << *iter->getRoot();
    REQUIRE(expected_node_order.at(i++) == actual_node.str());
  }
}

TEST_CASE("PreOrderIterator: Test ForRangeStmt with by child", "[PreOrderIterator]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x, answer;
        x = [1, 0, 4];
        for (item : 0 .. 10 by 2) answer = answer + item;
        return answer;
      }
    )";

  std::shared_ptr<ASTProgram> ast = std::move(ASTHelper::build_ast(stream));

  std::vector<std::string> expected_node_order = {
    "49c200d6e706d633c00da64e9db9ecc07431e823dfedb6330a6fbf66ae4dc9a0",
    "short() {...}",
    "short",
    "var x, answer;",
    "x",
    "answer",
    "x = [ 1, 0, 4 ];",
    "x",
    "[ 1, 0, 4 ]",
    "1",
    "0",
    "4",
    "for (item : 0 .. 10 by 2) answer = (answer+item);",
    "item",
    "0",
    "10",
    "2",
    "answer = (answer+item);",
    "answer",
    "(answer+item)",
    "answer",
    "item",
    "return answer;",
    "answer",
};

  SyntaxTree syntaxTree(ast);
  int i = 0;
  for (auto iter = syntaxTree.begin(""); iter != syntaxTree.end(""); ++iter) {
    std::stringstream actual_node;
    actual_node << *iter->getRoot();
//        std::cout << actual_node.str() << "\n";
    REQUIRE(expected_node_order.at(i++) == actual_node.str());
  }
}

TEST_CASE("PreOrderIterator: Test ForRangeStmt with no by child", "[PreOrderIterator]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x, answer;
        x = [1, 0, 4];
        for (item : 0 .. 5) answer = answer + item;
        return answer;
      }
    )";

  std::shared_ptr<ASTProgram> ast = std::move(ASTHelper::build_ast(stream));

  std::vector<std::string> expected_node_order = {
    "def53103c2ee17eee8f0d54c6dfa84070d9403c298075427c4f9584b108fc22b",
    "short() {...}",
    "short",
    "var x, answer;",
    "x",
    "answer",
    "x = [ 1, 0, 4 ];",
    "x",
    "[ 1, 0, 4 ]",
    "1",
    "0",
    "4",
    "for (item : 0 .. 5) answer = (answer+item);",
    "item",
    "0",
    "5",
    "answer = (answer+item);",
    "answer",
    "(answer+item)",
    "answer",
    "item",
    "return answer;",
    "answer",
};

  SyntaxTree syntaxTree(ast);
  int i = 0;
  for (auto iter = syntaxTree.begin(""); iter != syntaxTree.end(""); ++iter) {
    std::stringstream actual_node;
    actual_node << *iter->getRoot();
    //        std::cout << actual_node.str() << "\n";
    REQUIRE(expected_node_order.at(i++) == actual_node.str());
  }
}

TEST_CASE("PreOrderIterator: Test NegExpr", "[PreOrderIterator]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x, answer;
        x = input;
        x = -x;
        return answer;
      }
    )";

  std::shared_ptr<ASTProgram> ast = std::move(ASTHelper::build_ast(stream));

  std::vector<std::string> expected_node_order = {
    "short() {...}",
    "short",
    "var x, answer;",
    "x",
    "answer",
    "x = input;",
    "x",
    "input",
    "x = - x;",
    "x",
    "- x",
    "x",
    "return answer;",
    "answer",
};

  SyntaxTree syntaxTree(ast);
  int i = -1;
  for (auto iter = syntaxTree.begin(""); iter != syntaxTree.end(""); ++iter) {
    if (i != -1) {
      std::stringstream actual_node;
      actual_node << *iter->getRoot();
//      std::cout << actual_node.str() << "\n";
          REQUIRE(expected_node_order.at(i++) == actual_node.str());
    }
    else{
      i++;
    }
  }
}
//End Extension

TEST_CASE("PreOrderIterator: Test Traversal", "[PreOrderIterator]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x;
        return 5;
      }
    )";

  std::shared_ptr<ASTProgram> ast = std::move(ASTHelper::build_ast(stream));

  std::vector<std::string> expected_node_order = {
      "50c958e71584bef4c6a4334580c1669c4da35197a53365db8f53d54b1775f848",
      "short() {...}",
      "short",
      "var x;",
      "x",
      "return 5;",
      "5",
  };

  SyntaxTree syntaxTree(ast);
  int i = 0;
  for (auto iter = syntaxTree.begin(""); iter != syntaxTree.end(""); ++iter) {
    std::stringstream actual_node;
    actual_node << *iter->getRoot();
    REQUIRE(expected_node_order.at(i++) == actual_node.str());
  }
}

TEST_CASE("PreOrderIterator: Test dereference", "[PreOrderIterator]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x;
        return 5;
      }
    )";
  std::shared_ptr<ASTProgram> ast = std::move(ASTHelper::build_ast(stream));
  SyntaxTree syntaxTree(ast);
  auto iter = syntaxTree.begin("");
  REQUIRE(syntaxTree.getRoot() == (*iter).getRoot());
  REQUIRE(syntaxTree.getRoot() == iter->getRoot());
}

TEST_CASE("PreOrderIterator: Test Cloning", "[PreOrderIterator]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x;
        return 5;
      }
    )";

  std::shared_ptr<ASTProgram> ast = std::move(ASTHelper::build_ast(stream));
  SyntaxTree syntaxTree(ast);
  auto iter = new PreOrderIterator(syntaxTree, false);
  auto cloned = iter->clone();

  REQUIRE(iter->get_tree().getRoot() == cloned->get_tree().getRoot());
}

TEST_CASE("PreOrderIterator: Exhausted increment returns gracefully",
          "[PreOrderIterator]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x;
        return 5;
      }
    )";

  std::shared_ptr<ASTProgram> ast = std::move(ASTHelper::build_ast(stream));
  SyntaxTree syntaxTree(ast);
  auto iter = syntaxTree.begin("");
  for (; iter != syntaxTree.end(""); ++iter)
    ;

  REQUIRE_NOTHROW(iter++);
}

TEST_CASE("PreOrderIterator: Sentinel end", "[PreOrderIterator]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x;
        return 5;
      }
    )";

  std::shared_ptr<ASTProgram> ast = std::move(ASTHelper::build_ast(stream));
  SyntaxTree syntaxTree(ast);
  auto begin = PreOrderIterator(syntaxTree, false);
  auto end = PreOrderIterator(syntaxTree, true);

  REQUIRE(begin != end);
}
