
You are expected to extend and modify, as necessary, the implementation of the tipc
semantic analyses, to extend semantic unit test suites with new test files beginning with the
SIP character sequence, and to provide a brief description of your work. The description of
your work should in a file SOLUTION-D3.md that you add to the root directory of your project.
You should briefly describe any tricky aspects of your solution, any design alternatives you
considered and how you decided among them, and your approach to testing and achieving
high test coverage.

### Challenges
Definitely had some tricky parts to the implementation. Overall learning how to create the visitors and how
it worked for the type constraint collector was not that hard due to our experience with the AST, however generating
the type constraint rules themselves was relatively difficult especially with the array ones. Specifically array reference,
but we did manage to figure it out eventually by modeling it somewhat off of the deref visitor. Another problem we encountered 
was with getting the code to compile, we had some issues with the linker that was resolved with a quick piazza check after much head banging 
agains the wall and we didn't realize that the substituter needed some extra code as well. Other than that it went fairly smoothly.

### Design Alternatives
We didn't really consider any design alternatives mainly because when we went to class on Thursday, the professor managed to give some 
great pointers on how to design the TipArray and TipBoolean types which probably saved us some time. Other than that we followed the examples set by the
other code generaly. 


