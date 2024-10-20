#ifndef ASTARRAYREFEXPR_H
#define ASTARRAYREFEXPR_H
/*! \brief Class for an array reference expression
 */
#pragma once
#include "ASTExpr.h"

class ASTArrayRefExpr : public ASTExpr {
    std::shared_ptr<ASTExpr> ARRAY;
    std::shared_ptr<ASTExpr> INDEX;

public:
    std::vector<std::shared_ptr<ASTNode>> getChildren() override;
    ASTArrayRefExpr(std::shared_ptr<ASTExpr> ARRAY, std::shared_ptr<ASTExpr> INDEX)
        : ARRAY(ARRAY), INDEX(INDEX) {}
    ASTExpr *getArray() const { return ARRAY.get(); }
    ASTExpr *getIndex() const { return INDEX.get(); }
    void accept(ASTVisitor *visitor) override;
    llvm::Value *codegen() override;

protected:
    std::ostream &print(std::ostream &out) const override;
};


#endif //ASTARRAYREFEXPR_H
