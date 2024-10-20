//
// Created by gbc2rx on 10/14/24.
// Based on ASTIfStmt.h/ASTArrayExpr.h
//
#pragma once

#include "ASTExpr.h"
#include "ASTStmt.h"

/*! \brief Class for if-then-else.
 */
//for '(' expr ':' expr '..' expr ('by' expr)? ')' statement
class ASTForRangeStmt : public ASTStmt {
  std::shared_ptr<ASTExpr> INITIALIZER, RANGESTART, RANGEEND, STEP;
  std::shared_ptr<ASTStmt> BODY;

public:
  std::vector<std::shared_ptr<ASTNode>> getChildren() override;
  ASTForRangeStmt(std::shared_ptr<ASTExpr> INITIALIZER, std::shared_ptr<ASTExpr> RANGESTART,
            std::shared_ptr<ASTExpr> RANGEEND, std::shared_ptr<ASTExpr> STEP, std::shared_ptr<ASTStmt> BODY)
      : INITIALIZER(INITIALIZER), RANGESTART(RANGESTART), RANGEEND(RANGEEND), STEP(STEP), BODY(BODY) {}
  ASTExpr *getInitializer() const { return INITIALIZER.get(); }
  ASTExpr *getRangeStart() const { return RANGESTART.get(); }
  ASTExpr *getRangeEnd() const { return RANGEEND.get(); }
  ASTStmt *getBody() const { return BODY.get(); }

  /*! \fn getElse
   * \return Else statement if it exists and nullptr otherwise.
   */
  ASTExpr *getStep() const { return STEP.get(); }

  void accept(ASTVisitor *visitor) override;
  llvm::Value *codegen() override;

protected:
  std::ostream &print(std::ostream &out) const override;
};

