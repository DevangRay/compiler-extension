//
// Created by gbc2rx on 10/10/24.
//

#include "ASTFalseExpr.h"
#include "ASTVisitor.h"

void ASTFalseExpr::accept(ASTVisitor *visitor) {
    visitor->visit(this);
    visitor->endVisit(this);
}

std::ostream &ASTFalseExpr::print(std::ostream &out) const {
    out << "false";
    return out;
}

llvm::Value* ASTFalseExpr::codegen() {// LCOV_EXCL_LINE
    return nullptr;// LCOV_EXCL_LINE
}// LCOV_EXCL_LINE