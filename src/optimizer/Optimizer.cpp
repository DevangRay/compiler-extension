#include "Optimizer.h"
#include <iostream>

#include "llvm/Passes/PassBuilder.h"

#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"
#include "llvm/Transforms/Utils/Mem2Reg.h"
#include "loguru.hpp"

// d5
#include "llvm/Transforms/Scalar/LoopStrengthReduce.h"
#include "llvm/Transforms/Scalar/LoopUnrollPass.h"
#include "llvm/Transforms/Scalar/SROA.h"
#include "llvm/Transforms/Scalar/LoopSimplifyCFG.h"
#include "llvm/Transforms/Scalar/LoopInstSimplify.h"
#include "llvm/Transforms/Scalar/JumpThreading.h"

#include "llvm/Transforms/IPO/GlobalDCE.h"
#include "llvm/Transforms/IPO/ConstantMerge.h"
#include "llvm/Transforms/IPO/MergeFunctions.h"

#include "llvm/Transforms/Instrumentation/CGProfile.h"
#include "llvm/Analysis/CGSCCPassManager.h"
#include "llvm/Transforms/IPO/SCCP.h"
#include "llvm/Transforms/IPO/FunctionAttrs.h"
#include "llvm/Transforms/IPO/ArgumentPromotion.h"
#include "llvm/Transforms/IPO/CalledValuePropagation.h"

namespace { // Anonymous namespace for local function

    bool contains(Optimization o, llvm::cl::list<Optimization> &l) {
        for (unsigned i = 0; i != l.size(); ++i) {
            if (o == l[i]) return true;
        }
        return false;
    }

}


//  Minimal optimization pass using LLVM pass managers
void Optimizer::optimize(llvm::Module *theModule,
        llvm::cl::list<Optimization> &enabledOpts) {
  LOG_S(1) << "Optimizing program " << theModule->getName().str();

  // New pass builder
  llvm::PassBuilder passBuilder;

  // Setting-up Analysis Managers for different granularities of optimizations
  llvm::FunctionAnalysisManager functionAnalysisManager;
  llvm::ModuleAnalysisManager moduleAnalysisManager;
  llvm::LoopAnalysisManager loopAnalysisManager;
  llvm::CGSCCAnalysisManager cgsccAnalysisManager;

  // Registering the analysis managers with the pass builder
  passBuilder.registerModuleAnalyses(moduleAnalysisManager);
  passBuilder.registerCGSCCAnalyses(cgsccAnalysisManager);
  passBuilder.registerFunctionAnalyses(functionAnalysisManager);
  passBuilder.registerLoopAnalyses(loopAnalysisManager);

  // Cross Register Proxies so that analysis results can be reused as needed.
  passBuilder.crossRegisterProxies(loopAnalysisManager, functionAnalysisManager,
                                   cgsccAnalysisManager, moduleAnalysisManager);

  // Initiating Function and Module level PassManagers
  llvm::ModulePassManager modulePassManager;
  llvm::FunctionPassManager functionPassManager;
  llvm::LoopPassManager loopPassManagerWithMSSA;
  llvm::LoopPassManager loopPassManager;
  // Adding passes to the pipeline
  // Constructs SSA and is a pre-requisite for many other passes
  functionPassManager.addPass(llvm::PromotePass());

  // Instruction combine pass scans for a variety of patterns and replaces bitcodes matched with improvements.
  functionPassManager.addPass(llvm::InstCombinePass());

  // Reassociate expressions.
  functionPassManager.addPass(llvm::ReassociatePass());

  // Eliminate Common SubExpressions using the Global Value Numbering (GVN) algorithm.
  functionPassManager.addPass(llvm::GVNPass());

  // Simplify the control flow graph (deleting unreachable blocks, etc).
  functionPassManager.addPass(llvm::SimplifyCFGPass());

    if (contains(sccp, enabledOpts)) {
        llvm::CGSCCPassManager CGPM;
        CGPM.addPass(llvm::PostOrderFunctionAttrsPass());
        CGPM.addPass(llvm::ArgumentPromotionPass());
        CGPM.addPass(
            llvm::createCGSCCToFunctionPassAdaptor(llvm::SROAPass(llvm::SROAOptions::ModifyCFG)));
        modulePassManager.addPass(llvm::createModuleToPostOrderCGSCCPassAdaptor(std::move(CGPM)));

        modulePassManager.addPass(llvm::CGProfilePass());

        // Propagate constants at call sites into the functions they call.  This
        // opens opportunities for globalopt (and inlining) by substituting function
        // pointers passed as arguments to direct uses of functions.
        modulePassManager.addPass(llvm::IPSCCPPass(false));

        modulePassManager.addPass(llvm::CalledValuePropagationPass());
    }

  if (contains(lsr, enabledOpts)) {
    loopPassManagerWithMSSA.addPass(llvm::LoopStrengthReducePass());
  }

  if (contains(lu, enabledOpts)) {
    loopPassManager.addPass(llvm::LoopFullUnrollPass());
  }

  if (contains(lscfg, enabledOpts)) {
      loopPassManager.addPass(llvm::LoopSimplifyCFGPass());
  }
  // Add loop pass managers with and w/out MemorySSA
  functionPassManager.addPass(
      createFunctionToLoopPassAdaptor(std::move(loopPassManagerWithMSSA),true));

  functionPassManager.addPass(
      createFunctionToLoopPassAdaptor(std::move(loopPassManager)));

  if (contains(sroa, enabledOpts)) {
      functionPassManager.addPass(llvm::SROAPass(llvm::SROAOptions::ModifyCFG));
  }

  if (contains(jt, enabledOpts)) {
    functionPassManager.addPass(llvm::JumpThreadingPass());
  }
  // Passing the function pass manager to the modulePassManager using a function
  // adaptor, then passing theModule to the ModulePassManager along with
  // ModuleAnalysisManager.
  modulePassManager.addPass(
      createModuleToFunctionPassAdaptor(std::move(functionPassManager), true));

    //interprocedural optimization -- optimizing by combining repetetive code within the module
    if (contains(fm, enabledOpts)) {
        modulePassManager.addPass(llvm::GlobalDCEPass());
        modulePassManager.addPass(llvm::ConstantMergePass());
        modulePassManager.addPass(llvm::MergeFunctionsPass());
    }

  modulePassManager.run(*theModule, moduleAnalysisManager);
}
