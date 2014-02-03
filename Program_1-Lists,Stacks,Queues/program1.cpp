/********************************************************************************/
/*  Elie Weintraub                                                              */
/*  ECE 164 - Programming Project #1 - C++ Overview, Stacks, and Queues         */
/*                                                                              */
/*  This program reads in a structured input file  of commands relating to      */
/*  stacks and queues, creates the necessary stacks and queues and manipulates  */
/*  them, performing standard push and pop operations                           */
/********************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <string>
#include <cassert>
#include <cstdlib>
using namespace std;

// SimpleList is an abstract base class that provides simple linked list
// functionality
template <typename T>
class SimpleList{
private:
	// Node is a private nested class that serves as the building block for
	// the SimpleList class
	class Node{
	public:
		T data;
		Node *next;
		//Node(...) - Node Class constructor(implemented inline)
		Node(const T &d = T(), Node *n = NULL) : data(d), next(n) {}
	};
	Node *head; 
	Node *tail; 
	int  count;  // Stores the number of nodes in the list
	string name;

protected:
	// SimpleList(...) - SimpleList base class constructor (implemented inline)
	SimpleList(const string &listName)
	  : head(NULL), tail(NULL), count(0), name(listName) {}
	// push_front(...) - inserts element at beginning
	void push_front(const T &d);
	//push_back(...) - inserts element at end
	void push_back(const T &d);
	// pop_front() - removes element from beginning and returns its value
	const T pop_front();

public:
	// get_name() - returns name of list (implemented inline)
	string get_name() const {return name;}
	// is_empty() - returns whether or not list contains any elements (implemented inline)
	bool is_empty() const {return count == 0;}
	// push(...) - A pure virtual function to be implemented in derived Stack and Queue classes
	virtual void push(const T &d) = 0;
	// pop() - A pure virtual function to be implemented in derived Stack and Queue classes
	virtual const T pop ()= 0;  	
};

//Stack is a derived class that provides LIFO functionality
template <typename T>
class Stack : public SimpleList<T> {
public:
	// Stack(...) - Stack class constructor (implemented inline)
	Stack(const string &name):SimpleList<T>(name){}
	// push(...) - pushes element onto top of stack
	void push(const T &d) {return this->push_front(d);} 
	// pop() - pops element from top of stack and returns its value
	const T pop() {return this->pop_front();}
};

// Queue is a derived class that provides FIFO functionality
template <typename T>
class Queue : public SimpleList<T>{
 public:
	// Queue(...) - Queue class constructor (implemented inline)
	Queue(const string &name):SimpleList<T>(name){}
	// push(...) - pushes element onto top of queue
	void push(const T &d) {return this->push_back(d);}
	// pop() - pops element from bottom of queue
	const T pop() {return this->pop_front();}
};

// push_front implementation
template <typename T>
void SimpleList<T>::push_front(const T &d){
	Node *new_head = new Node(d, head);
	if (count==0) {	// list empty - update both head and tail pointer
		head = new_head;
		tail = new_head;
	}
	else  // list not empty - only update head pointer
		head = new_head;
	count++;
}

// push_back Implementation
template <typename T>
void SimpleList<T>::push_back(const T &d){
	Node *new_tail = new Node(d, NULL);
	if (count==0) // list empty - update head and tail pointer
		head = new_tail;
	else  // list not empty - only update tail pointer 
		tail->next = new_tail;
	tail = new_tail;
	count++;
}

// pop_front Implementation
template <typename T>
const T SimpleList<T>::pop_front(){
	assert(head != NULL);  // make sure not popping from an empty list
	Node *old_head = head;
	if (count== 1) { // list has only 1 element - update head and tail pointer
		head = NULL;
		tail = NULL;
	} 
	else  // list has more than one element - only update head pointer
		head = head->next;
	const T retval=old_head->data;
	delete old_head;
	count--;
	return retval;
}

// findName(...) - takes a pointer to a list of Simplelist pointers (listSL) as well as a Stack/
// Queue name(listName) and returns a pointer to the Stack/Queue if it exists and NULL otherwise 
template <typename T>
T findName(list<T> listSL, const string &listName){
	for (class list<T>::iterator itr = listSL.begin(); itr !=listSL.end() ; ++itr)
		if(listName==(*itr)->get_name())
			return *itr;
	return NULL;
}

// The main routine of the program, this function is responsible for parsing the input file
// and processing all the commands
void parse_and_process(ifstream &input, ofstream &output){
	list<SimpleList<int>*> listSLi;    // All int Stacks and Queues are stored in this list
	list<SimpleList<double>*> listSLd; // All double Stacks and Queues are stored in this list
	list<SimpleList<string>*> listSLs; // All string Stacks and Queues are stored in this list

	SimpleList<int> *pSLi;     // Pointer used to access all int Stacks and Queues
	SimpleList<double> *pSLd;  // Pointer used to access all double Stacks and Queues
	SimpleList<string> *pSLs;  // Pointer used to access all string Stacks and Queues

	string word1,word2; // Variables to store the first two command words
	string s_word3;     // Stores the third word for processing and when it is a string
	int i_word3;        // Stores the third word when it is an int to be pushed
	double d_word3;     // Stores the third word when it is a double to be pushed

	while(input>>word1>>word2){  // Read in first two words of line and check for EOF
		if (word1=="create"){
			input>>s_word3;
			output<<"PROCESSING COMMAND: "<<word1<<" "<<word2<<" "<<s_word3<<endl;
			switch(word2[0]){
				case 'i':
					if(findName(listSLi,word2))   // Stack or Queue already exists
						output<<"ERROR: This name already exists!"<<endl;
					else{   // Stack or Queue doesn't exist - create it and add to list
						(s_word3=="stack") ?pSLi=new Stack<int>(word2): pSLi=new Queue<int>(word2);  
						listSLi.push_front(pSLi);  
					}
					break;
				case 'd':
					if(findName(listSLd,word2))   // Stack or Queue already exists
						output<<"ERROR: This name already exists!"<<endl;
					else{   // Stack or Queue doesn't exist - create it and add to list
						(s_word3=="stack") ? pSLd=new Stack<double>(word2): pSLd=new Queue<double>(word2);	
						listSLd.push_front(pSLd);  
					}
					break;
				case 's':
					if(findName(listSLs,word2))   // Stack or Queue already exists
						output<<"ERROR: This name already exists!"<<endl;
					else{   // Stack or Queue doesn't exist - create it and add to list
						(s_word3=="stack") ? pSLs=new Stack<string>(word2): pSLs=new Queue<string>(word2);  
						listSLs.push_front(pSLs);  
					}
					break;
			}		
		}
		else if(word1=="push"){
			input>>s_word3;
			output<<"PROCESSING COMMAND: "<<word1<<" "<<word2<<" "<<s_word3<<endl;
			switch(word2[0]){
				case 'i':
					if(!(pSLi=findName(listSLi,word2)))  // Stack or Queue doesn't exist
						output<<"ERROR: This name does not exist!"<<endl;
					else {  // Stack or Queue does exist - push value to it	
							stringstream(s_word3)>>i_word3;
							pSLi->push(i_word3);
					}
					break;
				case 'd':
					if((pSLd=findName(listSLd,word2))==NULL)  // Stack or Queue doesn't exist
						output<<"ERROR: This name does not exist!"<<endl;
					else{   // Stack or Queue does exist - push value to it	
							stringstream(s_word3)>>d_word3;
							pSLd->push(d_word3);
					}
					break;
				case 's':
					if(!(pSLs=findName(listSLs,word2)))  // Stack or Queue doesn't exist
						output<<"ERROR: This name does not exist!"<<endl;
					else   // Stack or Queue does exist - push value to it	
							pSLs->push(s_word3);
					break;
			}					
		}	
		else{  // word1=="pop"
			output<<"PROCESSING COMMAND: "<<word1<<" "<<word2<<endl;	
			switch(word2[0]){
				case 'i':
					if(!(pSLi=findName(listSLi,word2)))  // Stack or Queue doesn't exist
						output<<"ERROR: This name does not exist!"<<endl;
					else if(pSLi->is_empty())   // Nothing to pop
						output<<"ERROR: This list is empty!"<<endl;
					else // Pop value
						output<<"Value popped: "<<pSLi->pop()<<endl;
					break;
				case 'd':
					if(!(pSLd=findName(listSLd,word2)))  // Stack or Queue doesn't exist
						output<<"ERROR: This name does not exist!"<<endl;
					else if(pSLd->is_empty())   // Nothing to pop
						output<<"ERROR: This list is empty!"<<endl;
					else // Pop value
						output<<"Value popped: "<<pSLd->pop()<<endl;
					break;
				case 's':
					if(!(pSLs=findName(listSLs,word2)))  // Stack or Queue doesn't exist
						output<<"ERROR: This name does not exist!"<<endl;
					else if(pSLs->is_empty())   // Nothing to pop
						output<<"ERROR: This list is empty!"<<endl;
					else // Pop value
						output<<"Value popped: "<<pSLs->pop()<<endl;
					break;
			}
		}
	}
}

main()
{
	string file_name_input, file_name_output; 

	cout<<"Enter name of input file: ";
	cin>>file_name_input;
	ifstream input(file_name_input.c_str());
	if (!input.is_open()){ // Error opening input file
		cout << "Unable to open input file"<<endl;
		exit(1);
	}
	cout<<"Enter name of output file: ";
	cin>>file_name_output;
	ofstream output(file_name_output.c_str());
	if (!output.is_open()){ // Error opening output file
		cout << "Unable to open output file"<<endl;	
		exit(1);
	}
	parse_and_process(input,output);
	input.close();
	output.close();	
	return 0;
}
