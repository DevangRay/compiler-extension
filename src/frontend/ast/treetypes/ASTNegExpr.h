//
// Created by gbc2rx on 10/14/24.
//

#pragma once

#include "ASTExpr.h"

/*! \brief Class for negated expression
 */
class ASTNegExpr : public ASTExpr {
    std::shared_ptr<ASTExpr> INIT;

public:
    std::vector<std::shared_ptr<ASTNode>> getChildren() override;
    ASTNegExpr(std::shared_ptr<ASTExpr> INIT) : INIT(INIT) {}
    ASTExpr *getInitializer() const { return INIT.get(); }
    void accept(ASTVisitor *visitor) override;
    llvm::Value *codegen() override;

protected:
    std::ostream &print(std::ostream &out) const override;
};
