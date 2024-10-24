#include "ASTLogicalNotExpr.h"
#include "ASTVisitor.h"
#include "ASTNode.h"

void ASTLogicalNotExpr::accept(ASTVisitor *visitor) {
    if (visitor->visit(this)) {
        getArg()->accept(visitor);
    }
    visitor->endVisit(this);
}

std::ostream &ASTLogicalNotExpr::print(std::ostream &out) const {
    out << "not " << *getArg();
    return out;
}

std::vector<std::shared_ptr<ASTNode>> ASTLogicalNotExpr::getChildren() {
    std::vector<std::shared_ptr<ASTNode>> children;
    children.push_back(ARG);
    return children;
}// LCOV_EXCL_LINE

llvm::Value* ASTLogicalNotExpr::codegen() {// LCOV_EXCL_LINE
    return nullptr;// LCOV_EXCL_LINE
}// LCOV_EXCL_LINE