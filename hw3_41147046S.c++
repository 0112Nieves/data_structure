#include <iostream>
#include <stack>
#include <cctype>
#include <string>
#include <queue>
#include <bool>
using namespace std;

struct Node {
    char data;
    Node* left;
    Node* right;

    Node(char val) : data(val), left(nullptr), right(nullptr) {}
};

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

Node* constructTree(const string& infix) {
    stack<Node*> operands;
    stack<char> operators;

    for (char c : infix) {
        if (isspace(c)) continue;
        if (isalnum(c)) {
            operands.push(new Node(c));
        }
        else if (c == '(') {
            operators.push(c);
        }
        else if (c == ')') {
            while (!operators.empty() && operators.top() != '(') {
                char op = operators.top();
                operators.pop();

                Node* right = operands.top(); operands.pop();
                Node* left = operands.top(); operands.pop();

                Node* newNode = new Node(op);
                newNode->left = left;
                newNode->right = right;

                operands.push(newNode);
            }
            operators.pop();
        }
        else if (isOperator(c)) {
            while (!operators.empty() && precedence(operators.top()) >= precedence(c)) {
                char op = operators.top();
                operators.pop();

                Node* right = operands.top(); operands.pop();
                Node* left = operands.top(); operands.pop();

                Node* newNode = new Node(op);
                newNode->left = left;
                newNode->right = right;

                operands.push(newNode);
            }
            operators.push(c);
        }
    }

    while (!operators.empty()) {
        char op = operators.top();
        operators.pop();

        Node* right = operands.top(); operands.pop();
        Node* left = operands.top(); operands.pop();

        Node* newNode = new Node(op);
        newNode->left = left;
        newNode->right = right;

        operands.push(newNode);
    }
    return operands.top();
}

void levelOrder(Node* root) {
    if (!root) return;
    queue<Node*> level;
    level.push(root);

    while (!level.empty()) {
        int levelSize = level.size();
        for (int i = 0; i < levelSize; i++) {
            Node* current = level.front();
            level.pop();
            cout << current->data;
            if (current->left) level.push(current->left);
            if (current->right) level.push(current->right);
        }
        cout << endl;
    }
}

int main()
{
    while(1){
        string str;
        bool ok = true;
        printf("Please enter an infix expression and press enter:");
        if(!getline(cin, str)){
            printf("Error reading input.\n");
            continue;
        }
        int cnt = 0;
        for(int i = 0; i < str.length(); i++){
            if(isspace(str[i])){
                printf("input shoudn't have space\n");
                ok = false;
                break;
            }
            if(isalpha(str[i])) cnt++;
        }
        if(cnt < 5 || cnt > 20) ok = false;
        if(ok){
            Node* root = constructTree(str);
            cout << "The level-order of the expression tree:" << endl;
            levelOrder(root);
        }
    }
    
    return 0;
}