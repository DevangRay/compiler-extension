You should briefly describe any tricky aspects of your solution, any design alternatives you
considered and how you decided among them, and your approach to testing and achieving
high test coverage.

#### Difficulties:
The learning curve of the project was kind of rough. Just trying to figure out how the tree traversal worked and the exact construction of the methods required. We considered a few design alternatives, mainly merging the decrement and increment statement into one item (similar to AST binary expression), but ultimately decided against it because of the change required in other parts of the code that were working that I didn't really want to touch. While it may add some extraenous code throughout the compilation process, we decided to prioritize a more simple implementation over conciseness.

Code coverage was also just kind of annoying becuase we would always forgot to execute some scripts and the code coverage would be a mess and not be working and then having to spend like 10 minutes figuring out exactly what I forgot to make sure it ran properly this time. It took practice to get accustomed to the practice of keeping track of code coverage, but implementation did get easier as time went on. 


#### Approach to testing:
It seems that this portion of the testing largely didn't include major areas where programs could be tossed and seemed to not exhibit any scenarios where edge cases were a problem. Therefore we mainly focused on doing base case tests with single item and multi-item expressions to ensure we covered our bases. We made sure to prioritize code coverage, meaning that every facet of implementation for each ASTNode is reached by some test. 


#### Achieving high test coverage:
First item was to exclude the lines that shouldnt be covered by excluding the codegen stubs and the final } that often had been stated as uncovered code to avoid visual and informational clutter. 
Then since there weren't a lot of branching we just ensured that our tests covered all the function we created either through explicit usage or through the frameworks/helper functions provided by the items that were originally in the test files. 
In terms of testing implementation, we first did the ASTNodeTests to ensure the accept functions were working and covered, then did the PrettyPrinter tests to ensure the PrettyPrinter code was working. Then we achieved 100% code coverage by expanding PreOrderIteratorTests to include our new nodes which called the GetChildren and Print functions of each AST node type. Then the ASTPrinterTests also covered the print functions of each node. 
