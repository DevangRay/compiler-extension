//
// Created by gbc2rx on 10/14/24.
// Based on ASTFunAppExpr.h
//

#pragma once

#include "ASTExpr.h"

/*! \brief Class for function call expressions
 */
class ASTArrayExpr : public ASTExpr {
  std::vector<std::shared_ptr<ASTExpr>> ACTUALS;

public:
  std::vector<std::shared_ptr<ASTNode>> getChildren() override;
  ASTArrayExpr(std::vector<std::shared_ptr<ASTExpr>> ACTUALS);
  std::vector<ASTExpr *> getActuals() const;
  void accept(ASTVisitor *visitor) override;
  llvm::Value *codegen() override;

protected:
  std::ostream &print(std::ostream &out) const override;
};
