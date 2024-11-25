You are to describe your process for selecting optimizations, describe each optimization that is included,
and describe the evidence that supports its unique benefit in optimizing SIP programs.

### Process
Since SIP added arrays to TIP we first decided to explore optimizations that helped with arrays. We first found loop strength reduction which stated that "
"
Added optimization for new SIP data structures / types.

All benchmarks can be found at
`/test/optimizations`
### Optimizations Included

##### Optimization 1, Loop optimization: loop unrolling
Benchmark `loopUnrolling.tip`

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



##### Optimization 2, Loop optimization: loop unrolling
Benchmark `loopUnrolling.tip`
