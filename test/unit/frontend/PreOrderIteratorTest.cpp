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

TEST_CASE("PreOrderIterator: Test TernaryExpr", "[PreOrderIterator]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x, answer;
        x = false;
        answer = x ? 1 : 0;
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
    "x = false;",
    "x",
    "false",
    "answer = x ? 1 : 0;",
    "answer",
    "x ? 1 : 0",
    "x",
    "1",
    "0",
    "return answer;",
    "answer",
};

  SyntaxTree syntaxTree(ast);
  int i = -1;
  for (auto iter = syntaxTree.begin(""); iter != syntaxTree.end(""); ++iter) {
    if (i != -1) {
      std::stringstream actual_node;
      actual_node << *iter->getRoot();
//            std::cout << actual_node.str() << "\n";
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
/*
virtual void endVisit(ASTDecrementStmt *element) override;
virtual void endVisit(ASTIncrementStmt *element) override;
virtual void endVisit(ASTLogicalNotExpr *element) override;
virtual bool visit(ASTForItrStmt *element) override;
virtual void endVisit(ASTForItrStmt *element) override;
virtual void endVisit(ASTArrayRepExpr *element) override;
virtual void endVisit(ASTArrayLenExpr *element) override;
virtual void endVisit(ASTArrayRefExpr *element) override;
*/

 TEST_CASE("PreOrderIterator: Increment stmt test",
             "[PreOrderIterator]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x;
        x++;
        return 5;
      }
    )";

  std::shared_ptr<ASTProgram> ast = std::move(ASTHelper::build_ast(stream));

  std::vector<std::string> expected_node_order = {
    "d50033ecd29bd03507d4ba4f3ed57367360e2bd1f988523ef1035306b13705f4",
    "short() {...}",
    "short",
    "var x;",
    "x",
    "x++;",
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
TEST_CASE("PreOrderIterator: Decerment stmt test",
            "[PreOrderIterator]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x;
        x--;
        return 5;
      }
    )";

  std::shared_ptr<ASTProgram> ast = std::move(ASTHelper::build_ast(stream));

  std::vector<std::string> expected_node_order = {
    "5fe59f90586a4ae71f0fa9393167fe30a0afd456eb13acc09b6890cf338ac3dc",
    "short() {...}",
    "short",
    "var x;",
    "x",
    "x--;",
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

TEST_CASE("PreOrderIterator: logical not tests",
             "[PreOrderIterator]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x;
        return not x;
      }
    )";

  std::shared_ptr<ASTProgram> ast = std::move(ASTHelper::build_ast(stream));

  std::vector<std::string> expected_node_order = {
    "c3ec1a0225238d728dad55fa2c58708588c716751b0eee9e741478e16768448a",
    "short() {...}",
    "short",
    "var x;",
    "x",
    "return not x;",
    "not x",
    "x"
};

  SyntaxTree syntaxTree(ast);
  int i = 0;
  for (auto iter = syntaxTree.begin(""); iter != syntaxTree.end(""); ++iter) {
    std::stringstream actual_node;
    actual_node << *iter->getRoot();
    REQUIRE(expected_node_order.at(i++) == actual_node.str());
  }
}

TEST_CASE("PreOrderIterator: for itr tests",
             "[PreOrderIterator]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x;
        for ( x : 10){ x++;}
        return x;
      }
    )";

  std::shared_ptr<ASTProgram> ast = std::move(ASTHelper::build_ast(stream));

  std::vector<std::string> expected_node_order = {
    "db19ce36d91761d69532355d726d7278b0dfcc0787a9f952ddb63677af56b4f0",
    "short() {...}",
    "short",
    "var x;",
    "x",
    "for (x : 10) { x++; }",
    "x",
    "10",
    "{ x++; }",
    "x++;",
    "x",
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

TEST_CASE("PreOrderIterator: array rep expr",
            "[PreOrderIterator]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x;
        return [x of 5];
      }
    )";

  std::shared_ptr<ASTProgram> ast = std::move(ASTHelper::build_ast(stream));

  std::vector<std::string> expected_node_order = {
    "cf3b262fb630adfcbd746e452b7d4ad0a342b656591dc7fc121753f19e1e0ee5",
    "short() {...}",
    "short",
    "var x;",
    "x",
    "return [x of 5];",
    "[x of 5]",
    "x",
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

TEST_CASE("PreOrderIterator: array len expr",
            "[PreOrderIterator]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x;
        return #x;
      }
    )";

  std::shared_ptr<ASTProgram> ast = std::move(ASTHelper::build_ast(stream));

  std::vector<std::string> expected_node_order = {
     "b47b24c52cdd3902b02a598392220946a878a726e495caff8bac50ee458fc0a5",
    "short() {...}",
    "short",
    "var x;",
    "x",
    "return #x;",
    "#x",
    "x"
};

  SyntaxTree syntaxTree(ast);
  int i = 0;
  for (auto iter = syntaxTree.begin(""); iter != syntaxTree.end(""); ++iter) {
    std::stringstream actual_node;
    actual_node << *iter->getRoot();
    REQUIRE(expected_node_order.at(i++) == actual_node.str());
  }

}

TEST_CASE("PreOrderIterator: array ref expr",
            "[PreOrderIterator]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x;
        return x[10];
      }
    )";

  std::shared_ptr<ASTProgram> ast = std::move(ASTHelper::build_ast(stream));

  std::vector<std::string> expected_node_order = {
    "bc71520dec2470468180cbdd8cd0d65ff54e3f6fa82fabf23238f30f4927e09f",
    "short() {...}",
    "short",
    "var x;",
    "x",
    "return x[10];",
    "x[10]",
    "x",
    "10",
};

  SyntaxTree syntaxTree(ast);
  int i = 0;
  for (auto iter = syntaxTree.begin(""); iter != syntaxTree.end(""); ++iter) {
    std::stringstream actual_node;
    actual_node << *iter->getRoot();
    REQUIRE(expected_node_order.at(i++) == actual_node.str());
  }

}