#include <iostream>
#include <string>
using namespace std;

typedef struct dlink* pointer;
typedef struct dlink {
    char c;
    pointer rlink;
    pointer llink;
} dlink;

dlink* create(char c) {
    dlink* temp = new dlink;
    temp->c = c;
    temp->rlink = nullptr;
    temp->llink = nullptr;
    return temp;
}

void set_sentence(dlink*& head, char c) {
    dlink* newNode = create(c);
    if (!head) {
        head = newNode;
        newNode->rlink = head;
        newNode->llink = head;
    }
    else {
        dlink* tail = head;
        while (tail->rlink != head) {
            tail = tail->rlink;
        }
        tail->rlink = newNode;
        newNode->llink = tail;
        newNode->rlink = head;
        head->llink = newNode;
    }
}

void printList(dlink* head) {
    if(!head) return;
    if(head->llink == nullptr){
        cout << head->c << endl;
        return;
    }
    dlink* temp = head;
    do {
        cout << temp->c;
        temp = temp->rlink;
    } while (temp != head);
    cout << endl;
}

void insert(dlink*& head, char c) {
    dlink* inserted = create(c);
    if (!head) return;

    dlink* now = head;
    // insert to head
    if(now->c == '|'){
        inserted->llink = now->llink;
        inserted->rlink = now;
        if (now->llink != nullptr) {
            now->llink->rlink = inserted;
        }
        now->llink = inserted;
        head = inserted;
    }
    // insert to middle
    else{
        while (now != nullptr && now->rlink != head && now->rlink->c != '|') {
            now = now->rlink;
        }
        if (now->rlink != nullptr && now->rlink->c == '|') {
            inserted->rlink = now->rlink;
            inserted->llink = now;
            now->rlink = inserted;
            if (inserted->rlink != nullptr) {
                inserted->rlink->llink = inserted;
            }
        }
    }
}

void backspace(dlink*& head) {
    if (!head) return;
    if(head->c == '|') return;
    dlink* now = head;
    while (now->rlink != head && now->rlink->c != '|') {
        now = now->rlink;
    }
    if(now->llink->c != '|'){
        now->llink->rlink = now->rlink;
        now->rlink->llink = now->llink;
    }
    else{
        head = now->rlink;
        head->llink = nullptr;
    }
    delete now;
}

void deleted(dlink*& head){
    if(head->c == '|') return;
    else{
        dlink* now = head;
        while (now->rlink != head && now->c != '|') {
            now = now->rlink;
        }
        if(now->rlink == head){
            cout << "There are no character can delete.\n";
            return;
        }
        else if(now->rlink->rlink == head){
            cout << "the length is 2\n";
            dlink* temp = now->rlink;
            delete temp;
        }
        else{
            now->rlink->llink = now->llink;
            now->llink->rlink = now->rlink;
            delete now;
        }
    }
}

void move_to_left(dlink*& head) {
    if (head->c == '|') return;
    dlink* now = head;
    while (now->c != '|') now = now->rlink;
    if(now->llink == head && now->rlink == head->rlink->rlink) head = now;

    dlink* leftNode = now->llink;
    dlink* leftLeftNode = leftNode->llink;
    
    leftNode->rlink = now->rlink;
    now->rlink->llink = leftNode;

    now->llink = leftLeftNode;
    leftLeftNode->rlink = now;

    now->rlink = leftNode;
    leftNode->llink = now;
}

void move_to_right(dlink*& head) {
    if(head->c == '|' && head->rlink == nullptr) return;
    dlink* now = head;
    while (now->c != '|') now = now->rlink;
    if(now->rlink == head) return;
    if(now->rlink == head->rlink) head = now->rlink;
    dlink* rightNode = now->rlink;
    dlink* rightRightNode = rightNode->rlink;
    rightNode->llink = now->llink;
    now->llink->rlink = rightNode;

    now->rlink = rightRightNode;
    rightRightNode->llink = now;

    now->llink = rightNode;
    rightNode->rlink = now;
}

int main() {
    dlink* head = nullptr;
    string str;
    cout << "Please enter an initial string consisting of a/A-z/Z and space: ";
    getline(cin, str);
    str.push_back('|');
    for (char ch : str) {
        set_sentence(head, ch);
    }
    cout << "The double linked list is: ";
    printList(head);

    string cmd;
    while (true) {
        cout << "Please choose what you want to do:\n"
                "a/A-z/Z and space: insert a character (please click enter for every single alphabet and space)\n"
                "0: delete\n"
                "1: move to left\n"
                "2: move to right\n"
                "3: redo\n"
                "4: undo\n"
                "9: backspace\n"
                "exit: exit\n";
        getline(cin, cmd);
        if (cmd == "exit") break;
        else if(cmd == "0"){
            deleted(head);
            printList(head);
        }
        else if(cmd == "1"){
            move_to_left(head);
            printList(head);
        }
        else if(cmd == "2"){
            move_to_right(head);
            printList(head);
        }
        else if (cmd == "9"){
            backspace(head);
            printList(head);
        }
        else if (cmd.length() == 1) {
            if(isalpha(cmd[0])){
                insert(head, cmd[0]);
                printList(head);
            }
            else cout << "No such commend\n" << endl;
        }
        else {
            cout << "Invalid command!" << endl;
        }
    }
    return 0;
}
