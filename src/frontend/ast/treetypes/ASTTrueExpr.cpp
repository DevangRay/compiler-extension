//
// Created by gbc2rx on 10/10/24.
//

#include "ASTTrueExpr.h"
#include "ASTVisitor.h"

void ASTTrueExpr::accept(ASTVisitor *visitor) {
    visitor->visit(this);
    visitor->endVisit(this);
}

std::ostream &ASTTrueExpr::print(std::ostream &out) const {
    out << "true";
    return out;
}// LCOV_EXCL_LINE

llvm::Value* ASTTrueExpr::codegen() {// LCOV_EXCL_LINE
    return nullptr;// LCOV_EXCL_LINE
}// LCOV_EXCL_LINE