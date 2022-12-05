#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <map>

//TODO: a working example of nary class with ctor, dtor, insertion, deletion like in the hw implementation
//also attains optimal efficiency as taught in class (after semester final exam)

int main(int argc, char** argv) {
    /**
     * Problem: knowing text (T) early, we can preprocess the text T ahead of time
     * assume T >> P: can build index in O(T) and search in O(P), much better querying since P << T.
     * define a substring: (start, length), can enumerate as 
     * length-1 substr: 0 1 2 3 4
     * length-2 substr: 0 1 2 3
     * length-3 substr: 0 1 2
     * encoded as dictionary (map <string, size_t>), however, space complexity is O(|T|^2)
     * how to reduce? string is individual characters set, there's overlap
     * can build a tree and representing character advancements as tree branching.
     * thus a path is a valid substring in T
     * 
     * however, confounding when T = CGTGC, how to differentiate C at 0 and C at 4?
     * solution: make every tree node a dictionary -- essentially a trie
     * trie: a rooted tree storing a collection of (K,V) pairs
     * each edge labeled with a character c in \Sigma
     * for a given node, at most 1 child edge has labeled c for any c in \Sigma
     * each key is spelled out along some path starting from root.
     * 
     * each key's value is stored at the last node in the path (when information is complete & no ambiguity)
     * each node in trie has up to |\Sigma| edges, each edge is potentially a nullptr
     * n-ary tree: collection of static arrays (memory waste but fast, for bigger when alphabet compare to T miniscule)
     * OR
     * a dynamically sized dictionary (map) with recursion in traversal (for smaller) 
     * when string overlapping insert("AB", 1) after insert("AB", 0), just insert 1 in the node's (internal) list
     * 
     * stores all substrings in a trie using narytree: insertions total in O(|T|+1) is possible
     * 
     * how about storing only 1 int in each node's internal?
     * still storing O(T^2) values on O(T^2) insertions, if only 1 int is allowed then tree size will bloat up
     * (to accomodate all substring as nodes)
     */
    
    /**
     * Suffixing: although storage of node is small, still need a lot (T^2) insertions to make sure is correct
     * efficient preprocessing: perform fewer insertions <namely O(T)> to build the tree
     * stores fewer values in index --> just store the suffixes:
     * suffix trie, suffix tree, suffix array, FM index --> still applicable today (see leetcode)
     * 
     * suffix trie: instead of storing t^2 diff substrs, just store t suffixes
     * but what's missing? repetitive occurrences
     * 
     * dummy value: $ at the end of text
     * notice that only leaf nodes in suffix trie stores value (indexing), so $ serves as a dummy node
     * that sole purpose is to store indexes (also would emerge a dummy branch from root that has ($, strlen(T)))
     * difference between a trie and tree: trie's key is char, while tree;s key is string
     * 
     * in trie: insert (T+1) times, storing (T+1) values corresponding to indexing
     * searching a suffix trie: each substring of T is a prefix of some suffix of T
     * each of T's suffixes is spelled along a path from root
     * 
     * thus, when we are querying pattern(s) from a suffix-trie, we can --> 
     * 1) search along path until pattern string exhausted 
     * 2) at the node where string is exhausted, continue search all of its descendants until reaching a leaf
     * 3) record leaf node index value as occurrence of P in T. 
     * bounded by O(h(trie)), where h(T) is the height of the tree, in worst case is linear with T
     * trie's nodes --> can grow in T^2 if T is (a^n)(b^n) pattern where n is half of |T|
     * in real life, unfortunately suffix trie instances are closer to the worst case than best case
     * 
     * --> how to improve this? path compression, when linear edges do not branch, compress to a single edge
     * so that map<char, int> become map<string, int> is useful.
     * the result data structure is called suffix-tree.
     * after path compression, will leave m leaves and at most m-1 internal nodes
     * each internal node has >= 2 children (one of which is $)
     * 
     * when deleting from a dynamic container (map, vector): 
     * while iterator of node's map not end yet:
     *  child = node's child
     *  if(child has only 1 child) 
     *      set temp = child's child
     *      node[new key] = temp
     *      delete child
     *      iterator need to --> 1) use post-increment which will copy current state to be erased and increment
     *      2) use iterator.next() returned from map.erase(it)
     *
     * how to directly construct a suffix tree: build the longest suffix and split accordingly based on label's prefix
     * has O(T) space (because of # of internal nodes bound by # of leaves) and slight longer time.
     * if the tree version just concatenate the char to string, we are still storing O(T^2) amount of info (duplication)
     * to reduce this, just store int pairs instead of strings that representing a substring to T, will be efficient.
     * (also each node's information is constant, 2 ints = 8 bytes)
     * 
     * the canonical algo for both O(T) time and space is on paper ('95) E. Ukkomen Algorithmeca '95 serial 14 
     */

    return 0;
}