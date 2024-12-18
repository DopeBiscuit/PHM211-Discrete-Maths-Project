#include <iostream>
#include <string>
#include<unordered_map>
using namespace std;

#define TO_BE_DECIPHERED "WEKWFCETNVDXZMNJDRV"  //given string to be deciphered
#define KEY_A 5
#define KEY_B 5

//Construct the alphabet map of letter
unordered_map<char, int> alphabetMap = {
    {'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4},
    {'F', 5}, {'G', 6}, {'H', 7}, {'I', 8}, {'J', 9},
    {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13}, {'O', 14},
    {'P', 15}, {'Q', 16}, {'R', 17}, {'S', 18}, {'T', 19},
    {'U', 20}, {'V', 21}, {'W', 22}, {'X', 23}, {'Y', 24},
    {'Z', 25}
    };
unordered_map<int, char> inverseAlphabetMap = {
    {0, 'A'}, {1, 'B'}, {2, 'C'}, {3, 'D'}, {4, 'E'},
    {5, 'F'}, {6, 'G'}, {7, 'H'}, {8, 'I'}, {9, 'J'},
    {10, 'K'}, {11, 'L'}, {12, 'M'}, {13, 'N'}, {14, 'O'},
    {15, 'P'}, {16, 'Q'}, {17, 'R'}, {18, 'S'}, {19, 'T'},
    {20, 'U'}, {21, 'V'}, {22, 'W'}, {23, 'X'}, {24, 'Y'},
    {25, 'Z'}
};
int modular_inverse(int a ) {  //function to get modular inverse of a as (a x a_inverse) and 1 are conguruent modulo 26
    int a_inverse=0 ;
    for (int i = 1; i < 26; i++) {
        if ((a*i) % 26==1) { 
            a_inverse=i; 
            break;
        }
    }
    return a_inverse;
}
string english_decipher(string ciphered,int key_a, int key_b){
    
    for (int i=0;i<ciphered.size();i++) {           //handle input of lower case letters 
        if (ciphered[i] >= 'a' && ciphered[i] <= 'z') 
            ciphered[i] = ciphered[i] - 'a' + 'A';
    }
    
    string deciphered="";

    int a_inverse=modular_inverse(key_a);   //get modular inverse of key_a

    for(int iterator=0;iterator<ciphered.size();iterator++){
        
        int ciphered_index=alphabetMap[ciphered[iterator]]; //ciphered character

        int deciphered_index=(a_inverse*(ciphered_index-key_b));   //appling deciphering function

        deciphered_index=((deciphered_index % 26) + 26) % 26;   //to ensure positive result of modulus operation

        deciphered+=inverseAlphabetMap[deciphered_index]; 
    
    }
    
    return deciphered;
    
    }
int main(){
    
    string to_be_deciphered=TO_BE_DECIPHERED;

    string deciphered=english_decipher(to_be_deciphered,KEY_A,KEY_B);
    
    cout<<deciphered;

    return 0;
}