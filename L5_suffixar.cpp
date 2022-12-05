#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <map>

using std::vector;
using std::string;
using std::map;

/**
 * review: suffix trie vs tree:
 * report occurrence of P in T: trie O(p), tree O(p) in optimum
 *        k locations of P in T: trie O(p + t^2) , tree O(p + k) using recursive and leaf index storage
 *        space to build: trie O(t^2) naively, tree O(t) in optimum (paper implementation)
 * suffix tree usage: plagirism detection, given two texts, check portion in common
 *  --> generalized suffix tree (a stree storing many strings)
 * molecular ancestry, using personal gene querying the human collective genome
 * initialization (indexing) --> O(T)
 * query (search) --> O(P) for each query gene (short length)
 * however, storage is expensive
 */

//TODO: suffix array construction methods in this file (after passing the correctness tests)
//URGENT: the entire impelentation is erroneous, need to go on extensive review after final
vector<int> build_sarray(string& t);
vector<int> sarray_search(string& t);

int main(int argc, char** argv) {
    /**
     * suffix array: array sorted lexicograhically to provide good indexing functionalities
     * lexicograpgic ordering: content ordering A < B < C, \null < $ < 0 < A < z defined by ascii
     * matching (arrangement) ordering: "ABAC" vs "ABCD", "ABAC" < "ABCD", have nothing to do with string length
     * "bb$" is larger than "b$b", the third char is irrelevant
     * 
     * a suffix array is a ranking of suffxies based on their lexicograpgical order.
     * std::sort(), a mixture of heapsort, quicksort and mergesort optimized in O(n*logn)
     * theoretical suffix array construction time: O(t log t), but...?
     * --> string copy: take time, malloc or shallow copy?
     * --> string compare: O(t), result in O(t * t log t) = O(t^2 log t) in time
     * optimization: use sort algo awaring of items being sorted are all suffixes of same string 
     * best so far: O(t) linear using divide n conquer
     * 
     * flatten from a suffix tree: traverse in lexicographical order and record suffix index values, O(m)   
     * goal for first-timer: try to build bounded by O(t^2 log t), essentially quadratic or better
     * 
     * search a suffix array: naively O(t * p), each t search up to p characters
     * but array is sorted, do binary search on string suffixes comp to P
     * avoid short-circuting --> check the prev/next element on naive prefix match
     * efficiency: O(p log t + p * t), uh-uh
     * 
     * further optimization: find the interval of including targeting pattern among the suffixes
     * starting at full range (0, t), if curr match P, recurse to (0, t/2-1), ..., until a left bound is returned
     * same principle for the right bound deliniation.(if match, recurse to (t/2+1, t)
     * time complexity O(p log t + k) exact pattern repetition, possible to be improved to O(p + log t), beats suffix tree (benchmark)
     * 
     * both suffix tree and suffix array take in O(t) space, but with diff coefficients, normally suffix array's leading coeff is smaller
     */
    

    return 0;
}