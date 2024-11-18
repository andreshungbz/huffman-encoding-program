# Huffman Encoding Compression Program

Project 2 for the CMPS2131 Data Structures Final Project done by Andres Hung for the 2024-1 semester at the University of Belize.

## Quick Introduction to the Huffman Coding Algorithm

The Huffman Coding Algorithm is a lossless data compression algorithm that assigns a variable-length bit encoding to a character based on its frequency, and encodes a file using the generated encoding. More frequent characters are assigned shorter codes, thus, by writing to file with the encoded bits, space can be saved as opposed to the full ASCII byte representation of the character.

These codes have a special property known as the prefix property, which means that no code is a prefix of another code. This allows for the encoded file to be decoded unambiguously. A binary tree called the Huffman Tree is constructed from the frequency table of characters, from which the encoded Huffman Code is generated. When decoding the compressed file, the same tree is traversed to decode the encoded bits back to the original characters.

In the practical application of writing to file, the representation of the tree must also be stored alongside the encoded Huffman Code. It is the only structure needed to decode the file.

## Project Structure & Implementation Notes

At a high-level overview, the Huffman Tree needed for this algorithm is constructed from the following steps:

1. Create a frequency table of characters from the input file. A hash map data structure is appropriate here.
2. Construct a priority queue of nodes, where each node has a character key and its frequency. The priority queue is implemented as a minimum heap.
3. Construct the Huffman Tree by repeatedly extracting the two nodes with the lowest frequency from the priority queue, creating a new node with the sum of their frequencies, and inserting it back into the priority queue.

Afterward, the Huffman Tree can be used to Huffman Code written to file (The generation of the encoding table also uses a hash map).

The project structure is described as follows:

- `/driver/`: Main driver program used in `main`.
- `/src/`: Contains the header and source files for classes used for the construction of the Huffman Tree. Also contains additional utility functions used in the classes.
  - `/src/huffman_tree`: Contains the class for the Huffman Tree and Node
    - `/src/huffman_tree/components`: Component classes which used in the Huffman Tree class.
    - `/src/huffman_tree/hash_map`: Frequency hash map class used in constructing the Huffman Tree.
    - `/src/huffman_tree/priority_queue`: Priority queue class used in constructing the Huffman Tree.
  - `/src/utils`: Contains definition of a number of utility functions originally factored out of the Huffman Tree class.
    - `src/utils/compression`: Utility functions for compression and decompression. Writes to and reads from file.
    - `src/utils/file`: Utility functions for retrieving information about a file.
    - `src/utils/generate`: Utility functions generating the necessary data members in the Huffman Tree object.
    - `src/utils/instantiate`: Utility functions for reconstructing the Huffman Tree object from the encoded file.

The project uses the C++ 17 standard and project files are provided for compilation with CMake and qmake in the `CMakelists.txt` and `02-huffman-encoding.pro` files respectively. The program should compile correctly on both Windows and Linux/macOS systems.

Further implementation details and reasoning can be found documented in the header files.

## Testing

The `/test/` folder contains some files used for testing with the program. During program execution, the relative or absolute path to a file can be provided. When running the program in your IDE, you can quickly test compression and decompression by using the `../test/regular-txt-file/witw.txt` relative file path.

The three folders in the `/test/` demonstrate the following aspects of the Huffman Coding Algorithm in this implementation:

- For tiny text files, the compressed file will end up having a larger size than the original file. This is because additional data is being written besides the Huffman Code: namely the Tree Representation, the file name and extension, and lastly a header file that is required to delimit each section.
- For regular text files, the compressed file will have a smaller size than the original file. The compression ratio is dependent on the frequency of characters in the file.
- For non-text files, the compression ratio will most likely be minimal. The algorithm is not optimized for binary files, but it can still compress them to some extent.

In conclusion, the Huffman Code Algorithm works best with text files where there is much data redundancy and much space can be saved lossless.

## Other Notes

The Qt Creator IDE on the university computers may have a strange bug where you must make an insubstantial change to the `02-huffman-encoding.pro` file and resave it in order for the build process to complete. Otherwise, it just shows as hanging mid-way the build progress bar. The compiler also does not support the \<filesystem> library.

Maybe this should be a sign to update the Qt Creator IDE, Qt framework, the MinGW compiler, and maybe also install CMake as an alternative build system 👀.

## Acknowledgements

Understanding of the algorithm was referenced from this [Stanford University CS106B article](https://web.stanford.edu/class/archive/cs/cs106b/cs106b.1234/resources/huffman.html) written by Julie Zelenski (with additions from Kenneth Huffman and Schwarz).

The original paper on the algorithm, written by David A. Huffman in 1952, can be found in this [online PDF file](https://compression.ru/download/articles/huff/huffman_1952_minimum-redundancy-codes.pdf).

The provided "The Wind in the Willows" text file in `/test/regular-txt-file/witw.txt` was sourced from [Project Gutenberg](https://www.gutenberg.org/ebooks/289). The text was written by Kenneth Grahame and is in the public domain. Attribution is located in the text file itself.
