# HEG
Huffan's algorithm allows to find the optimal variable-length encoding for a given alphabet. In this work, Huffman's algorithm is implemented following 3 steps:

- analysis of a text file to generate a list of symbols and corresponding frequencies
- creation of a tree-like representation of the optimal encoding using Huffman's algorithm
- generation of the code associated with each symbol by inspecting the tree

In order for the algorithm to be efficient, the sequences of `0`'s and `1`'s obtained by inspecting the tree needs to be packet into a stream of bits. This processing is performed by the `BinaryMessageBuilder` class (hopefully I will soon find a better name for it).