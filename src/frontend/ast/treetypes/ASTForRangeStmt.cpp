//
// Created by gbc2rx on 10/14/24.
// Based on ASTIfStmt.cpp

#include "ASTForRangeStmt.h"
#include "ASTVisitor.h"

void ASTForRangeStmt::accept(ASTVisitor *visitor) {
  if (visitor->visit(this)) {
    getInitializer()->accept(visitor);
    getRangeStart()->accept(visitor);
    getRangeEnd()->accept(visitor);
    if (getStep() != nullptr) {
      getStep()->accept(visitor);
    }
    getBody()->accept(visitor);
  }
  visitor->endVisit(this);
}

std::ostream &ASTForRangeStmt::print(std::ostream &out) const {
    out << "for ( " << *getInitializer() << " : " << *getRangeStart() << " .. " << *getRangeEnd();
    if (getStep() != nullptr) {
        out << " by " << *getStep();
    }
    out << " ) " << *getBody();

    return out;
}

std::vector<std::shared_ptr<ASTNode>> ASTForRangeStmt::getChildren() {
  std::vector<std::shared_ptr<ASTNode>> children;

  children.push_back(INITIALIZER);
  children.push_back(RANGESTART);
  children.push_back(RANGEEND);
  if (getStep() != nullptr) {
    children.push_back(STEP);
  }
  children.push_back(BODY);

  return children;
}

llvm::Value* ASTForRangeStmt::codegen() {
  return nullptr;
}