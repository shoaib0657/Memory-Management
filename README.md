# Memory-Management
In this assignment, we are simulating memory management as is done in interpreted languages like Python or Java. 

When a program requests requests a block of memory dynamically, the system must allocate memory, and when a program terminates or frees memory previously requested, the system must deallocate memory. 

For the sake of this assignment, we will assume we have only 64 MB of memory. Starting at zero memory allocation, we will process several kinds of transactions: </br>
--- Allocate a block of memory dynamically </br>
--- Deallocate a block obtained earlier 

From time to time, we will print out the contents of the data structures involved. 

Write a program in any language to accomplish this. Your program (which may be in several files) should do the following: 

--- Define an array to represent one block of memory from which you can allocate or deallocate.

--- Once allocated, the return value should include the starting address (an integer). 

--- Assignments like a=b, will increase the reference count of the memory previously allocated to a

--- Deallocation only happens when the reference count goes to 0.

--- Create two linked lists of blocks. One list contains blocks that are presently in use, and the other contains blocks that are not presently in use. This will help in allocation of memory.

--- Periodically carry out memory compaction. This will update the pointers as well as the two linked lists.

--- Open and read the input file and carry out the transactions. You can define specific formats for allocation and deallocation.

--- After all transactions, reprint the contents of both lists. If possible, print contents in another output file.

--- Be sure you close the files before the program exit.

--- If there is no sufficiently large block for allocation, then first try to compact the memory and if still not possible then print an error message

Please submit code as well as Documentation of your code along with sample input and output. Your documentation should clearly mention about any assumptions, have some diagrams on how allocations and deallocations are happening.

Also, you will have to use style guidelines given here: https://faculty.cs.niu.edu/~byrnes/csci240/240doc.htm

 
Example [You can define your own transactions]:

a = allocate (500);   // Will allocate 500 bytes, and a will be assigned starting address of 500 bytes
                                // The linked lists will be updated

b = a; // reference count of the allocation will increase by 1, value of b will be value of a

free (a); // reference count will decrease by 1

free (b);  // Deallocate 500 bytes from given starting address value given by b
               // Both the linked lists will be updated
Deadline: 21 Jan 2023
