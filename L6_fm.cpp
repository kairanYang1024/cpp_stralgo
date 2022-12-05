#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <unordered_map>

using std::vector;
using std::string;
using std::unordered_map;

/**
 * review: suffix tree --> O(n) for P existence, O(n+k) for P occurrences report in T, 
 * O(m) space in optimal construction (generally larger)
 * suffix array --> O(n * log m) for P existence, O(n log m + k) for P occurrences report in T, 
 * O(m) space in general.
 */

//TODO: implement a working FM index class (with subsampling to reduce space)
#define OCCUR_STEP_SIZE 4
#define SARRAY_STEP_SIZE 2

int main(int argc, char** argv) {
    /**
     * basic of FM index (which compresses the space to be less than original text) --> on
     * text transformation called burrows wheeler transform, i.e. reversible permutation
     * how to encode, how to decode, and how to use in search?
     * text rotation: wrapping the string around by shifting k digits left or right
     * once getting the entire wrap of length T, sort the array of permutations lexicographically
     * and BWT(T) is taking the last column of the array of strings
     * 
     * decoding BWT: how can BWT be smaller than original text?
     * prepend BWT string as a column, sort each string entry as row, then prepend again until OG text recovered
     * right context: since every single row is a rotation of each other, we can deduce for each char in BW column,
     * the next char in the OG string is just on the very left of the column, which in turn to be sorted version of OG string
     * so no matter how the sorted string array rbind is, the BW transform text prepending is always valid and 
     * propelling to the original context, so in t prepending, the OG string will be revealed in any one of the rows
     * in the reconstructed string array
     *
     * e.g. right context of "ap[p]le$" is "le$ap", and --> a letter will always have the right context in the BW transformation
     *  (rotational matrix)
     * 
     * T-ranking: giving each char in T, rank of the char = times the char occurred previously in T.
     * not explicitly stored, just there for illustration
     * notice that letters occur in the same order in column F (first col) and column L (last col --> taken as BWT(T))
     * in the rotational matrix
     * LF-mapping: ith occurrence of char c in L and ith occurrence of c in F correspond to the same occurrence in T
     * thus are having the same right context (because are the same character)
     * 
     * Given BWT(T), L is the BWT(L), and F is the sorted BWT(L) with exact letter repetition correspondance in F
     * now, correspondance can be established since L is just F's previous character
     * which is far more efficient than prepend-sort
     * 
     * can also enhance efficiency by the context-given order: F order which is just a sorted string, indexing much easier
     * L is size of (T), and F is representable by array of |Sigma| ints
     * 
     * FM index is BWT with a few small auxilary data structures
     * querying FM index: F column legitimately is head of the suffix array (with index as value), but cannot do binary search
     * looking for range of rows of BWM with P a prefix, start with shortest suffix, match successively longer suffixes
     * (since F sorted), jump between F and L until P exhausted
     * --> scan L column for viable characters -- O(t)
     * --> T suffix information from FL mapping is lost, unlike a suffix array.
     * 
     * compression: after BM transform, a repetitive text is showing consecutive repeating chars
     */
    /**
     * given alphabet(ordered) available, we can actually store F as only a vector of its char's first occurrence
     * FM-index: use to query pattern matching given F, L columns.
     * 1) search for appropriate range matching the last char of P,
     * 2) match last char of P with the first identical letter in F, jump to L and prepend P's matching prefix
     * 3) if construct finishes, report idx (from suffix array -- aux data), else report not found.
     * But: scanning chars in L is slow (O(t)), also not storing suffix index heading the chars in F
     * 
     * to fast scan L, construct occurrence table as in HW8, but this time do so cumulatively and 
     * do that for every character, this effectively reduced lookup of specific char in L given F's location to O(1)
     * which specific char c (ranking)? --> deducible from the occurrence array (but large space)
     * reduce spacing: record only cumulative occurrences at the index of a multiple of a constant (k)
     * i.e. reduction is in constant factor, but it would beat suffix array
     * 
     * to indexing the actual suffix, do a subset of suffix array (e.g. only the even suffix indices),
     * when we find match, report, when lookup failed --> go to L and find previous suffix's indices, 
     * add 1 to that and is guaranteed to be the one you want (add 1 is to compensate the lookup to L recedes 1 from index)
     * this is also O(1) because we are sampling subset of constant factor
     * 
     * to sum up: 1) get_frange() yields the interval of segments in F given char, starting rank and ending rank
     * 
     * 2) get_lrange() takes in the interval segment and get the mapping interval of second char (advanced back in P)
     * is resolvable in O(constant) giving the occurrence table (with some rows intentionally blank)
     * and loop in between frange() and lrange() until either range returned is NULL or P exhausted, O(p)
     * 
     * 3) when P exhausted, we know the frange() matching the entire pattern, access the suffix subset array
     * if access failed, do FL mapping again until the current F index has an entry, add the track steps to that entry
     * and report (O(const))
     * 
    */

    return 0;
}