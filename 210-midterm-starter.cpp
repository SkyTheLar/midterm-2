/*********************************************************

COMSC 210 | Midterm 2 | Skylar Robinson | IDE Used: Eclipse

*********************************************************/

#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
using namespace std;

const int MAX_NM = 99; //max names

class DoublyLinkedList {
private:
    struct Node {
        string data;
        Node* prev;
        Node* next;
        Node(string val, Node* p = nullptr, Node* n = nullptr) {
            data = val; 
            prev = p;
            next = n;
        }
    };

    Node* head;
    Node* tail;

public:
    DoublyLinkedList() { head = nullptr; tail = nullptr; }

    void insert_after(string value, int position) {
        if (position < 0) {
            cout << "Position must be >= 0." << endl;
            return;
        }

        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
            return;
        }

        Node* temp = head;
        for (int i = 0; i < position && temp; ++i)
            temp = temp->next;

        if (!temp) {
            cout << "Position exceeds list size. Node not inserted.\n";
            delete newNode;
            return;
        }

        newNode->next = temp->next;
        newNode->prev = temp;
        if (temp->next)
            temp->next->prev = newNode;
        else
            tail = newNode;
        temp->next = newNode;
    }

    void delete_val(string value) {
        if (!head) return;

        Node* temp = head;
        
        while (temp && temp->data != value)
            temp = temp->next;

        if (!temp) return; 

        if (temp->prev)
            temp->prev->next = temp->next;
        else
            head = temp->next; 

        if (temp->next)
            temp->next->prev = temp->prev;
        else
            tail = temp->prev; 

        delete temp;
    }

    void delete_pos(int pos) {
        if (!head) {
            cout << "List is empty." << endl;
            return;
        }
    
        if (pos == 1) {
            pop_front();
            return;
        }
    
        Node* temp = head;
    
        for (int i = 1; i < pos; i++){
            if (!temp) {
                cout << "Position doesn't exist." << endl;
                return;
            }
            else
                temp = temp->next;
        }
        if (!temp) {
            cout << "Position doesn't exist." << endl;
            return;
        }
    
        if (!temp->next) {
            pop_back();
            return;
        }
    
        Node* tempPrev = temp->prev;
        tempPrev->next = temp->next;
        temp->next->prev = tempPrev;
        delete temp;
    }

    void push_back(string v) {
        Node* newNode = new Node(v);
        if (!tail)
            head = tail = newNode;
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }
    
    void push_front(string v) {
        Node* newNode = new Node(v);
        if (!head)
            head = tail = newNode;
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }
    
    void pop_front() {

        if (!head) {
            cout << "List is empty." << endl;
            return;
        }

        Node * temp = head;

        if (head->next) {
            head = head->next;
            head->prev = nullptr;
        }
        else
            head = tail = nullptr;
        delete temp;
    }

    void pop_back() {
        if (!tail) {
            cout << "List is empty." << endl;
            return;
        }
        Node * temp = tail;

        if (tail->prev) {
            tail = tail->prev;
            tail->next = nullptr;
        }
        else
            head = tail = nullptr;
        delete temp;
    }

    ~DoublyLinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    void print() {
        Node* current = head;
        if (!current) {
            cout << "List is empty.\n" << endl;
            return;
        }
        while (current) {
            cout << "\t" << current->data << endl;
            current = current->next;
        }
        cout << endl;
    }

    void print_reverse() {
        Node* current = tail;
        if (!current) { 
            cout << "List is empty.\n" << endl;
            return;
        }
        while (current) {
            cout << "\t" << current->data << endl;
            current = current->prev;
        }
        cout << endl;
    }

    //getter
    string getData(int pos) {
    	if (!head) {
    		cout << "List is empty." << endl;
    	    return "";
    	}
        Node* temp = head;

        for (int i = 1; i < pos; i++){
            if (!temp) {
                cout << "Position doesn't exist." << endl;
                return "";
            }
            else
                temp = temp->next;
        }
        if (!temp) {
            cout << "Position doesn't exist." << endl;
            return "";
        }
    	return temp->data;
    }

    //length of list
    int length() {
    	if (!head){
    		return 0;
    	}
    	Node* temp = head;
    	int counter = 0;
    	while (temp) {
    		counter++;
    		temp = temp->next;
    	}
    	return counter;
    }
};

string getName(); //returns a random name
int prob(); //returns random number 1-100
void helped(DoublyLinkedList&); //front customer is helped
void newCus(DoublyLinkedList&); //new customer joins line
void endLeft(DoublyLinkedList&); //customer at end of line leaves
void randLeft(DoublyLinkedList&, int); //random customer leaves
void VIP(DoublyLinkedList&); //vip joins the front of the line

int main() {
	srand(time(0));
	//create line
    DoublyLinkedList line;

    cout << "Store opens:\n";

    //populate line
    for (int i = 0; i < 5; i++)
    	newCus(line);

    cout << "Resulting line:\n";
    line.print();

    //loop to time interval 20
    for (int i = 2; i <= 20; i++) {
    	cout << "Time step #" << i << endl;
    	//different probability checks
    	if (prob() <= 60)
    	    newCus(line);
    	//runs probability check for each customer
    	for (int i = 1; i < line.length(); i++) { //loop doesn't include last in line
    		if (prob() <= 10)
    			randLeft(line,i);
    	}
    	//more probability checks
    	if (prob() < 10)
    		VIP(line);
    	if (prob() <= 40 && line.length() != 0)
    		helped(line);
    	if (prob() <= 20 && line.length() != 0)
    		endLeft(line);
    	cout << "Resulting line:\n";
    	line.print();
    }

    return 0;
}

string getName() {
	ifstream in;
	in.open("names.txt");
	if (!in) {
		cout << "File open error.\n";
		return "";
	}
	int num = (rand() % MAX_NM) + 1;
	string name;
	for (int i = 0; i < num; i++)
		getline(in, name);
	in.close();
	return name;
}

int prob() {
	return rand() % 100 + 1;
}

void helped(DoublyLinkedList &d) {
	cout << "\t" << d.getData(1) << " is served.\n";
	d.pop_front();
}

void newCus(DoublyLinkedList &d) {
	string name = getName();
	cout << "\t" << name << " joined the line.\n";
	d.push_back(name);
}

void endLeft(DoublyLinkedList &d) {
	cout << "\t" << d.getData(d.length()) << " (at the rear) left the line.\n";
	d.pop_back();
}

void randLeft(DoublyLinkedList &d, int p) {
	cout << "\t" << d.getData(p) << " left the line.\n";
	d.delete_pos(p);
}

void VIP(DoublyLinkedList &d) {
	string name = getName();
	cout << "\t" << name << " (VIP) joins the front of the line.\n";
	d.push_front(name);
}
