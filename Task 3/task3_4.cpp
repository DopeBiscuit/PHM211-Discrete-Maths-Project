#include <iostream>
#include <string>
#include <vector>
using namespace std;
//Encrypts a message using a simple cipher algorithm using the Italian alphebet.
//using a function(x) = ax + b 
//Affine cipher Keys : a = 5 , b = 5
string chipered_message(string message)
{
    int a, b;
    int alphabet_size = 21;
    vector<int> x;
    vector<int> func_result;
    vector<int> modulo_result;
    string encrypted_message = "";
    a = 5;
    b = 5;
    for (int a{}; a < message.size(); a++)
    {
        message[a] = toupper(message[a]);
    }
    char italian_alphabet[21] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'Z' };
    for (int i{}; i < message.length(); i++)
    {
        for (int j{}; j < 21; j++)
        {
            if (message[i] == italian_alphabet[j])
                x.push_back(j);
        }
    }
    func_result.resize(x.size());
    modulo_result.resize(x.size());
    for (int k{}; k < x.size(); k++)
    {
        func_result[k] = a * x[k] + b;
        modulo_result[k] = func_result[k] % alphabet_size;
    }
    for (int u{}; u < message.length(); u++)
    {
        encrypted_message += italian_alphabet[modulo_result[u]];
    }
    return encrypted_message;
}
int main()
{
    string message;
    string submessage;
    int index = 0;
    string modified_message;
    cout << "Encrypting into the Italian language" << endl;
    cout << "Please enter the message to be ciphered :";
    getline(cin, message);
    for (int i{}; i < message.size(); i++) {
        if ((message[i] == ' ')|| (i == message.size() - 1)) {
            submessage = message.substr(index, i - index);
            index = i + 1;
            modified_message += chipered_message(submessage) + ' ';
        }
    }
    cout << endl << modified_message;
}