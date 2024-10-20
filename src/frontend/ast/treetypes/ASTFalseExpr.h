#pragma once

#include "ASTExpr.h"

/*! \brief Class for a null expression
 */
class ASTFalseExpr : public ASTExpr {
public:
    ASTFalseExpr() {}
    void accept(ASTVisitor *visitor) override;
    llvm::Value *codegen() override;

protected:
    std::ostream &print(std::ostream &out) const override;
};
