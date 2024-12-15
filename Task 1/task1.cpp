#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <stack>
#include <sstream>
#include <cctype>
#include <cmath>

using namespace std;

// Logical operations
bool NOT(bool a) { return !a; }
bool OR(bool a, bool b) { return a || b; }
bool AND(bool a, bool b) { return a && b; }
bool IMPLIES(bool a, bool b) { return !a || b; }
bool BICONDITIONAL(bool a, bool b) { return a == b; }

// Function to evaluate a postfix logical expression
bool evaluatePostfix(const string& postfix, const map<char, bool>& values) {
    stack<bool> stack;
    for (char token : postfix) {
        if (isalpha(token)) {
            stack.push(values.at(token));
        }
        else if (token == '1' || token == '0') {
            stack.push(token == '1');
        }
        else if (token == '!') {
            bool a = stack.top(); stack.pop();
            stack.push(NOT(a));
        }
        else if (token == '&') {
            bool b = stack.top(); stack.pop();
            bool a = stack.top(); stack.pop();
            stack.push(AND(a, b));
        }
        else if (token == '|') {
            bool b = stack.top(); stack.pop();
            bool a = stack.top(); stack.pop();
            stack.push(OR(a, b));
        }
        else if (token == '>') { // Implication
            bool b = stack.top(); stack.pop();
            bool a = stack.top(); stack.pop();
            stack.push(IMPLIES(a, b));
        }
    }
    return stack.top();
}

// Function to convert infix to postfix using Shunting Yard Algorithm
string infixToPostfix(const string& infix) {
    string postfix;
    stack<char> operators;
    map<char, int> precedence = { {'!', 3}, {'&', 2}, {'|', 1}, {'>', 0}, {'<', 0} };

    for (size_t i = 0; i < infix.size(); ++i) {
        char token = infix[i];

        if (isalpha(token) || token == '1' || token == '0') {
            // Operand: directly append to postfix
            postfix += token;
        }
        else if (token == '(') {
            // Left parenthesis: push onto the stack
            operators.push(token);
        }
        else if (token == ')') {
            // Right parenthesis: pop until left parenthesis is found
            while (!operators.empty() && operators.top() != '(') {
                postfix += operators.top();
                operators.pop();
            }
            if (!operators.empty()) operators.pop(); // Remove '('
        }
        else { // Operator
            while (!operators.empty() && precedence[token] <= precedence[operators.top()]) {
                postfix += operators.top();
                operators.pop();
            }
            operators.push(token);
        }
    }

    // Pop remaining operators
    while (!operators.empty()) {
        postfix += operators.top();
        operators.pop();
    }

    return postfix;
}


int main() {
    int numPremises;
    cout << "Enter the number of premises: ";
    cin >> numPremises;
    cin.ignore();

    vector<string> premises(numPremises);
    string conclusion;
    set<char> variables;

    // Input premises
    for (int i = 0; i < numPremises; ++i) {
        cout << "Enter premise " << (i + 1) << ": ";
        string infix;
        getline(cin, infix);
        premises[i] = infixToPostfix(infix);
        for (char c : infix) if (isalpha(c)) variables.insert(c);
    }

    // Input conclusion
    cout << "Enter the conclusion: ";
    string infixConclusion;
    getline(cin, infixConclusion);
    conclusion = infixToPostfix(infixConclusion);
    for (char c : infixConclusion) if (isalpha(c)) variables.insert(c);

    // Generate truth table combinations
    int numVariables = variables.size();
    vector<char> varList(variables.begin(), variables.end());
    int rows = pow(2, numVariables);
    bool satisfiable = false, valid = false;

    // Display header
    cout << "\nTruth Table:\n";
    for (char var : varList) cout << var << "\t";
    for (int i = 0; i < numPremises; ++i) cout << "P" << (i + 1) << "\t";
    cout << "C\n";

    // Evaluate truth table
    for (int i = 0; i < rows; ++i) {
        map<char, bool> values;
        for (int j = 0; j < numVariables; ++j) {
            values[varList[j]] = (i >> (numVariables - j - 1)) & 1;
            cout << values[varList[j]] << "\t";
        }

        bool allPremisesTrue = true;
        for (const string& premise : premises) {
            bool premiseResult = evaluatePostfix(premise, values);
            cout << premiseResult << "\t";
            allPremisesTrue = allPremisesTrue && premiseResult;
        }

        bool conclusionResult = evaluatePostfix(conclusion, values);
        cout << conclusionResult << "\n";

        if (allPremisesTrue) {
            satisfiable = true;
            if (!conclusionResult) valid = false; // Counterexample
        }
    }

    // Results
    cout << "\nAnalysis Results:\n";
    cout << (satisfiable ? "The set of statements is SATISFIABLE.\n" : "The set of statements is NOT SATISFIABLE.\n");
    cout << (valid ? "The argument is VALID.\n" : "The argument is FALSIFIABLE.\n");

    std::cout << "Press Enter to exit...";
    std::cin.get(); // Waits for user input
    return 0;
}
