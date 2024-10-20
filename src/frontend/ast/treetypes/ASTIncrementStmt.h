#pragma once

#include "ASTStmt.h"
#include "ASTExpr.h"

class ASTIncrementStmt : public ASTStmt {
      std::shared_ptr<ASTExpr> ARG;

public:
    std::vector<std::shared_ptr<ASTNode>> getChildren() override;
    ASTIncrementStmt(std::shared_ptr<ASTExpr> ARG) : ARG(ARG) {}
    ASTExpr *getArg() const { return ARG.get(); }
    void accept(ASTVisitor *visitor) override;
    llvm::Value *codegen() override;

protected:
    std::ostream &print(std::ostream &out) const override;
};
