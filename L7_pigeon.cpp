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
 * @brief 
 * limitation of exact pattern matching: if you mistype, cannot adjust that using exact pattern matching
 * (or achievable but costly).
 * "bad word" --> "b@d word", "bunny" --> "bnuuy"
 * typo correction/spell check: De Brujin graph
 * genetic sequence: what if OG sequence (T) has damaging code (NA)?
 */

int main(int argc, char** argv) {
    /**
     * problem: input T, pattern P, and distance d
     * output: all positions in T where P has at most d mismatches & edits
     * e.g. P="word", T="there would have been a time for such a word"
     *                         word -- distance 2 match          word -- distance 0 (exact) match
     * e.g. X: 10011                           10011
     *      Y: 00110 --> hamming distance = 3,  00110 --> edit distance = 2 (when force region overlap)
     * a substitution replaces one char with another --> daedalos -> daedalus 
     * hamming distance = minim number of substitutions to turn one str to another.
     * when hamming distance fail: ATATAT vs TATATA, hamming distance = 6
     * 
     * an insertion adds a char and shifting all chars behind 1 back.
     * insertion is relative to the insert position
     * a deletion removes a char and shift all chars behind 1 forward.
     * deletion is relative to the delete position
     * editing distance = minimum number of editings to turn one str to another
     * e.g. X = GCGCT --> delete (G, 0), delete (C, 0) = 2
     *      Y = GCT --> insert (G,0), insert (C, 1) = 2, (other choices available)
     * in general, computing editing distance is dynamic programming.
     * and hamming distance-k strings of str S is a subset of editing distance-k strings (+ins, del)
     * 
     * efficiently finding 1 mismatch of P in T:
     * if P occurred in T with 1 mismatch, then P's paritition (u, v) has either 1 mismatch or no mismatch
     * this is called pigeonhole principle
     * 0) exact-search (T, u), (T, v), grab their occurrence indices
     * 1) compute # of mismatches for each half match by extending the other half (partition) on each
     * 2) return all matches that has errors <= 1, but do not duplicate (track the index)
     * 
     * if search up to k mismatches: partition P to [p0, p1, ..., pk]
     * at least 1 of the segments of P must appear 0 mismatches in T
     * verify a match from a partial match is O(P)
     * pros: use O(P/k) on segment exact match, efficient, flexible in hamming and edit distance (not yet)
     * cons: large k yields small partition, matches many times randomly, bring up verification work of extension
     * k+1 exact pattern matching, one per partition, need to repeat k times
     * 
     * the algorithm only counts mismatches, but not verifying <=d hammering distance \
     * (pigeonhole implie d -> mismatches, but not mismatch->d)
     * need to manually extend to verify.
     * 
     * partition: contiguous ran of characters (designable)? all the same length (not necessarily)
     * 
     * also, FM-index is doable for counting mismatching characters when do the backward selection process
     * (just track incorrect # of matches along the way for P, when incorrect >= mm or P exhausted, return) 
     */
    
    return 0;
}