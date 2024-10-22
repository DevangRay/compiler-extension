//
// Created by gbc2rx on 10/14/24.
// Based on ASTIfStmt.cpp
//
#include "ASTTernaryExpr.h"
#include "ASTVisitor.h"

void ASTTernaryExpr::accept(ASTVisitor *visitor) {
    if (visitor->visit(this)) {
        getCondition()->accept(visitor);
        getThen()->accept(visitor);
        getElse()->accept(visitor);
    }
    visitor->endVisit(this);
}

//expr '?' expr ':' expr
// E1 ? E2 : E3 --> E1 = COND; E2 = THEN; E3 = ELSE

std::ostream &ASTTernaryExpr::print(std::ostream &out) const {
    out << *getCondition() << " ? " << *getThen() << " : " << *getElse();
    return out;
}

std::vector<std::shared_ptr<ASTNode>> ASTTernaryExpr::getChildren() {
    std::vector<std::shared_ptr<ASTNode>> children;

    children.push_back(COND);
    children.push_back(THEN);
    children.push_back(ELSE);

    return children;
}

llvm::Value* ASTTernaryExpr::codegen() {// LCOV_EXCL_LINE
    return nullptr;// LCOV_EXCL_LINE
}// LCOV_EXCL_LINE