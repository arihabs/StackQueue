/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* 
Name: Ariel Habshush 
   
Project Name: Data Structures and Algorithms I: Programming Assignment 1
   
Project Description:
The objective of this program is to manipulate stacks and queues. 
Specifically, the program asks the user for the name of an input text file and an output text file. 
The input file will contain a list of commands, one per line. 
Each command will direct the program to create a stack or a queue, to push a value onto a stack or a queue, or to pop a value from a  stack or a queue. 
After each command is read, the program outputs a string containing the text of the input command as well as any errors that might have occured.
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
using namespace std;

 // SimpleList -- Abstract base class that provides simple linked list functionality.
 template <typename Type>
 class SimpleList
 {
 private:
	 string strListName;
	 struct Node
	 {
		 Type element;
		 Node *ptrNextNode;
		 Node(const Type &theElement, Node *n=NULL) // When a Node is initialized with no pointer to the next node, than that node becomes the final node, since it points to NULL.
			 : element(theElement), ptrNextNode(n) {}
	 };
 public:
 	 SimpleList(const string &ListName)
		 : strListName(ListName), ptrStart(NULL), ptrEnd(NULL) {}
	 const string& GetListName() const {return strListName;}
	 
	 // bool isEmpty() -- Return true if list is empty, otherwise false. A list is empty if the address of the first node is NULL.
	 bool isEmpty() {return (ptrStart==NULL);}

	 // Virtual push and pop member functions. These functions are defined individualy in the Stack and Queue classes.
	 virtual Type pop() = 0;
	 virtual void push(const Type &) = 0;

 protected:
 	 Node *ptrStart; // Address of first node in linked list.
	 Node *ptrEnd;   // Address of last node in linked list. 
	 
	 void InsertStart(const Type &x);   // Adds x to the begining of the list.
	 void InsertEnd(const Type &x);    //  Adds x to the end of the list. 
	 void RemoveStart(); 			  //   Removes node from the start of the list.
 };
 
 // void InsertStart -- Adds x to the begining of the list. If the list is empty, make this new node both the Start and End node of the list. Otherwise, have the added Node point to the original Front node.
 template <typename Type>
 void SimpleList<Type>::InsertStart(const Type &x)
 {
	if(isEmpty()) 
	{
		ptrStart = new Node(x);
		ptrEnd = ptrStart;
	}
	else
	{
	      ptrStart = new Node(x,ptrStart);
	}
 }
 
 // void InsertEnd -- Adds x to the end of the list. If the list is empty, make this new node both the Start and End node of the list. Otherwise, have the origninal Last node point to the newly added Node.
 template <typename Type>
 void SimpleList<Type>::InsertEnd(const Type &x)
 {
	if(isEmpty()) 
	{
		ptrStart = new Node(x);
		ptrEnd = ptrStart;
	}
	else
	{
		ptrEnd->ptrNextNode = new Node(x);
		ptrEnd = ptrEnd->ptrNextNode;
	}
 }

 // void RemoveStart -- Removes node from the start of the list. The first node of the list is removed by making the address of the Start node equal to the address pointed to by the original front node. 
 // 					   Because Nodes are dynamically allocated, they should be deleted when they are not in use. Therefore, a temporary pointer is created to point at the first node so then later it could be deleted.
 template <typename Type>
 void SimpleList<Type>::RemoveStart()
 {
	 Node *ptrStartTemp = ptrStart;
	 ptrStart = ptrStart->ptrNextNode;
	 delete ptrStartTemp;
 }
 
 // Stack -- Inherited class that provides stack functionality.
 // For the Stack class, data is pushed into nodes at the start of the list and data is popped off from the start of the list.
 template <typename Type>
 class Stack: public SimpleList<Type>
 {
 private:
	 SimpleList<Type>::InsertEnd;  // For Stack, we do not want to give access to adding to the end of the list, so we make this function private.
 public:
	 Stack(const string &ListName) 
	 : SimpleList<Type>::SimpleList(ListName) {} 
	 virtual Type pop();
	 virtual void push(const Type &x){this->InsertStart(x);}
 };
 
 template <typename Type>
 Type Stack<Type>::pop()
 {
	 Type topOfStack = SimpleList<Type>::ptrStart->element; // The first item to leave the Stack is pointed by the pointer that points to the Start node.
	 SimpleList<Type>::RemoveStart();
	 return topOfStack;
 }

 // Queue -- Inherited class that provides queue functionality.
 // For the Queue class, data is pushed into nodes at the end of the list and data is popped off from the start of the list.
 template <typename Type>
 class Queue: public SimpleList<Type>
 {
 private:
	 SimpleList<Type>::InsertStart; // For Queue, we do not want to give access to adding to the start of the list, so we make this function private.
 public:
	Queue(const string &ListName)
	: SimpleList<Type>::SimpleList(ListName) {}

	virtual Type pop();
	virtual void push(const Type &x) { this->InsertEnd(x);}
 };
 
 template <typename Type>
 Type Queue<Type>::pop()
 {
	 Type endOfQueue = SimpleList<Type>::ptrStart->element; // The first item to leave the queue is pointed by the pointer that points to the Start node.
	 SimpleList<Type>::RemoveStart();
	 return endOfQueue; 
 }

 // void ParseInputCommand -- Parse the input command into individual words. An input command contains either 2 or 3 words.
 void ParseInputCommand(const string &strInCmd, string &strCmdWrd1, string &strCmdWrd2, string &strCmdWrd3)
 {
	 std::stringstream ss(strInCmd);
	 ss >> strCmdWrd1 >> strCmdWrd2 >> strCmdWrd3;
 }

 // void CreateListObject -- Create a stack or queue object with a specified name and store a pointer to this object in a large list that contains pointers to all stacks and queues that were created.
 template <typename Type>
 void CreateListObject(const string &strObjClass, const string &strObjName, list<SimpleList<Type> *> &LargeList)
 {
	 SimpleList<Type> *ptrSL;
	 if(strObjClass.compare("stack") == 0)
	 {
	 	 ptrSL = new Stack<Type>(strObjName);
	 }
	 else if(strObjClass.compare("queue") == 0)
	 {
	 	 ptrSL = new Queue<Type>(strObjName);
	 }

	 LargeList.push_front(ptrSL);
 }

 // bool ObjectNameExists -- Check to see if a stack or queue with a specific name has already been created. 
 // 							By passing an iterator by reference, the value of the iterator at which the name exits is returned. 
 //							If the name does not exist, the value of the iterator is set to List.end().
 template <typename Type>
 bool ObjectNameExists(const string &strObjName, list<SimpleList<Type> *> &LargeList, typename list<SimpleList<Type> *>::iterator &iter)
 {
	 bool bNameExists = false;
	 for(iter = LargeList.begin(); iter != LargeList.end(); iter++)
	 {
		 if(!strObjName.compare((*iter)->GetListName()))
		 {
			 bNameExists = true; 
			 break;
		 }
	 }
	 if(!bNameExists)
	 {iter = LargeList.end();}
	 return bNameExists;
 }

 // void ProcessCreateCmd -- Process the "create" input command. Creates a stack or queue with a specified name. If the stack or queue already exists, it outputs an error to the output file. 
 template <typename Type>
 void ProcessCreateCmd(const string &strObjClass, const string &strObjName, list<SimpleList<Type> *> &LargeList, ofstream &Out)
 {
	 if(LargeList.empty())
	 {
		 CreateListObject(strObjClass, strObjName, LargeList);
	 }
	 else
	 {
		 typename list<SimpleList<Type> *>::iterator itr = LargeList.begin();
		 bool bNameExists = ObjectNameExists(strObjName, LargeList, itr);
		 if(bNameExists)
		 {Out << "ERROR: This name already exists!" << endl;}
		 else
		 {CreateListObject(strObjClass, strObjName, LargeList);}
	 }
 }

 // void ProcessPushCmd -- Processes the "push" command from the input file. Performs the push command on a specified list as well as checks for errors such as non-existing list name.
 template <typename Type>
 void ProcessPushCmd(const string &strObjName, const string &strPushValue, list<SimpleList<Type> *> &LargeList, ofstream &Out)
 {
	 typename list<SimpleList<Type> *>::iterator itr;
	 if(!ObjectNameExists(strObjName, LargeList, itr))
	 {
 		 Out << "ERROR: This name does not exist!" << endl;
	 }
	 else
	 {
		 // Before permorming a push, convert the push value which is currently stored as a string into the correct data type (integer, double, string).
		 Type pushValue;
		 stringstream ss(strPushValue);
		 ss >> pushValue;
		 (*itr)->push(pushValue);
	 }
 }

 // void ProcessPopCmd -- Processes the "pop" command from the input file. Performs the pop command on a specified list as well as checks for errors such as non-existing list name and empty list.
 template <typename Type>
 void ProcessPopCmd(const string &strObjName, list<SimpleList<Type> *> &LargeList, ofstream &Out)
 {
	 typename list<SimpleList<Type> *>::iterator itr;
	 if(!ObjectNameExists(strObjName, LargeList, itr))
	 {
		 Out << "ERROR: This name does not exist!" << endl;
	 }
	 else if((*itr)->isEmpty())
	 {
		 Out << "ERROR: This list is empty!" << endl;
	 }
	 else
	 {	
		 Type poppedValue = (*itr)->pop();
		 Out << "Value popped: " << poppedValue << endl;
	 }
 }

 int main()
{
	// Create 3 lists that contain pointers to all stacks and queues depending on data type they store. One list is for integers, one is for doubles, and one is for strings.	
	list<SimpleList<int> *> listSLi; // all integer stacks and queues
	list<SimpleList<double> *> listSLd; // all double stacks and queues
	list<SimpleList<string> *> listSLs; // all string stacks and queues


	// Display text to screen asking the user to type the names of both the input and output file.
	string strFileNameInput;
	cout << "Enter name of input file: ";
	cin >> strFileNameInput;
	cout << endl;

	string strFileNameOutput;
	cout << "Enter name of output file: ";
	cin >> strFileNameOutput;
	cout << endl;

	ifstream inputFile(strFileNameInput.c_str());
	ofstream outputFile(strFileNameOutput.c_str());

	// Open and read from the input file using ifstream. Use a WHILE loop to read through each line of the input file until the end of the file is reached.
	while (!inputFile.eof())
	{
		// Store each input command (one line of the input file) in a string. The string is then parsed into individual words.
		string strInputCommand; 
		getline(inputFile, strInputCommand);

		// Parse the input command into 2 or 3 words (depending on the type of command)
		string strCommandWord1;
		string strCommandWord2;
		string strCommandWord3;
		ParseInputCommand(strInputCommand, strCommandWord1, strCommandWord2, strCommandWord3);
		
		// The second word of the input command is always the list's name. The first character of the name determines the data type (int, double, string).
		string strObjectName = strCommandWord2;
		char charObjectType = strObjectName[0];
		
		// Look at the first command word (create, push, or pop) and perform the command. 
		// Each command has a function associated with it making it simple to perform the command.
		if(!strCommandWord1.compare("create"))
		{
			outputFile <<"PROCESSING COMMAND: "<< strInputCommand << endl;
			string strObjectClass = strCommandWord3;
			if(charObjectType=='i')
			{ProcessCreateCmd(strObjectClass,strObjectName,listSLi,outputFile);}
			else if(charObjectType=='d')
			{ProcessCreateCmd(strObjectClass,strObjectName,listSLd,outputFile);}
			else if(charObjectType=='s')
			{ProcessCreateCmd(strObjectClass,strObjectName,listSLs,outputFile);}
		}
		else if(!strCommandWord1.compare("push"))
		{
			outputFile <<"PROCESSING COMMAND: "<< strInputCommand << endl;
			if(charObjectType=='i')
			{ProcessPushCmd(strObjectName,strCommandWord3,listSLi,outputFile);}
			else if(charObjectType=='d')
			{ProcessPushCmd(strObjectName,strCommandWord3,listSLd,outputFile);}
			else if(charObjectType=='s')
			{ProcessPushCmd(strObjectName,strCommandWord3,listSLs,outputFile);}
		}
		else if(!strCommandWord1.compare("pop"))
		{
			outputFile <<"PROCESSING COMMAND: "<< strInputCommand << endl;
			if(charObjectType=='i')
			{ProcessPopCmd(strObjectName,listSLi,outputFile);}
			else if(charObjectType=='d')
			{ProcessPopCmd(strObjectName,listSLd,outputFile);}
			else if(charObjectType=='s')
			{ProcessPopCmd(strObjectName,listSLs,outputFile);}
		}
	}
	
return 0;

}