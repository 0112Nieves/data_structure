#include <iostream>
#include <stack>
#include <cctype>
#include <string>
#include <queue>
#include <map>
#include <limits>
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

void postOrder(Node* root, string& expression) {
    if (root == nullptr) return;
    
    postOrder(root->left, expression);
    postOrder(root->right, expression);
    cout << root->data;
    expression += root->data;
}


void preOrder(Node* root){
    if(root == nullptr) return;
    cout << root->data;
    preOrder(root->left);
    preOrder(root->right);
}

int evaluate(const string& expression, map<char, int>& num) {
    stack<int> s;
    
    for (char c : expression) {
        if (num.find(c) != num.end()) s.push(num[c]);
        else {
            int right = s.top(); s.pop();
            int left = s.top(); s.pop();
            switch (c) {
                case '+': s.push(left + right); break;
                case '-': s.push(left - right); break;
                case '*': s.push(left * right); break;
                case '/': s.push(left / right); break;
                default: break;
            }
        }
    }

    return s.top();
}


int main()
{
    printf("Please enter an infix expression and press enter:");
    while (1) {
        string str = "";
        string expression = "";
        priority_queue<char, vector<char>, greater<char>> alpha;
        map<char, int> num;
        bool ok = true;

        if (!getline(cin, str) || str.empty()) {
            printf("Error reading input or empty input.\n");
            continue;
        }
        if (str[0] == 27) break; // esc
        int cnt = 0;
        for (int i = 0; i < str.length(); i++) {
            if (isspace(str[i])) {
                printf("Input shouldn't have any space\n");
                ok = false;
                break;
            }
            if (isalpha(str[i])) {
                cnt++;
                alpha.push(str[i]);
            }
        }
        if((cnt < 5 || cnt > 20) && ok){
            cout << "there should be 5~20 operand in operational expression\n";
            ok = false;
        }
        if (ok) {
            int temp;
            for (int i = 0; i < cnt; i++) {
                char c = alpha.top();
                alpha.pop();
                cout << c << " = ?";
                cin >> temp;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                num[c] = temp;
            }
            Node* root = constructTree(str);
            cout << "The level-order of the expression tree:" << endl;
            levelOrder(root);
            cout << "The postfix expression: ";
            postOrder(root, expression);
            cout << endl;
            cout << "The prefix expression: ";
            preOrder(root);
            cout << endl;
            char eval;
            cin >> eval;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (eval == '=') {
                cout << evaluate(expression, num) << endl;
            }
        }
        printf("Please enter an infix expression and press enter:");
    }
    return 0;
}