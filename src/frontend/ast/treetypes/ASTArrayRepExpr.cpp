#include "ASTArrayRepExpr.h"
#include "ASTVisitor.h"

void ASTArrayRepExpr::accept(ASTVisitor *visitor) {
    if (visitor->visit(this)) {
        getStart()->accept(visitor);
        getEnd()->accept(visitor);
    }
    visitor->endVisit(this);
}

std::ostream &ASTArrayRepExpr::print(std::ostream &out) const {
    out << "[" << *getStart() << " of " << *getEnd() << "]";
    return out;
}

std::vector<std::shared_ptr<ASTNode>> ASTArrayRepExpr::getChildren() {
    std::vector<std::shared_ptr<ASTNode>> children;
    children.push_back(START);
    children.push_back(END);
    return children;
}// LCOV_EXCL_LINE

llvm::Value* ASTArrayRepExpr::codegen(){// LCOV_EXCL_LINE
    return nullptr;// LCOV_EXCL_LINE
}// LCOV_EXCL_LINE// LCOV_EXCL_LINE
