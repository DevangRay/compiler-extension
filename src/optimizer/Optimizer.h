#pragma once

#include "llvm/IR/Module.h"
#include "llvm/Support/CommandLine.h"

enum Optimization {
    indvars,lsr, lu, sroa, lscfg, jt
  /*
  options right now thinking about
        loop-reduce. Didnt work I tried, llvm bitcode didnt change at all with it in place and without
            idk what it does rn, something about array references
        indvars. Didnt work I tried, dont know if I tried well enough
            which does
            for (i = 7; i*i < 1000; ++i)
            into
            for (i = 0; i != 25; ++i)
            This transformation should be followed by strength reduction after all of the desired loop transformations
            have been performed. Additionally, on targets where it is profitable, the loop could be transformed to count down
            to zero (the “do loop” optimization).
        loop-unroll or loop unroll and jams, loop-unroll said it works best with indvars done first
            loop unrolls idk what else it does
        sccp Sparse Conditional Constant Propagation
            recommended running dead code optimization after this one, might be hard to test wtihout a big function
        jump-threading, optimizes that:
            if () { ...
                X = 4;
            }
            if (X < 3) {
    */
};
/*! \class Optimizer
 *  \brief routines to optimize generated code.
 */
class Optimizer {
public:
  /*! \brief optimize LLVM module.
   *
   * Apply a series of basic optimization passes to the given LLVM module.
   * \param theModule an LLVM module to be optimized
   */
    static void optimize(llvm::Module *theModule,
                 llvm::cl::list<Optimization> &enabledOpts);
};
