You are to describe your process for selecting optimizations, describe each optimization that is included,
and describe the evidence that supports its unique benefit in optimizing SIP programs.

## Process
First thing I did was go here https://llvm.org/docs/Passes.html and just started reading for anything that seemed usable.
Since SIP added arrays to TIP we first decided to explore optimizations that helped with arrays. We first found loop strength reduction which stated that "strength reduction on array references inside loops that have as one or more of their components the loop induction variable" then tried SROA (Scalar Representation of Aggregates) which should theoritically just turn small arrays into a series of variables/registers but it refused to work again. I think our arrays are resistant to optimization at this point.
The professor recommended poking around the clang/llvm repo for their optimizations so that's what we did. Found loop simplify cfg that way and it mentioned that it was good at cleaning up loops after other optimizations ran so I tried it after loop unrolling.
Found jump threading after that by looking through clang repo and the llvm docs. 

## Optimizations Included
All benchmarks can be found at
`/test/optimizations`<br>
If you would like to see them all run `benchmarks.sh` should do the trick
#### Optimization 1, Loop optimization: loop unrolling
Benchmark `loopUnrolling.tip` with -lu

##### Benchmark results:
Building loopUnrolling.tip without extra optimizations<br>
Running loopUnrolling without unrolling...<br>
Total runtime for 10 runs:<br>
2.80

Building with -lu flag...<br>
Running loopUnrolling with -lu...<br>
Total runtime for 10 runs:<br>
.53

##### .ll file comparison

In this scenario loop unrolling split the inner while loop into 10 increment statements which were then
combined through the instruction combine pass into a singular add 10 as evidenced by looking at the .ll file where the instruction turned into

header2:                                          ; preds = %header2.preheader
br label %body2

body2:                                            ; preds = %header2
br label %body2.1

body2.1:                                          ; preds = %body2
br label %body2.2

body2.2:                                          ; preds = %body2.1
br label %body2.3

body2.3:                                          ; preds = %body2.2
br label %body2.4

body2.4:                                          ; preds = %body2.3
br label %body2.5

body2.5:                                          ; preds = %body2.4
br label %body2.6

body2.6:                                          ; preds = %body2.5
br label %body2.7

body2.7:                                          ; preds = %body2.6
br label %body2.8

body2.8:                                          ; preds = %body2.7
br label %body2.9

body2.9:                                          ; preds = %body2.8 <br>
%incrementTmp.9 = add i64 %x.0, 10<br>
br i1 false, label %body2.10, label %header1.loopexit

Can be found. This effectively reduced the number of instructions by a factor of N where 
N is the end range of the upper loop, in this scenario 10. Here loop unrolling shows its benefit to 
improve SIP/TIP code through providing an avenue for instruction combination.
Further optimization appears to be required in order to prune these unnecessary headers and breaks.
It also seems that this optimization works better at larger volumes so that the factor of N reduction is more relevant.
I attempted testing it at a lower external loop iteration number, but that lead to unfruitful results, possibly due to the repeated breaks and headers


#### Optimization 2, Loop optimization: loop simplify control flow graph 
Benchmark `loopUnrolling.tip` with -lu and -lscfg

##### Benchmark results:

Building with -lu flag...<br>
Running loopUnrolling with -lu...<br>
Total runtime for 10 runs:<br>
.53

Building with -lu and -lscfg flag...<br>
Running loopUnrolling with -lu and -lcsfg...<br>
Total runtime for 10 runs:<br>
.10

Approximately a 5x increase in speed as compared to just loop unrolling. 

##### .ll file comparison

header2.preheader:                                ; preds = %header1<br>
%incrementTmp.9 = add i64 %x.0, 10<br>
br label %header1

The loop cfg simplifier recognized that all those headers that just broke to the next header were unnecessary and removed them.
This significantly reducing the amount of instructions that have to be run.

#### Optimization 3, Jump Threading
Benchmark `loopUnrolling.tip` with -jt

##### Benchmark results:

Building jumpTheading.tip without extra optimizations<br>
Running jumpThreading without jump threading...<br>
Total runtime for 10 runs:<br>
2.00

Building with -jt flag...<br>
Running jumpThreading with -jt...<br>
Total runtime for 10 runs:<br>
1.64

Here there is an apprixamtely 25% decrease in runtime between the optimized and non optimized versions

##### .ll file comparison

ifmerge3:                                         ; preds = %entry<br>
%calltmp = call i64 @foo(i64 4)<br>
br label %ifmerge9<br>

then4:                                            ; preds = %entry<br>
%calltmp11 = call i64 @foo(i64 8)<br>
br label %ifmerge9<br>

ifmerge9:                                         ; preds = %then4, %ifmerge3<br>
%q.2 = phi i64 [ 8, %then4 ], [ 4, %ifmerge3 ]<br>
ret i64 %q.2<br>
}



in the jump thread optimized one and 



then3:                                            ; preds = %entry<br>
%calltmp = call i64 @foo(i64 4)<br>
br label %ifmerge3<br>

ifmerge3:                                         ; preds = %entry, %then3<br>
%q.0 = phi i64 [ 4, %then3 ], [ 0, %entry ]<br>
br i1 %ifcond.not, label %then4, label %ifmerge4<br>

then4:                                            ; preds = %ifmerge3<br>
%calltmp11 = call i64 @foo(i64 8)<br>
br label %ifmerge4<br>

ifmerge4:                                         ; preds = %ifmerge3, %then4<br>
%q.1 = phi i64 [ 8, %then4 ], [ %q.0, %ifmerge3 ]<br>
%_lttmp13 = icmp slt i64 %q.1, 2<br>
br i1 %_lttmp13, label %then5, label %ifmerge9<br>

then5:                                            ; preds = %ifmerge4<br>
%calltmp18 = call i64 @foo(i64 9)<br>
br label %ifmerge9<br>

ifmerge9:                                         ; preds = %ifmerge4, %then5<br>
%q.2 = phi i64 [ 9, %then5 ], [ %q.1, %ifmerge4 ]<br>
ret i64 %q.2<br>
}

in the standard.

Jump threading eliminated then3, ifmerge4, and then5 blocks as it realized they were unnecessary reducing the amount of instructions required, speeding up the program. 

#### Optimization 4, Sparse Conditional Constant Propagation and Dead Code Removal
Benchmark `constantVariables.tip` with -sccp

##### Benchmark results:

Building constantVariables.tip without extra optimizations<br>
Running file without SCCP...<br>
Total runtime for 10 runs:<br>
7.40

Building with -sccp flag...<br>
Running Optimizing with -sccp...<br>
Total runtime for 10 runs:<br>
7.11

Here there is approximately a 4% decrease in runtime between the optimized and non optimized versions

##### .ll file comparison

%calltmp = call i64 @addConstant(i64 99)
%calltmp.neg = sub i64 0, %calltmp
%divtmp = sdiv i64 %iter.0, %calltmp
%subtmp = add i64 %iter.0, 99
%addtmp = add i64 %subtmp, %calltmp.neg
%addtmp11 = add i64 %addtmp, %divtmp
%divtmp15 = sdiv i64 %answer.0, %addtmp11
%iter.0.neg = sub i64 0, %iter.0
%subtmp17 = add i64 %answer.0, %iter.0.neg
%addtmp18 = add i64 %subtmp17, %divtmp15



in the SCCP optimized one and



%calltmp = call i64 @addConstant(i64 99)
%calltmp5 = call i64 @addConstant(i64 99)
%calltmp.neg = sub i64 0, %calltmp
%subtmp = add i64 %calltmp.neg, %iter.0
%divtmp = sdiv i64 %iter.0, %calltmp5
%addtmp = add i64 %subtmp, %divtmp
%divtmp13 = sdiv i64 %answer.0, %addtmp
%iter.0.neg = sub i64 0, %iter.0
%subtmp15 = add i64 %iter.0.neg, %answer.0
%addtmp16 = add i64 %subtmp15, %divtmp13

in the standard.

The SCCP optimization removed dead code from the function by using a single call to addConstant to complete the necessary calculations. 

#### Optimization 5, Function Merging
Benchmark `functionMerge.tip` with -intop

##### Benchmark results:

Building functionMerge.tip without extra optimizations<br>
Running file without function merging...<br>
Total runtime for 10 runs:<br>
11.30

Building with -intop flag...<br>
Running Optimizing with -intop...<br>
Total runtime for 10 runs:<br>
10.59

Here there is approximately a 7% decrease in runtime between the optimized and non-optimized versions

##### .ll file comparison

define internal i64 @arrayAdd(i64 %x) {
entry:
%addtmp = add i64 %x, 10
ret i64 %addtmp
}

define i64 @_tip_main() {
entry:
br label %header1

header1:                                          ; preds = %body1, %entry
%answer.0 = phi i64 [ 0, %entry ], [ %addtmp16, %body1 ]
%iter.0 = phi i64 [ 0, %entry ], [ %incrementTmp, %body1 ]
%_lttmp = icmp slt i64 %iter.0, 25000000
br i1 %_lttmp, label %body1, label %exit1

body1:                                            ; preds = %header1
%calltmp = call i64 @arrayAdd(i64 99)
%calltmp5 = call i64 @arrayAdd(i64 99)


in the function merged optimized one and



define internal i64 @arrayAdd(i64 %x) {
entry:
%addtmp = add i64 %x, 10
ret i64 %addtmp
}

define internal i64 @arrayAddClone(i64 %x) {
entry:
%addtmp6 = add i64 %x, 10
ret i64 %addtmp6
}
define i64 @_tip_main() {
entry:
br label %header1
header1:                                          ; preds = %body1, %entry
%answer.0 = phi i64 [ 0, %entry ], [ %addtmp16, %body1 ]
%iter.0 = phi i64 [ 0, %entry ], [ %incrementTmp, %body1 ]
%_lttmp = icmp slt i64 %iter.0, 25000000
br i1 %_lttmp, label %body1, label %exit1
body1:                                            ; preds = %header1
%calltmp = call i64 @arrayAdd(i64 99)
%calltmp5 = call i64 @arrayAddClone(i64 99)

in the standard.

The function merging optimization removed 2 congruent code blocks within the function and united the functionality within the program. The example did not have a very significant optimization increase but the program was more space efficient, and the optimization would have a greater effect in larger programs.  