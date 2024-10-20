#ifndef ASTARRAYLENEXPR_H
#define ASTARRAYLENEXPR_H
#pragma once
#include "ASTExpr.h"

class ASTArrayLenExpr : public ASTExpr {
    std::shared_ptr<ASTExpr> ARRAY;


public:
    std::vector<std::shared_ptr<ASTNode>> getChildren() override;
    ASTArrayLenExpr(std::shared_ptr<ASTExpr> ARRAY)
        : ARRAY(ARRAY) {}
    ASTExpr *getArray() const { return ARRAY.get(); }
    void accept(ASTVisitor *visitor) override;
    llvm::Value *codegen() override;

protected:
    std::ostream &print(std::ostream &out) const override;
};


#endif //ASTARRAYLENEXPR_H
