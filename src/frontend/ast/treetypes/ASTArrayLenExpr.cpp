#include "ASTArrayLenExpr.h"
#include "ASTVisitor.h"

void ASTArrayLenExpr::accept(ASTVisitor *visitor) {
    if (visitor->visit(this)) {
        getArray()->accept(visitor);
    }
    visitor->endVisit(this);
}

std::ostream &ASTArrayLenExpr::print(std::ostream &out) const {
    out << "#" << *getArray();
    return out;
}

std::vector<std::shared_ptr<ASTNode>> ASTArrayLenExpr::getChildren() {
    std::vector<std::shared_ptr<ASTNode>> children;
    children.push_back(ARRAY);
    return children;
}// LCOV_EXCL_LINE

llvm::Value* ASTArrayLenExpr::codegen(){// LCOV_EXCL_LINE
    return nullptr;// LCOV_EXCL_LINE
}// LCOV_EXCL_LINE