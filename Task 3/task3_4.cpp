#include <iostream>
#include <string>
#include <vector>
using namespace std;
//Encrypts a message using a simple cipher algorithm using the Italian alphebet.
//using a function(x) = ax + b 
//Affine cipher Keys : a = 5 , b = 5
string chipered_message(string message , int a , int b)
{
    int alphabet_size = 22;
    vector<int> x;
    vector<int> func_result;
    vector<int> modulo_result;
    string encrypted_message = "";
    //transering the message to uppercase
    for (int c{}; c < message.size(); c++)
    {
        message[c] = toupper(message[c]);
    }
    char italian_alphabet[22] = { ' ','A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'Z'};
    //encoding the message
    for (int i{}; i < message.length(); i++)
    {
        for (int j{}; j < alphabet_size ; j++)
        {
            if (message[i] == italian_alphabet[j])
                x.push_back(j);
        }
    }
    func_result.resize(x.size());
    modulo_result.resize(x.size());
    //Calculating the function and modulos for x vector 
    for (int k{}; k < x.size(); k++)
    {
        func_result[k] = a * x[k] + b;
        modulo_result[k] = func_result[k] % alphabet_size;
    }
    //Decoding the modulo_result vector into italian charcters 
    for (int u{}; u < message.length(); u++)
    {
        encrypted_message += italian_alphabet[modulo_result[u]];
    }
    return encrypted_message;
}
/******** Example Usage **********/
//argumentes bounded by '*' are the user input in cmd :
/*
------------------------------------------------------------------------------------
Encrypting into the Italian language
Please enter the message to be ciphered : *STUDIA BENE PER LA FINALE*
Please enter the Affine cipher keys:* 5 5 *
-The return value
The ciphered message : BGNCFLEQHZHEIHUEMLEOFZLMH
*/
int main()
{
    string message;
    string submessage;
    int a, b;
    string modified_message;
    cout << "Encrypting into the Italian language" << endl;
    cout << "Please enter the message to be ciphered : ";
    getline(cin, message);
    cout << endl << "Please enter the Affine cipher keys: ";
    cin >> a >> b ;
    modified_message = chipered_message(message , a , b);
    cout << endl<< "The ciphered message : " << modified_message;
}