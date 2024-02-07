/*
LP Assignment 1
Mohammad Shoaib Ansari
BT21CSE063
Semester 6
*/

#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <string>

using namespace std;

// Define the structure for memory blocks
struct Block;

// Function declarations
void initMemory();
int allocate(int, string);
bool deallocate(string);
void compact();
void printMemory();

// Constant for total memory size
const int MEMORY_SIZE = 64 * 1024 * 1024;   // 64 MB 

// Memory block structure
struct Block {
    int startAddress;
    int size;
    int refCount;
    vector<string> names;

    // Constructor for initializing a block
    Block(int startAddress, int size, string id, int refCount = 0) {
        this->startAddress = startAddress;
        this->size = size;
        this->refCount = refCount;
        this->names.push_back(id);
    }
};

// Lists to store free and used memory blocks
list<Block> freeBlocks;
list<Block> usedBlocks;

// Output file stream
ofstream outputFile;

// Initialize memory with a single free block covering the entire memory size
void initMemory()
{
    Block block(0, MEMORY_SIZE, "0");
    freeBlocks.push_back(block);
}

// Allocate memory for a block of specified size and identifier
int allocate(int size, string id) 
{
    // Iterate over the freeBlock list
    for(list<Block>::iterator it = freeBlocks.begin(); it != freeBlocks.end(); it++)
    {
        // Find the first free block that is large enough
        if(it -> size >= size)
        {
            Block newBlock(it -> startAddress, size, id);
            newBlock.refCount++;
            usedBlocks.push_back(newBlock);

            // Update the free block's start address and size
            it -> startAddress += size;
            it -> size -= size;

            // Remove the free block if its size becomes 0
            if(it -> size == 0)
            {
                freeBlocks.erase(it);
            }

            return newBlock.startAddress;
        }
    }

    // If no free block is large enough, perform compaction and retry allocation
    compact();

    // Repeat the same process after compaction
    for(list<Block>::iterator it = freeBlocks.begin(); it != freeBlocks.end(); it++)
    {
        // Find the first free block that is large enough
        if(it -> size >= size)
        {
            Block newBlock(it -> startAddress, size, id);
            newBlock.refCount++;
            usedBlocks.push_back(newBlock);

            it -> startAddress += size;
            it -> size -= size;

            if(it -> size == 0)
            {
                freeBlocks.erase(it);
            }

            return newBlock.startAddress;
        }
    }

    // If allocation is still not possible, print an error message
    outputFile << "Error: Not enough memory to allocate " << size << " bytes.\n\n";
    return -1;
}

// Deallocate memory for a block with a given identifier
bool deallocate(string id)
{
    for(list<Block>::iterator it = usedBlocks.begin(); it != usedBlocks.end(); it++)
    {
        // Find the block with the given id in its names vector
        vector<string> v = it -> names;
        for(int i = 0; i < (int)v.size(); i++)
        {
            if(v[i] == id)
            {
                it -> refCount--;
                it -> names.erase(it -> names.begin() + i);

                // If the block is no longer referenced, move it to the free block list
                if(it -> refCount == 0)
                {
                    Block newBlock(it -> startAddress, it -> size, "0");
                    freeBlocks.push_back(newBlock);
                    usedBlocks.erase(it);
                }

                return true;
            }
        }        
    }

    // If no block is found with the given id, print an error message
    outputFile << "Error: No block found with ID = " << id << ".\n\n";
    return false;
}

// print the contents of the used and free lists
void printMemory() 
{
    outputFile << "Used blocks:\n";
    for(list<Block>::iterator it = usedBlocks.begin(); it != usedBlocks.end(); it++)
    {
        outputFile << "Start address: " << it -> startAddress << ", Size: " << it -> size << ", Ref count: " << it -> refCount << ", Names: ";
        for(int i = 0; i < (int)it -> names.size(); i++)
        {
            outputFile << it -> names[i] << " ";
        }
        outputFile << "\n";
    }

    outputFile << "\n";

    outputFile << "Free blocks:\n";
    for(list<Block>::iterator it = freeBlocks.begin(); it != freeBlocks.end(); it++)
    {
        outputFile << "Start address: " << it -> startAddress << ", Size: " << it -> size << "\n";
    }
    // outputFile << "\n";
    outputFile << "----------------------------------------------------------------------------------------------------------\n";
}

// Compact the memory by moving used blocks to lower addresses and creating a single free block with remaining memory
void compact()
{
    // Sort the used blocks by their starting addresses
    usedBlocks.sort(
        [](const Block& a, const Block& b) {
            return a.startAddress < b.startAddress;
        }
    );

    // Move the used blocks to the lower addresses
    int start = 0;
    for(list<Block>::iterator it = usedBlocks.begin(); it != usedBlocks.end(); it++)
    {
        it -> startAddress = start;
        start += it -> size;
    }

    // clear the free list and add one block with the remaining memory
    freeBlocks.clear();
    Block block(start, MEMORY_SIZE - start, "0");
    freeBlocks.push_back(block);
}

// Assign the value of one block to another
void assignment(string lhs, string rhs)
{
    for(list<Block>::iterator it = usedBlocks.begin(); it != usedBlocks.end(); it++)
    {
        // Find the block with the given id in its names vector
        vector<string> v = it -> names;
        for(int i = 0; i < (int)v.size(); i++)
        {
            if(v[i] == rhs)
            {
                it -> names.push_back(lhs);
                it -> refCount++;
                return;
            }
        }        
    }

    // If no block is found with the given id, print an error message
    outputFile << "Error: No block found with ID = " << rhs << ".\n\n";
}

int main()
{
    // Initialize memory
    initMemory();

    // Open input file
    ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open input file\n";
        return 1;
    }

    // Open output file
    outputFile.open("output.txt");
    if (!outputFile.is_open()) {
        cerr << "Error: Unable to open output file\n";
        return 1;
    }

    // Read input file line by line
    string line;
    while(getline(inputFile, line))
    {
        if(line.find("Allocate") != string::npos)
        {
            int size = stoi(line.substr(line.find("(") + 1, line.find(")") - line.find("(") - 1));
            string id = line.substr(line.find(" ") + 1, line.find("(") - line.find(" ") - 2);
            // cout << size << " " << id << "\n";
            allocate(size, id);
        }
        else if(line.find("Deallocate") != string::npos)
        {
            string id = line.substr(line.find("(") + 1, line.find(")") - line.find("(") - 1);
            // cout << id << "\n";
            deallocate(id);
        }
        else if(line.find("Assignment") != string::npos)
        {
            string lhs = line.substr(line.find("(") + 1, line.find(",") - line.find("(") - 1);
            string rhs = line.substr(line.find(",") + 2, line.find(")") - line.find(",") - 2);
            // cout << lhs << " " << rhs << "\n";
            assignment(lhs, rhs);
        }
        else if(line.find("Print") != string::npos)
        {
            printMemory();
        }
    }

    // Close the input and output files
    inputFile.close();
    outputFile.close();

    return 0;
}