#include "ASTIncrementStmt.h"
#include "ASTVisitor.h"
#include "ASTNode.h"

void ASTIncrementStmt::accept(ASTVisitor *visitor) {
    if (visitor->visit(this)) {
        getArg()->accept(visitor);
    }
    visitor->endVisit(this);
}

std::ostream &ASTIncrementStmt::print(std::ostream &out) const {
    out << *getArg() << "++;";
    return out;
}

std::vector<std::shared_ptr<ASTNode>> ASTIncrementStmt::getChildren() {
    std::vector<std::shared_ptr<ASTNode>> children;
    children.push_back(ARG);
    return children;
}// LCOV_EXCL_LINE
