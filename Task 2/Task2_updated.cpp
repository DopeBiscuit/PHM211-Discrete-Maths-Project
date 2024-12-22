#include <bits/stdc++.h>
using namespace std;

int main(){
    int x = 3;

    // Function pointer to our given circuit boolean logic equation (a is lsb 2^0), (b is 2^1), (c is msb 2^2)
    auto logic1 = [] (int& i) -> bool {
        bool a = (i & 1), b = (i & 2), c = (i & 4);
        return (((a |b) & (a & b)) | (b & (!b)) | (b & c));
    };

    // Function pointer to the simplified boolean expression, hardcoded and changed by the user
    auto logic2 = [] (int& i) -> bool {
        bool a = (i & 1), b = (i & 2), c = (i & 4);
        return (a | c) & b; /// Please place your simplified expression here following Order of Operation.
    };

    // Function pointer to truth table generator using the appropriate logical expression (either logic1 or logic2)
    auto gen_tt = [] (auto fn) -> array<bool, 8> {
        array<bool, 8> tt;
        for(int i{};i < 8; i++)
            tt[i] = fn(i);
        return tt;
    };

    // Function that takes an integer and convert it to its binary constituents
    auto to_str = [](int& n){
        return (to_string(bool(n & 4)) + to_string(bool(n & 2)) + to_string(bool(n & 1)));
    };



    /// Generate the truth table for both the Given circuit and the Simplified expression
    auto tt = gen_tt(logic1);
    auto simple_tt = gen_tt(logic2);

    /// Print both truth tables
    cout << "Original Circuit Truth Table:\n";
    for(int i{};i < 8; i++)
        cout << "\t" << to_str(i) << ":\t\t" << tt[i] << endl;

    bool equal{true};

    cout << "\nSimplified Circuit Truth Table: \n";
    for(int i{};i < 8; i++){
        equal = (simple_tt[i] == tt[i]);    // This checks if all truth table elements are equal or not
        cout << "\t" << to_str(i) << ":\t\t" << simple_tt[i] << endl;
    }

    /// Handle equality or mismatch
    if(equal)
        cout << "\n*Since Truth tables are the same, therefore they are equivalent expressions*" << endl;
    else
        cout << "\n*Since Truth tables are different, therefore they are not equivalent expressions*" << endl;
    cout << endl << "********************************************************************************" << endl;

    /// Handle common satisfying minterms (Print all minterms that satisfies both)
    cout << "Input values that make both circuit satisfiable:\n";
    cout << "A B C: ";
    for(int i{};i < 8; i++){
        if(tt[i] && simple_tt[i])
            cout << to_str(i) << ' ';
    }
    cout << endl << endl;
}

/***
 *                  TEST CASE ---> (logic1 = (A | C) & B)
            * Original Circuit Truth Table:
                    000:            0
                    001:            0
                    010:            0
                    011:            1
                    100:            0
                    101:            0
                    110:            1
                    111:            1

            Simplified Circuit Truth Table:
                    000:            0
                    001:            0
                    010:            0
                    011:            1
                    100:            0
                    101:            0
                    110:            1
                    111:            1

            *Since Truth tables are the same, therefore they are equivalent expressions*

            ********************************************************************************
            Input values that make both circuit satisfiable:
            A B C: 011 110 111

*/