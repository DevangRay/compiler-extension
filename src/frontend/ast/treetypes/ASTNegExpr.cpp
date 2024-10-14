//
// Created by gbc2rx on 10/14/24.
// based on ASTAllocExpr node
//
#include "ASTNegExpr.h"
#include "ASTVisitor.h"

void ASTNegExpr::accept(ASTVisitor *visitor) {
    if (visitor->visit(this)) {
        getInitializer()->accept(visitor);
    }
    visitor->endVisit(this);
}

std::ostream &ASTNegExpr::print(std::ostream &out) const {
    out << "- " << *getInitializer();
    return out;
}

std::vector<std::shared_ptr<ASTNode>> ASTNegExpr::getChildren() {
    std::vector<std::shared_ptr<ASTNode>> children;
    children.push_back(INIT);
    return children;
}

llvm::Value* ASTNegExpr::codegen() {
    return nullptr;
}
