#include <iostream>
#include <vector>
#include <map>
using namespace std;

bool logic(int& i);
string to_str(int& n);
vector<string> simplify(vector<pair<string, string>>& minterms);

vector<string> simp_minterms;
int main() {
    // inputs truth table for original function
    bool tt[8] = {0}, tt_simple[8] = {0};

    // Generate truth table values
    for(int i{};i < 8; i++)
        tt[i] = logic(i);

    // Simplify logic circuit using Quine McCluskey Method
    vector<pair<string, string>> minterms;
    for(int i{};i < 8; i++)
        if(tt[i]) minterms.push_back({to_str(i), to_string(i)});

    vector<string> simple;
    simple = simplify(minterms);

    // Print original circuit truth table
    cout << "Original Circuit Truth Table:\n";
    for(int i{};i < 8; i++){
        cout << "\t" << (i >> 2 & 1) << ' ' << (i >> 1 & 1) << ' ' << (i & 1) << ":\t\t" << tt[i] << endl;
    }

    cout << endl << "********************************************************************************";

    // Generate Truth table for simplified circuit
    if(simple.size()){
        for(int i{0};i < 8; i++){
            bool in[3] = {(bool)(i & 4),(bool)(i & 2), (bool)(i & 1)};
            bool logic = {false};

            for(auto term : simp_minterms){
                bool temp{true};
                for(int i{};i < 3; i++){
                    if(term[i] == '1'){
                        temp &= in[i];
                    } else if (term[i] == '0'){
                        temp &= !in[i];
                    }
                }
                logic |= temp;
            }
            tt_simple[i] = logic;
        }
    }

    cout << "\nSimplified Circuit Expression:\t";
    for(int i{}; i < simple.size(); i++){
        cout << simple[i];
        if(i == (simple.size() - 1))
            cout << endl;
        else
            cout << " + ";
    }

    if(!simple.size()){
        cout << tt[0];
    }

    // Print simplified expression truth table
    cout << "\nSimplified Circuit Truth Table: \n";
    for(int i{};i < 8; i++){
        cout << "\t" << (i >> 2 & 1) << ' ' << (i >> 1 & 1) << ' ' << (i & 1) << ":\t\t" << tt_simple[i] << endl;
    }

    cout << "*Since Truth tables are the same, therefore they are equivalent expressions* \n" << endl;
    cout << endl << "********************************************************************************" << endl;


    // Print minterms of original circuit
    cout << "Input values that make both circuit satisfiable:\n";
    cout << "A B C: ";
    for(auto& [elem, t] : minterms)
        cout << elem << ' ';
    cout << endl;
}


bool logic(int& i){
    // The boolean eq of our given circuit
    bool a = (i & 1);
    bool b = (i & 2);
    bool c = (i & 4);

    bool s0, s1, s2, s3, s4, s5, s6;
    s0 = a | b;
    s1 = a & b;
    s2 = !b;
    s3 = b & c;
    s4 = s0 & s1;
    s5 = b & s2;
    s6 = s3 | s4 | s5;
    return s6;
//    return (((a |b) & (a & b)) | (b & (!b)) | (b & c));
}

string to_str(int& n){
    string val = "";
    for(int i{2}; i >= 0; i--){
        val += to_string(n >> i & 1);
    }
    return val;
}

vector<string> simplify(vector<pair<string, string>>& minterms){
    vector<vector<pair<string,string>>> curr_pairs(4);
    vector<vector<pair<string,string>>> next_pairs(4);
    bool change{true};

    // construct current pairs
    for(int i{};i < minterms.size(); i++){
        int cnt = 0;
        for(auto& c : minterms[i].first) {
            cnt += (c == '1');
        }
        next_pairs[cnt].push_back(minterms[i]);
    }

    while(change){
        curr_pairs = next_pairs;
        next_pairs.clear(); next_pairs.resize(4);
        change = false;

        for(int i{}; i < 3; i++){
            for(auto [elem, t1] : curr_pairs[i]){
                bool prime{true};
                for(auto& [cmp, t2] : curr_pairs[i + 1]) {
                    string tmp = elem;
                    int cnt = 0, idx = -1;

                    for(int k{0}; k < 3; k++)
                        if(cmp[k] != elem[k])
                            cnt++, idx = k;

                    if(cnt == 1){
                        tmp[idx] = '-';
                        if(tmp != elem){
                            next_pairs[i].push_back({tmp, t1 + t2});
                            change = true;
                            prime = false;
                        }
                    }
                }
                if(prime && curr_pairs[i + 1].size())
                    next_pairs[i].push_back({elem, t1});
            }
        }
    }

    map<int, bool> mp;
    vector<string> simple;
    simp_minterms.clear();
    // prime implicant table
    for(auto group : curr_pairs){
        for(auto [elem, t] : group){
            bool change = false;
            for(auto c : t)
                if(mp.count(c) == 0)
                    mp[c] = true, change = true;
            if(change){
                string s = "";
                for(int i{2};i >= 0; i--){
                    if(elem[i] == '-')
                        continue;
                    s += ('C' - i);
                    if(elem[i] == '0')
                        s += "'";
                }
                if(s != ""){
                    simple.push_back(s);
                    simp_minterms.push_back(elem);
                }

            }
        }
    }

    return simple;
}
