//============================================================================
// Name        : lab10_8.cpp
// Author      : Christopher Smith
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;//to use cout and whatnot...
int getIndex(char c);//function to find an index
class Message{
	private:
	public:
	int truthVal;//determines if it is regular or morse
	int stringSize;//needed for translating
	string message;//contains the message
	Message();//default constructor
	Message(string);//parametric constructor
	virtual void print();//print method
};
Message::Message(){
	cout << "Please enter a message\n" << endl;//where the message gets entered
	cin >> message;
	stringSize = message.length();//using the length function of the string class
	truthVal = 0;
}
Message::Message(string message1){
	message = message1;//sets the message to whatever was passed in
	stringSize = message.length();
	truthVal = 0;
}
class morseMessage : public Message{
	private:
	public:
	string* morseMsg;
	morseMessage();
	void translate(string);
	void print();
	void toLED();
};
morseMessage::morseMessage(){
	stringSize = message.length();
	cout << "size of morse string is " << stringSize << endl;
	translate(message);//translates the message... sets the elements in the array in the translate method...
	truthVal = 1;
}
class stackMessage{
	public:
	int numStackElements;;
	Message *stackPtr[10];//an array of ten pointers each pointing to a message...
	Message *startPtr;//keeps track of the first pointer
	stackMessage();//constructor
	~stackMessage();//destructor
	void pushMsg(Message);
	void pushMsg(morseMessage);
	void popMsg();
	void printStack(stackMessage);
	int i;
};
stackMessage::~stackMessage(){//frees each element in the array of pointers
	int j = 0;
	while(j < i){
		delete stackPtr[j];
		j++;
	}
}
void stackMessage::printStack(stackMessage msg){
	int j = 0;
	cout << "\n\t\tSTACK\n" << endl;
	while(j < i)
	{
		cout << "Eleement " << j+1 << ":" << endl;
		stackPtr[j]->print();//use of dynamic binding.. calls the print method of the message OR morse message class and prints out the info
		j++;
}
	cout << "\n\n" << endl;
}
stackMessage::stackMessage(){
	i = 0;//no initial elements..
	startPtr = NULL;
}
void stackMessage::pushMsg(Message message){//two different pushMsg methods, depends on if it is a regular message or a morse message
	if(startPtr == NULL);
	else{
		startPtr = &message;
	}
	if(numStackElements >= 10) return;
	else{
		stackPtr[i] = new Message(message);//allocates memory for a normal message type, sets the stack to point at this element
		i++;
	}
}
void stackMessage::pushMsg(morseMessage message){
	if(startPtr == NULL);
	else{
		startPtr = &message;
	}
	if(numStackElements >= 10) return;
	else{
		stackPtr[i] = new morseMessage(message);//allocates memory for a morse message type, sets the stack pointer
		i++;
	}
}
void stackMessage::popMsg(){
	if(i == 0){//error check to make sure we dont free memory that hasnt been allocated yet
	cout << "Theres nothing to pop!" << endl;
	return;
	}
	delete stackPtr[i-1];
	i--;
}
void Message::print(void)//print method that will be called when a a normal message needs to be printed
{
	cout << "\tMessage: " << message << endl;
}
void morseMessage::print()//this print method will be called when a morse message needs to be printed
{
	int k =0;
	string* startPtr = morseMsg;
	cout << "\tMessage:" << message << endl;
	cout << "\tMorse Message:" << endl;
	while (k < stringSize){
		cout << *morseMsg << "\t";
		morseMsg++;
		k++;
	}
	morseMsg = startPtr;
	cout << endl;
}
void morseMessage::toLED()
{
		int fd;		// for the file descriptor of the special file we need to open.
		unsigned long *BasePtr;		// base pointer, for the beginning of the memory page (mmap)
		unsigned long *PBDR, *PBDDR;	// pointers for port B DR/DDR

	    fd = open("/dev/mem", O_RDWR|O_SYNC);	// open the special file /dem/mem
		if(fd == -1){
			printf("\n error1\n");
		    return;  // failed open
		}

		// We need to map Address 0x80840000 (beginning of the page)
		BasePtr = (unsigned long*)mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x80840000);
	    if(BasePtr == MAP_FAILED){
	    	printf("\n Unable to map memory space \n");
	    	return;
	    }  // failed mmap

		// To access other registers in the page, we need to offset the base pointer to reach the
		// corresponding addresses. Those can be found in the board's manual.
		PBDR = BasePtr + 1;		// Address of port B DR is 0x80840004
	    PBDDR = BasePtr + 5;	// Address of port B DDR is 0x80840014

	    *PBDDR &= 0x00;
	    *PBDDR |= 0xE0;		// configures port B7 as output (Green LED)
		*PBDDR &= 0xFFFFFFFE;	// configures port B0 as input (first push button). You could use: &= ~0x01

		// The program will turn on the green LED, sleep for a while, then off, sleep, then on again, then off.
		// You could use loops, if you wanted/needed.
	    //*PBDR |= 0x80;	// ON: write a 1 to port B0. Mask all other bits.
		//sleep(1);	// How can you sleep for less than a second?
	    //*PBDR &= ~0x80;	// OFF: write a 0 to port B0. Mask all other bits.
	    //sleep(1);
	    //*PBDR |= 0x80;
		//sleep(2);
	    //*PBDR &= ~0x80;

		// If you wanted to read the status of Port B0, how could you do it?

	   	// close the special file
		cout << "PLEASE PRINT THIS MESSAGE" << endl;
		string* startPtr = morseMsg;
		int z = 0;
		cout << "TESTING 123" << endl;
		*PBDR &= 0x00;
		sleep(3);

		while(z < stringSize)
		{
				string temp = *morseMsg;
				cout << "TEST:" << temp << endl;
				int y = temp.length();

				int i = 0;

				while(i < y){
					char c = temp[i];
					if(c == '.'){
						cout << "RED" << endl;
						*PBDR |= 0x20;
						usleep(500000);
						*PBDR &= 0x00;
						usleep(500000);
					}
					else if(c == '-'){
						cout << "YELLOW" << endl;
						*PBDR |= 0x40;
						usleep(500000);
						*PBDR &= 0x00;
						usleep(500000);
					}
					i++;
					//*PBDR &= 0x1F;
				}

				cout << y << endl;
				z++;
				morseMsg++;
		}
		cout << "GREEN" << endl;
		*PBDR |= 0x80;
		usleep(500000);
		*PBDR &= 0x00;
		usleep(500000);
		morseMsg = startPtr;
		close(fd);
}
int main(void)
{

	/*Message msg1;//these tests go fine, i can translate effectively and print it out
	msg1.print();
	Message msg2("test string for Method two");
	msg2.print();
	morseMessage msg3;
	msg3.print();
	cout << "\nOKAY; now we built three different types of classes time to stack shit up" << endl;
	int userInput = 1000;
	int regORmorse;
	int i = 0;
	stackMessage msg4;
	while (userInput != 0)//where the user menu begins
	{
		cout << "What do you want to do?\n0: Exit the program\n1:Push a message onto the stack\n2:POP a message off of the stack\n3: DISPLAY the current stack" << endl;
		cin >> userInput;
		while(userInput < 0 || userInput > 3){
			cout <<"Try that again, user" << endl;
			cin >> userInput;
		}
		switch(userInput)
		{
		case 0:
			return 1;
			break;
		case 1:
			cout << "Regular (1) message or morse message (2)?" << endl;
			cin >> regORmorse;
			while(regORmorse != 1 && regORmorse != 2){
				cout << "ONE OR A TWO"<< endl;
				cin >> regORmorse;
			}
			if(regORmorse == 1)
			{
				Message tempMessage;//trying to make a new message here, and then push it onto the stack, by making the stackpointer point to each of these messages created
				msg4.pushMsg(tempMessage);
			}
			else
			{
				morseMessage tempMessage;//in case the user wanted to use a morse message
				msg4.pushMsg(tempMessage);
			}
			break;
		case 2:
			msg4.popMsg();
			break;
		case 3:
			msg4.printStack(msg4);
			break;
		default:
			break;
		}
	}
		*/
	morseMessage lab10msg;
	lab10msg.toLED();
	return 1;
}
void morseMessage::translate(string message1)
{
	morseMsg = new string[stringSize];
	char c;
	int index;
	int i;
	cout << "translating..\n" << endl;
	string* startPtr = morseMsg;
	string letters = "abcdefghijklmnopqrstuvwxyz";
	string morseLetters[26] = {
			".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",
			".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--.."};
	for(i=0;i<stringSize;i++)
	{
		c = message[i];
		if(isupper(c)==1) c = tolower(c);//converts to lowercase if it is uppercase
		index = getIndex(c);
		*morseMsg = morseLetters[index];//sets the actual morse string
		morseMsg++;//bumps the string pointer
	}
	morseMsg = startPtr;
}
int getIndex(char c)//function to find the index of the letter
{
	int index;
	index = c -97;//subtract 97 because lowercase letters begin at 97 on ASCII table..
	return index;
}
