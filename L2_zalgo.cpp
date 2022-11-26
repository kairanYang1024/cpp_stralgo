#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <cstdlib>

#define TERMINAL ("$")

int* get_zvals(std::string&P, std::string& T);

int main(int argc, char** argv) {
    /*
    naive approach 
    pros: easy to implement, correctness (easy to debug)
    cons: slow to compute (time complexity: (# of alignments) * (cost of alignment))
    in the naive approach, ((m-n+1) * n) where m >> n, will have O(m * n) where
    m = len(T), n = len(P).
    For real life problem like genome reading, len(T) ~ 3 billion compare with len(P) ~ 100 - 150 nuclides

    z-algorithm: assumption-less solution to solve exact pattern matching on O(m+n) time bounding.
    z-value: given str S, Z_i(S) is the length of the longest substring in S starting at position i that matches
    a **prefix** of S

    e.g. S= TTCGTTAGCG. Z_1(S) = len("T") = 1, Z(2) = len("") = 0, Z(3) = 0, Z(4) = len("TT") = 2
    Z(5) = len("T") = 1, everything else is 0.
    by definition, Z_0(S) = len(S)

    explicitly comparing characters: for |S|-1 substrings, the number of comparisons are:
    1+2+3+...+(|S|-1) = O(|S|^2)

    However, z-value are self-indexing (self-referencing),
    let S = P$T where $ means terminal character that exists outside of the alphabet (in constr P & T)
    compute Z(S) = [NA, 1, 0, 2, 2, 2, 1], for those, i=3,4,5 are matched substring of T to P
    because Z(S)[i] == |P| for all i there, use $ to tell the impossibility of matching before and at index of $
    in the concatenated string S, minus (len(S)+1) to locate the real string T.
    */ 

   // z-values are prefix-comparisons, thus by definition, a repetitive segment has recurring z-value
   // but we need to make sure the next position is a closure character (differ)
   // [ABC$ABC] has recurring z-val but [AAA$AAAA] does not in the first AAA after $
    std::string P = "AA";
    std::string T = "AAAA";
    int* zs = get_zvals(P, T);
    std::cout << "[";
    for(int i = 0; i < P.length()+1+T.length(); i++) {
        std::cout << zs[i] << ", ";
    }
    std::cout << "]\n";
    delete[] zs;
    return 0;
}

//inspired from https://www.geeksforgeeks.org/z-algorithm-linear-time-pattern-searching-algorithm/
int* get_zvals(std::string& P, std::string& T) {
    std::string S = P + TERMINAL + T;
    int* zvals = new int[S.length()]; //the zvals array for S
    memset(zvals, 0, S.length()*sizeof(int));
    zvals[0] = S.length();
    
    //reformed: if I know z[i] = k, then z[i+k] have to be equal to 0
    //since S[i,... i+k-1] matches P[0,...,k-1], so S[i+k] != P[k], otherwise z[i] != k
    //thus we can easily prune some search space out.

    //'irl' trick: i=curr index, r=rightmost idx having a match at z[l], l=idx of z-val which r belongs
    //if we know z[l], we can fill in interval [l+1, r] perfectly by copying from [1, ..., z[l]]
    int l = 0; //base case to trigger search from i=1, not really accessing S[0] from a sliding window perspective
    int r = 0;
    for(int i = 1; i < S.length(); i++) {
        if(r < i) { //initiate another search from i, increase r index by pattern matching
            l = i;
            r = i;
            while(r < S.length() && S[r] == S[r - l]) {
                r++;
            }
            zvals[i] = r - l; //number of offsets grew is the amount of z-value at index i
            r--; //cancel the last index comparison, will be < l when while-loop is false
            //but will restore to i in the next comparison
        } else {
            if(zvals[i-l] < r-i+1) { 
                //in z array, r-i+1 is the theretical maximum value of z[i] given z[l] is r-l+1 (as long as the combo ends -- delimited by T not P)
                //r-i+1 cannot be greater than max(zvals[i-l]) on part of string such that i belongs to T
                //since zvals[i-1] is limited by len(P), i.e., the search is delimited by $
                //to prevent z-value exceeds limit on length of pattern string.
                //e.g. [A, B, A, B, $, A, B, A, B, ...]
                //         0  2  0     4  0  2  0, at i=6, l=5, r=8, r-i+1=3 but zvals[1] = 0 < 3, thus
                //since zvals[1] is known and pattern S[5:8] identical to S[0:3] -- from zvals[l] = 4 (thus P), 
                //we know zvals[6] = zvals[1], so no substring prefix starting at i=6 is meaningful to explore  
                zvals[i] = zvals[i-l]; //so we follow the previous result
            } else if(zvals[i-l] > r-i+1) { 
				//the matched prefixed at i is not as long as at i-l
				//e.g. [A, A, A, A, $, A, A, A, B, ...]
				//         3  2  1  0  3  2  1, at i = 6, we should pick realistic one and not believing in the old way
				zvals[i] = r-i+1; //we will truncate to these since Z[i-l] at current setting is unreasonable
			} else { //inconclusive case, namely Z[i-l] == r-i+1
                //we are not known what's after r, perform another search starting at r
                //if i < len(P), this repeatedly would be barred at $, served as inferring z-val on rest of indices of pattern string
                //else for i in terr of T, if r-i+1 <= zvals[i-l], then we are in some special region of repetitive search
                //so we need to advance r to the newest point we know so far, and accordingly change l to accomodate to this new search
                l = i;
                //explanation on 'special region': the segment explored by r on T is not limited by T but by P, 
                //so in zvals[i-l] must be delimited by $ to have the following while loop executed
                //the condition on S[r] == S[r-l] always hold true for the initial r, since zvals[i] > 0 here
                //thus, we skip current r since it has already be compared before.
				r++;
                //but not so after that, in fact, the while loop only will increase r in the case where
                //T has more repetitive characters matching pattern P to be revealed
                //e.g. [A, A, A, A, $, A, A, A, A, -- A] the new one is A (and again delimited by $), thus we will 
                //have zval[6] = 4 instead of 3 if followed zvals[1]
                
                while(r < S.length() && S[r] == S[r-l]) {
                    r++;
                }
                zvals[i] = r - l;
                r--;//cancel the last index comparison
            }
        }
    }
    return zvals;
}
