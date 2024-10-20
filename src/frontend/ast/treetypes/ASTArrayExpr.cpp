//
// Created by gbc2rx on 10/14/24.
// Inspired by ASTFunAppExpr.cpp
//

#include "ASTArrayExpr.h"
#include "ASTVisitor.h"
#include "ASTinternal.h"

ASTArrayExpr::ASTArrayExpr(std::vector<std::shared_ptr<ASTExpr>> ACTUALS) {
  for (auto &actual : ACTUALS) {
    std::shared_ptr<ASTExpr> a = actual;
    this->ACTUALS.push_back(a);
  }
}

std::vector<ASTExpr *> ASTArrayExpr::getActuals() const {
  return rawRefs(ACTUALS);
}

void ASTArrayExpr::accept(ASTVisitor *visitor) {
  if (visitor->visit(this)) {
    for (auto a : getActuals()) {
      a->accept(visitor);
    }
  }
  visitor->endVisit(this);
}

std::ostream &ASTArrayExpr::print(std::ostream &out) const {
  out << "[ ";
  bool skip = true;
  for (auto &arg : getActuals()) {
    if (skip) {
      skip = false;
      out << *arg;
      continue;
    }
    out << ", " << *arg;
  }
  out << " ]";
  return out;
} // LCOV_EXCL_LINE

std::vector<std::shared_ptr<ASTNode>> ASTArrayExpr::getChildren() {
  std::vector<std::shared_ptr<ASTNode>> children;
  for (auto &actual : ACTUALS) {
    children.push_back(actual);
  }
  return children;
}

llvm::Value* ASTArrayExpr::codegen() {
  return nullptr;
}