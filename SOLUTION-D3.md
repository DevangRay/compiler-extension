

Our test plan for Deliverable 3 was based around code coverage, similar to Deliverable 2. Saidamir and I covered all the code we added in this Deliverable except for the print function of the Array type. We tried to achieve code coverage through a system test, since that was how the print function for the Boolean Type was covered, but we ran into errors as we haven't extended code coverage for SIP. As a result, we have committed to testing the function in our next Deliverable when we are able to implement code coverage for arrays. 

We added tests to `/u/gbc2rx/sipc-ray_umarkhodjaev/test/unit/semantic/types/constraints/TypeConstraintCollectTest.cpp` rather than making a new file specifically for SIP Tests as we wanted to take advantage of the helper functions provided in the Test file. We followed the patterns provided by the TIP tests already found in the file. To achieve high code coverage we wrote at least one test for every endVisit function we added to `TypeConstraintVisitor.cpp`, and added multiple tests to ensure full code coverage and ensuring every facet of our added code is hit by tests.  