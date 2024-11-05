#pragma once
#include "TipCons.h"
#include "TipType.h"

/*!
 * \class TipArray
 *
 * \brief A proper type representing a Arrayerence
 */
class TipArray : public TipCons {
public:
    TipArray() = delete;
    TipArray(std::shared_ptr<TipType> of);

    std::shared_ptr<TipType> getArrayType() const;

    bool operator==(const TipType &other) const override;
    bool operator!=(const TipType &other) const override;

    void accept(TipTypeVisitor *visitor) override;

protected:
    std::ostream &print(std::ostream &out) const override;
};
