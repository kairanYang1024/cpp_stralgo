#include <iostream>
#include <string>

/**
 * @brief course content: String algorithms & data structures
 * --> foundational content of the topic
 * exact string matching
 * compressed self-indices (related to the search)
 * inexact pattern matching (genomic sequence, spell checker, etc.), harder problem
 * -- most part of this class
 * -- experience practical aspects of various string algorithms
 */


int main() {
    // string: a finite sequence of characters drawn from a (well-defined) alphabet \Sigma, 
    // usually assumed finite {A,C,G,T}, {0, 1}
    // char*: char array (c-string) , and std::string, has abundant of operation supports (an actual class)
    // string operations: concat, substr, equal...
    // "how efficient is my algorithm at searching for a given pattern P?" --> getopt(), strtok() in C
    // "how much memory do I need to allocate for this text file?" --> malloc(), fseek()... in C
    std::string str = "Hewwo mommi";
    std::string tsr = "My namu is cute astraea O.O";

    // size of string s: s.length() 
    std::cout << "length of str: " << str << " is " << str.length() << "\n";
    std::cout << "length of tsr: " << tsr << " is " << tsr.length() << "\n";
    // equality of strings: s.equals(t) interpreted as == in std::string, 
    std::cout << "string equality: " << std::boolalpha << (str == tsr) << "\n";
    std::cout << "string inequality: " << std::boolalpha << (str != tsr) << "\n";
    // using == (without overloading) is going to fail in char*
    char raw[] = "Hewwo mommi";
    char taw[] = "My namu is cute astraea O.O";
    char eaw[] = "Hewwo mommi";
    std::cout << "char* equality: " << std::boolalpha << (raw == taw) << "\n";
    std::cout << "char* inequality:" << std::boolalpha << (raw != taw) << "\n";
    std::cout << "char* supposing equality: " << std::boolalpha << (raw == eaw) << "\n";
    // add string: s.concat(t) will have <s><t>, t.concat(s) is <t><s>
    // in std::string, operator overloading will have them functions properly (==, +)
    // std::string sum = str.append(tsr); <-- this is in-place modification and will change content of caller
    std::string sum = str + tsr;
    std::cout << sum.length() << " is equal to " << str.length() + tsr.length() <<"\n";

    // substring: S is a substring of T iff there exists strings (empty possible) u, v such that
    // T = uSv. T.substr(1, 4) means the substring starting at T[1] and has length of 4
    std::cout << "slicing str at 3 with length 4: " << str.substr(1, 4) << "\n";
    // prefix: special case of substring such that S is a prefix of T iff T = uSv and u = ""
    // suffix: special case of substring such that S is a suffix of T iff T = uSv and v = ""
    std::cout << "tsr prefix of length 4: " << tsr.substr(0, 4) << "\n";
    std::cout << "tsr suffix of length 6: " << tsr.substr(tsr.length()-6, 6) << "\n";
    std::cout << "going out of bound: " << tsr.substr(tsr.length()-3, 11) << 
    " The out of bound portion will have no effect\n";

    //exact pattern matching: a string pattern P and text T, find instance (full length copy) of P in T
    //sliding window: keep slicing substrings of length(P) in T, O(t * p)
    //is naive search a good solution? --> how do we improve the naive algo?
    //when constraints imposed, or no constraints.
    //infinite space? knowing pattern ahead? knowing text ahead?
    return 0;
}