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
    out << "! " << *getArg();
    return out;
}

std::vector<std::shared_ptr<ASTNode>> ASTLogicalNotExpr::getChildren() {
    std::vector<std::shared_ptr<ASTNode>> children;
    children.push_back(ARG);
    return children;
}

llvm::Value* ASTLogicalNotExpr::codegen() {
    return nullptr;
}