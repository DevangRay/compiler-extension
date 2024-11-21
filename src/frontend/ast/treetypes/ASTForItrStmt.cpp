#include "ASTForItrStmt.h"
#include "ASTVisitor.h"

void ASTForItrStmt::accept(ASTVisitor *visitor) {
    if (visitor->visit(this)) {
        getStart()->accept(visitor);
        getEnd()->accept(visitor);
        getBody()->accept(visitor);
    }
    visitor->endVisit(this);
}

std::ostream &ASTForItrStmt::print(std::ostream &out) const {
    out << "for (" << *getStart() << " : " << *getEnd() << ") " << *getBody();
    return out;
}

std::vector<std::shared_ptr<ASTNode>> ASTForItrStmt::getChildren() {
    std::vector<std::shared_ptr<ASTNode>> children;
    children.push_back(START);
    children.push_back(END);
    children.push_back(BODY);
    return children;
}// LCOV_EXCL_LINE