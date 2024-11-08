### Challenges
Definitely had some tricky parts to the implementation. Overall learning how to create the visitors and how
it worked for the type constraint collector was not that hard due to our experience with the AST, however generating
the type constraint rules themselves was relatively difficult especially with the array ones. Specifically array reference,
but we did manage to figure it out eventually by modeling it somewhat off of the deref visitor. Another problem we encountered 
was with getting the code to compile, we had some issues with the linker that was resolved with a quick piazza check after much head banging 
against the wall, and we didn't realize that the substitute-r needed some extra code as well. Other than that it went fairly smoothly.

### Design Alternatives
We didn't really consider any design alternatives mainly because when we went to class on Thursday, the professor managed to give some 
great pointers on how to design the TipArray and TipBoolean types which probably saved us some time. Other than that we followed the examples set by the
other code generally.

### Test Plan - Problems
Our test plan for Deliverable 3 was based around code coverage, similar to Deliverable 2. Saidamir and I covered all the code we added in this Deliverable except for the print function of the Array type. We tried to achieve code coverage through a system test, since that was how the print function for the Boolean Type was covered, but we ran into errors as we haven't extended code coverage for SIP. As a result, we have committed to testing the function in our next Deliverable when we are able to implement code coverage for arrays.

### Test Plan Strategy
We added tests to `/u/gbc2rx/sipc-ray_umarkhodjaev/test/unit/semantic/types/constraints/TypeConstraintCollectTest.cpp` rather than making a new file specifically for SIP Tests as we wanted to take advantage of the helper functions provided in the Test file. We followed the patterns provided by the TIP tests already found in the file. To achieve high code coverage we wrote at least one test for every endVisit function we added to `TypeConstraintVisitor.cpp`, and added multiple tests to ensure full code coverage and ensuring every facet of our added code is hit by tests.