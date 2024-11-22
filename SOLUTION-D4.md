You should briefly describe any tricky aspects of your solution, any design alternatives you
considered and how you decided among them, and your approach to testing and achieving
high test coverage.

### Challenges
This Deliverable was one of the more challenging projects of the semester. A lot of adjustment was needed
trying to wrap around the fact that we are making code to generate code, and as such all the logic is inside the
llvm bitcode. Other than that we discovered several problems that weren't found in previous submissions which definitely slowed
us down. On the bright side chatgpt was pretty good at making some of the code which was nice.

### Design Alternatives
For ternary expressions we originally used the CreateSelect until Professor Dwyer mentioned in class the side effects of codegening both elements of the ternary so then we considered using PHI nodes, but ultimately decided against it because it seemed kind of hard and required
more understanding at and work than just using a temporary variable to store values. We also short circuited the boolean and and or operators
until we realized we didn't have to so we just switched back to built in llvm_ir CreateAnd and CreateOr. Other than that we didn't really consider any design alternatives

### Testing Approach
For testing we avoided doing unit tests. We started on the CodeGenFunctionsTest to check for null code generations, but the Professor mentioned in class we didn't really
need to extend those for SIP so we stopped doing those.
For testing of the functionality of the code generation we first aimed for close to 100% code coverage, (ignoring any missing coverage on error for null code gen), by writing a simple base case test.
Then once we got that for every new feature of sip, we then focused on making more complicated tests for each of the new features, if the feature was complex enough for more tests. This involved a variety of strategies such as
nesting ternary expressions, loops, etc to ensure there was no spill over with CreateAllocs and variables. We also had chatgpt come up with test cases when we had a hard time coming up with them.