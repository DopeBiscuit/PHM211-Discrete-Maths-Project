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
    for (char token : postfix) { // Iterate over tokens in the postfix expression
        if (isalpha(token)) { // variables (operands)
            stack.push(values.at(token));
        }
        else if (token == '1' || token == '0') { // constants i.e. 1 or 0 (operands)
            stack.push(token == '1');
        }
        else if (token == '!') { // NOT
            if (stack.empty()) {
                cerr << "Error: Not enough operands for '!'\n";
                return false;
            }
            bool a = stack.top(); stack.pop();
            stack.push(NOT(a));
        }
        else if (token == '&') { // AND
            if (stack.size() < 2) {
                cerr << "Error: Not enough operands for '&'\n";
                return false;
            }
            bool b = stack.top(); stack.pop();
            bool a = stack.top(); stack.pop();
            stack.push(AND(a, b));
        }
        else if (token == '|') { // OR
            if (stack.size() < 2) {
                cerr << "Error: Not enough operands for '|'\n";
                return false;
            }
            bool b = stack.top(); stack.pop();
            bool a = stack.top(); stack.pop();
            stack.push(OR(a, b));
        }
        else if (token == '>') { // Implication
            if (stack.size() < 2) {
                cerr << "Error: Not enough operands for '>'\n";
                return false;
            }
            bool b = stack.top(); stack.pop();
            bool a = stack.top(); stack.pop();
            stack.push(IMPLIES(a, b));
        }
        else {
            cerr << "Error: Unknown token '" << token << "'\n";
            return false;
        }
    }
    if (stack.size() != 1) { // There should be exactly one value in the stack (the final result) after evaluating the expression
        cerr << "Error: Malformed postfix expression. Stack size: " << stack.size() << "\n";
        return false;
    }
    return stack.top();
}


// Function to convert infix to postfix using Shunting Yard Algorithm 
// (stacking an expression in postfix form by considering precedence of operators)
string infixToPostfix(const string& infix) {
    string postfix;
    stack<char> operators;
    map<char, int> precedence = { {'!', 3}, {'&', 2}, {'|', 1}, {'>', 0}, {'<', 0} };

    for (size_t i = 0; i < infix.size(); ++i) { 
        if (infix[i] == ' ') continue; // Ignore spaces

        if (isalpha(infix[i]) || infix[i] == '1' || infix[i] == '0') {
            // Operand: directly append to postfix
            postfix += infix[i];
        }
        else if (infix[i] == '(') {
            // Left parenthesis
            operators.push(infix[i]);
        }
        else if (infix[i] == ')') {
            // Right parenthesis: pop until '('
            while (!operators.empty() && operators.top() != '(') {
                postfix += operators.top();
                operators.pop();
            }
            if (!operators.empty()) operators.pop(); // Remove '('
        }
        else if (infix[i] == '-' && i + 1 < infix.size() && infix[i + 1] == '>') {
            // Handle '->' as a single implication operator
            while (!operators.empty() && precedence['>'] <= precedence[operators.top()]) {
                postfix += operators.top();
                operators.pop();
            }
            operators.push('>');
            ++i; // Skip the '>' part of '->'
        }
        else { // Other operators: !, &, |
            while (!operators.empty() && precedence[infix[i]] <= precedence[operators.top()]) {
                postfix += operators.top();
                operators.pop();
            }
            operators.push(infix[i]);
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
    bool satisfiable = false, valid = true; // Argument is valid by default, and falsifiable only when a counterexample is found

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
            if (!conclusionResult) valid = false; // Counterexample found: If premises are true and conclusion is false, argument is invalid (falsifiable)
        }
    }

    // Results
    cout << "\nAnalysis Results:\n";
    if (!satisfiable) {
        cout << "The set of statements is NOT SATISFIABLE.\n";
        cout << "The argument is INVALID.\n"; // If premises are not satisfiable, argument cannot be valid
    } else {
        cout << "The set of statements is SATISFIABLE.\n";
        cout << (valid ? "The argument is VALID.\n" : "The argument is FALSIFIABLE.\n");
    }

    std::cout << "Press Enter to exit...";
    std::cin.get(); // Waits for user input
    return 0;
}

/********************* Example Usage **********************
(arguments bounded by '*' are user inputs):
-----------------------------------------------------------
Enter the number of premises: * 2 * 
Enter premise 1: * (k | m) -> !a *
Enter premise 2: * a | m *
Enter the conclusion: * a | !k *

Truth Table:
a       k       m       P1      P2      C
0       0       0       1       0       1
0       0       1       1       1       1
0       1       0       1       0       0
0       1       1       1       1       0
1       0       0       1       1       1
1       0       1       0       1       1
1       1       0       0       1       1
1       1       1       0       1       1

Analysis Results:
The set of statements is SATISFIABLE.
The argument is FALSIFIABLE.

-----------------------------------------------------------
Enter the number of premises: * 2 *
Enter premise 1: * p -> q *
Enter premise 2: * p *
Enter the conclusion: * q *

Truth Table:
p       q       P1      P2      C
0       0       1       0       0
0       1       1       0       1
1       0       0       1       0
1       1       1       1       1

Analysis Results:
The set of statements is SATISFIABLE.
The argument is VALID.

-----------------------------------------------------------
**********************************************************/