#ifndef ASTFORITRSTMT_H
#define ASTFORITRSTMT_H
#pragma once

#include "ASTExpr.h"
#include "ASTStmt.h"

/*! \brief Class for a iterative for loop.
 */
class ASTForItrStmt : public ASTStmt {
    std::shared_ptr<ASTExpr> START;
    std::shared_ptr<ASTExpr> END;
    std::shared_ptr<ASTStmt> BODY;

public:
    std::vector<std::shared_ptr<ASTNode>> getChildren() override;
    ASTForItrStmt(std::shared_ptr<ASTExpr> START, std::shared_ptr<ASTExpr> END, std::shared_ptr<ASTStmt> BODY)
        : START(START), END(END), BODY(BODY) {}
    ASTExpr *getStart() const { return START.get(); }
    ASTExpr *getEnd() const { return END.get(); }
    ASTStmt *getBody() const { return BODY.get(); }
    void accept(ASTVisitor *visitor) override;
    llvm::Value *codegen() override;

protected:
    std::ostream &print(std::ostream &out) const override;
};


#endif //ASTFORITRSTMT_H
