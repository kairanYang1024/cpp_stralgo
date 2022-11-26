#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <map>

std::vector<std::vector<int>> prep_badchar_skiptable(std::string& P, std::string& alphabet);
int bmoore_search(std::string& P, std::string& T, std::string& alpha, std::vector<int>& outList);

void print_matrix(std::vector<std::vector<int>>& X) {
    for (int i = 0; i < (int) X.size(); ++i) {
        std::cout << "{ ";
        for(int j = 0; j < (int) X[i].size()-1; ++j){
            std::cout << X[i][j] << ", ";
        }
        std::cout << X[i][X[i].size()-1] << "}" << std::endl;
    }
}

int main(int argc, char** argv) {
    /**
     * z-algorithm: O(p+t), alphabet-independent solution, however, for other problems:
     * searching a set of patterns (Aho-Coarsick) -- z-algo tend not to do very well
     * also, sublinear character comparison is achievable (pre-processing the string): Boyer-Moore
     * which allows to have pattern (P) ahead of time by preprocessing at O(|P|)
     * 
     * overlap between z-algo and bmoore: 
     * zalgo: learn from (past values) to avoid re-computation now (posterior)
     * bmoore: learn at present values to infer future sequence (ad-hoc)
     * 
     * e.g. T="carl carried the cat", P="cat"
     * in T[2]='r', it tells that at position 2 of T, the char in string we interested in (P) is never there (at 2 of P).
     * thus, at i=1, i=2, we cannot have an instance "cat" within T (ruled out) -- skip alignment 1, 2
     * 
     * e.g. T="GTAGATGGCTGAT..."
     *       P="TAGAC" aligned at i=1, how many alignments can skip? 3, skip starting 'A', 'G', 'A' but
     *  T got another 'T' at alignment 4 after i=1, so cannot skip alignment at this (bad character)
     * 
     * e.g. T="AAABABAAA..."
     *       P="AABBB", bad char at i=4 where T[4] = 'A' != P[3] = 'B', how many alignments can skip?
     *   only 1, AABBB having bad char unmatched but
     *            AABBB has the bad char 'A' actually matched to the pattern at this position
     * (somewhat we are going to be dumber and ignore the fact that AABBB does not match segment BABAA
     *  despite second char is 'A' -- cuz it will require us to look back, and we don't wanna do that)
     * 
     * rules for Boyer-Moore:
     * upon mismatch: skip alignments until (1) mismatch become a match like in prev sample;
     * (2) P moves past the mismatched character (T[4] = 'A' in this case)
     * (3) if no mismatch, do not skip
     *
     * can we skip more and look at less characters (only comparing a character pair `once`)
     * notice that when the rightmost (only) character is mismatching, we can skip all chars on the left of the bad char
     * (like in first example, car vs cat --> we locate r & t immediately...from right)
     * .: right-to-left scanning (on pattern string P), only determines skip size to be maximized
     * 
     * how do we put the rules in practice? --> pre-processing without ever knowing the text
     * e.g. P: (T,C,G,C), Alpbet: (A,C,G,T), construct a table to track skips 
     * (for a particular situation, how many chars to skip?)
     * assume we found a mismatch of T[i] = T, P[i] = C, then to find T in P we need to skip 2 chars (C,G)
     * thus matrix[alphab='T'][pattern='C'] = 2
     * assume we found mismatch of T[i] = A, P[i] = C, there is no 'A' in P, thus can skip 3 chars (C,G,C)
     * here T[i] is all possible characters could appear in text, namely how the alphabet is doing
     * so that is how constructing a matrix is good about.
     * for char c in P:
     *   for char a in Alpha:
     *     construct skip length on encountering a at P position of c 
     *     by finding the closest previous instance of c to the left of a, this is constant operation, why?
     * 
     * keep tracking skip length --> look up value at matrix, then validate the edge case (on string) to really sure
     * we skipped that much.
     * 
     * BONUS: is this O(p+t)? --> not so, at worst case:
     * [AAA]AAAAAAAAAAAA... --> each check of full match is O(p) and we did this t times by just according to bad char
     * refinement: "strong" good suffix rule & Gaili rule (not covered)
     * "weak" suffix rule: T[ACAGAC] comp to [ACATAC] --> bc rule says skip 3, good suffix rule also says skip at least 3
     * but its skippyness can cause full match being ignored, thus do not seek for suffix corresp. to suffix, but
     * a (prefix-suffix) partial match, where seeking any prefix of the matched suffix in T would be a good stretch of our search field
     * 
     * using bad char rule or good suffix rule, whichever said skipping more to achieve skipping more. 
     */
    if(argc != 4) {
        std::cerr << "Error: input not supplied with meaningful strings\n";
        std::cerr << "./a.out [alphabet] [pattern] [text]\n";
        return 1;
    }
    std::string alpha = argv[1];
    std::string P = argv[2];
    std::string T = argv[3];
    std::cout << "Pattern is: " << P << "\n";
    std::cout << "Text is " << T << "\n";
    std::vector<std::vector<int>> skiptable = prep_badchar_skiptable(P, alpha);
    print_matrix(skiptable);
    std::vector<int> outlist;
    int comps = bmoore_search(P, T, alpha, outlist);
    for(int match : outlist) {
        if(match == -1) {
            std::cout << "No match of pattern " << P << " in text " << T << std::endl;
            break;
        }
        std::string display = T.substr(0, match);
        display += "[";
        display += T.substr(match, P.length());
        display += "]";
        display += T.substr(match+P.length(), T.length());
        std::cout << "Matching index at " << match << " with instance " << display << std::endl;
    }
    std::cout << "The number of char comparison in Boyer-Moore algorithm (basic) is " << comps;
    std::cout << " with theretical linear benchmark of " << P.length() + T.length();
    std::cout << " and theretical brute-force maximum of " << P.length() * T.length() << std::endl;
    return 0; 
}

//what if using a set of char alpha? convert to set by iterator
std::vector<std::vector<int>> prep_badchar_skiptable(std::string& P, std::string& alpha) {
    std::map<char, int> last_occur;
    std::vector<std::vector<int>> badchar_skiptable;
    badchar_skiptable.reserve(alpha.length()); //rows corresponding to alphabet characters, reserve memory

    // record all alphabetical chars initially having -1 positional value in pattern
    // meaning the "closest previous position is in front of pattern"
    for(char a : alpha) {
        last_occur[a] = -1; 
        //initialize the 2d vector's rows to empty vectors
        std::vector<int> row;
        row.reserve(P.length());
        badchar_skiptable.push_back(row);
    }

    //O(|P||Alp|) complexity of construction
    for(size_t i = 0; i < P.length(); i++) { 
        //for each character in pattern, compute skip length when encountering possible alpha[j] in text
        char p = P[i]; 
        for(size_t j = 0; j < alpha.length(); j++) {
            char a = alpha[j];
            //assume the alphabet characters's closest instance are initialized
            //skip distance=0 (not 1) is defined as moving window to next char 
            badchar_skiptable[j].push_back((i - last_occur[a])-1); 
        }
        last_occur[p] = i; //update last occurrence of character p on pattern's index
    }
    return badchar_skiptable;
}

// return # of character comparisons in the algorithm
int bmoore_search(std::string& P, std::string& T, std::string& alpha, std::vector<int> & outList) {
    std::vector<std::vector<int>> badchar_skiptable = prep_badchar_skiptable(P, alpha);
    std::map<char, int> get_alphabet_idx; //reverse of alpha char array, but we cant do enumerate() in c++
    for(int i = 0; i < alpha.length(); i++) {
        char a = alpha[i];
        get_alphabet_idx[a] = i;
    }
    // match backward, since we do not skip when we match, 
    // some special format may have the following code run in O(n*m) (i.e. worst-case)
    // TODO: implement <good suffixes> criterion in the future
    size_t l = 0;
    int compcount = 0;
    while(l + P.length() <= T.length()) {
        //search backward
        bool match = true;
        for(int pindex = ((int)P.length())-1; pindex >= 0; pindex--) {
            compcount++;
            if(P[pindex] != T[l + pindex]) { //bad character exists at T[l+pindex]
                //the index of bad character in alphabet, is O(1) by hashing
                int badcharindex = get_alphabet_idx[T[l+pindex]]; 
                //retrieve skip length from the array constructed
                int skiplength = badchar_skiptable[badcharindex][pindex]; 
                match = false;
                //skip the inferrable string segment, even if reach the end we need to let it exit loop
                l += skiplength; 
                break;
            }
        }
        if(match) {
            outList.push_back((int)l); //record matching index, do not skip when match is true
        }
        l++; //no matter skipped or not, we need to increase l 
    }
    if(outList.empty()) outList.push_back(-1);
    return compcount;
}
