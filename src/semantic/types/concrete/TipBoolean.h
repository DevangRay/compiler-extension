#pragma once

#include "TipCons.h"
#include "TipType.h"
#include <string>

/*!
 * \class TipBoolean
 *
 * \brief A proper type representing an boolean
 */
class TipBoolean : public TipCons {
public:
  TipBoolean();

  bool operator==(const TipType &other) const override;
  bool operator!=(const TipType &other) const override;

  void accept(TipTypeVisitor *visitor) override;

protected:
  std::ostream &print(std::ostream &out) const override;
};
