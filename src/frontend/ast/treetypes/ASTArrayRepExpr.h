#ifndef ASTARRAYREPEXPR_H
#define ASTARRAYREPEXPR_H
#pragma once

#include "ASTExpr.h"

/*! \brief Class for an array repetition expression
 */
class ASTArrayRepExpr : public ASTExpr {
    std::shared_ptr<ASTExpr> START;
    std::shared_ptr<ASTExpr> END;

public:
    std::vector<std::shared_ptr<ASTNode>> getChildren() override;
    ASTArrayRepExpr(std::shared_ptr<ASTExpr> START, std::shared_ptr<ASTExpr> END)
        : START(START), END(END) {}
    ASTExpr *getStart() const { return START.get(); }
    ASTExpr *getEnd() const { return END.get(); }
    void accept(ASTVisitor *visitor) override;
    llvm::Value *codegen() override;

protected:
    std::ostream &print(std::ostream &out) const override;
};


#endif //ASTARRAYREPEXPR_H
