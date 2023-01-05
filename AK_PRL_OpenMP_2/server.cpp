// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <omp.h>
#include <iostream>
#include <chrono>
#include <fstream>
#include "lines.h"
#include <thread>
#include <pthread.h>
#include <queue>


#define PORT 8080

using namespace std;

// Max number of states in the matching machine.
// Should be equal to the sum of the length of all keywords.
const int MAXS = 500;

// Maximum number of characters in input alphabet
const int MAXC = 26;

// OUTPUT FUNCTION IS IMPLEMENTED USING out[]
// Bit i in this mask is one if the word with index i
// appears when the machine enters this state.
int out[MAXS];

// FAILURE FUNCTION IS IMPLEMENTED USING f[]
int f[MAXS];

// GOTO FUNCTION (OR TRIE) IS IMPLEMENTED USING g[][]
int g[MAXS][MAXC];

// Builds the string matching machine.
// arr - array of words. The index of each keyword is important:
//		 "out[state] & (1 << i)" is > 0 if we just found word[i]
//		 in the text.
// Returns the number of states that the built machine has.
// States are numbered 0 up to the return value - 1, inclusive.
int buildMatchingMachine(string arr[], int k)
{
	// Initialize all values in output function as 0.
	memset(out, 0, sizeof out);

	// Initialize all values in goto function as -1.
	memset(g, -1, sizeof g);

	// Initially, we just have the 0 state
	int states = 1;

	// Construct values for goto function, i.e., fill g[][]
	// This is same as building a Trie for arr[]
	for (int i = 0; i < k; ++i)
	{
		const string &word = arr[i];
		int currentState = 0;

		// Insert all characters of current word in arr[]
		for (int j = 0; unsigned(j) < word.size(); ++j)
		{
			int ch = word[j] - 'A';

			// Allocate a new node (create a new state) if a
			// node for ch doesn't exist.
			if (g[currentState][ch] == -1)
				g[currentState][ch] = states++;

			currentState = g[currentState][ch];
		}

		// Add current word in output function
		out[currentState] |= (1 << i);
	}

	// For all characters which don't have an edge from
	// root (or state 0) in Trie, add a goto edge to state
	// 0 itself
	for (int ch = 0; ch < MAXC; ++ch)
		if (g[0][ch] == -1)
			g[0][ch] = 0;

	// Now, let's build the failure function

	// Initialize values in fail function
	memset(f, -1, sizeof f);

	// Failure function is computed in breadth first order
	// using a queue
	queue<int> q;

	// Iterate over every possible input
	for (int ch = 0; ch < MAXC; ++ch)
	{
		// All nodes of depth 1 have failure function value
		// as 0. For example, in above diagram we move to 0
		// from states 1 and 3.
		if (g[0][ch] != 0)
		{
			f[g[0][ch]] = 0;
			q.push(g[0][ch]);
		}
	}

	// Now queue has states 1 and 3
	while (q.size())
	{
		// Remove the front state from queue
		int state = q.front();
		q.pop();

		// For the removed state, find failure function for
		// all those characters for which goto function is
		// not defined.
		for (int ch = 0; ch <= MAXC; ++ch)
		{
			// If goto function is defined for character 'ch'
			// and 'state'
			if (g[state][ch] != -1)
			{
				// Find failure state of removed state
				int failure = f[state];

				// Find the deepest node labeled by proper
				// suffix of string from root to current
				// state.
				while (g[failure][ch] == -1)
					failure = f[failure];

				failure = g[failure][ch];
				f[g[state][ch]] = failure;

				// Merge output values
				out[g[state][ch]] |= out[failure];

				// Insert the next level node (of Trie) in Queue
				q.push(g[state][ch]);
			}
		}
	}

	return states;
}

// Returns the next state the machine will transition to using goto
// and failure functions.
// currentState - The current state of the machine. Must be between
//			 0 and the number of states - 1, inclusive.
// nextInput - The next character that enters into the machine.
int findNextState(int currentState, char nextInput)
{
	int answer = currentState;
	int ch = nextInput - 'A';

	// If goto is not defined, use failure function
	while (g[answer][ch] == -1)
		answer = f[answer];

	return g[answer][ch];
}

// This function finds all occurrences of all array words
// in text.
void searchWords(string arr[], int k, string text,char* r)
{
	// Preprocess patterns.
	// Build machine with goto, failure and output functions
	
	char m[1024];
	buildMatchingMachine(arr, k);


	// Initialize current state
	int currentState = 0;
	

	// Traverse the text through the built machine to find
	// all occurrences of words in arr[]
	
	auto size = text.size();
	int textSize = static_cast<int>(size);


	#pragma omp parallel for
	
	for (int i = 0; i< textSize; ++i)
	{


		currentState = findNextState(currentState, text[i]);


		// If match not found, move to next state
		if (out[currentState] == 0)
			continue;

		// Match found, print all matching words of arr[]
		// using output function.
		for (int j = 0; j < k; ++j)
		{
			if (out[currentState] & (1 << j))
			{

				char* char_array = new char[arr[j].size() + 1];
				strcpy(char_array, arr[j].c_str());


				int init = i - arr[j].size() +1;
				sprintf(m,"Word %s appears from %d to %d\n",char_array,init,i);
				strcat(r,m);
				
			}
		}
	}
}


void foo() 
{
  system("./client");
}


int main(int argc, char const* argv[])
{

	//set number of thread
	int NUM_HILOS = 16;
	omp_set_num_threads(NUM_HILOS);

	//measure server time execution
	
	auto start = std::chrono::high_resolution_clock::now();
	
	int server_fd, new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);

	
  	vector<string> strings;

	std::ifstream file("tests/S4.txt");
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			// using printf() in all tests for consistency
			strings.push_back(line.c_str());
			printf("%s\n", line.c_str());
		}
    file.close();
}


   




	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET,
				SO_REUSEADDR | SO_REUSEPORT, &opt,
				sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr*)&address,
			sizeof(address))
		< 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	//std::thread first (foo);     // spawn new thread that calls foo()


		auto size = strings.size();
		int resultSize = static_cast<int>(size);
        

		string arrStrings[resultSize]; //array of strings
		std::copy(strings.begin(), strings.end(), arrStrings); 



		

    while(1){


		char buffer[1024]; 

		char r[1024];
		


        if ((new_socket = accept(server_fd, (struct sockaddr*)&address,(socklen_t*)&addrlen))< 0) {
            printf("accept: %d",new_socket);
            exit(EXIT_FAILURE);
	    }
        

        //obtenemos usuario
		if ((readLine(new_socket, buffer, 256)==-1)){printf("Error en el servidor");break;}
        printf("TEXT TO ANALIZE:%s\n", buffer);


		char textToAnalize[1024];
		sprintf(textToAnalize,buffer); //we pass the word to a new variable


		if (strcmp(textToAnalize,"FINISH") == 0){
			printf("FINISH\n");
			break;
		}

		/*
		for (int i = 0; i < resultSize; i++){
			cout << "Pattern to analize: "<<arrStrings[i] << endl;
		}
		*/
        
		
		int k = sizeof(arrStrings)/sizeof(arrStrings[0]);

		printf("K:%d\n",k);
		searchWords(arrStrings, k, string(textToAnalize),r);
			
		printf("Dopo: %s\n",r);
		

		
		printf("FOUND PATTERNS:\n%s\n",r);

		if (strcmp(r,"") == 0){
			sprintf(r, "No matches, pattern not found");
		}

		sprintf(buffer,r);
		if ((sendMessage(new_socket, buffer, strlen(buffer)+1) == -1)){printf("Error en envío\n");break;}  
		// Use the result.
        printf("Index message sent\n");

		//flush r
		sprintf(r,"\n");

		
    }
	close(new_socket);

	// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);

	//end of time measurement
	auto stop = std::chrono::high_resolution_clock::now();
	
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	cout << "Time taken by function in server: " << duration.count()/1000000.0 << " second" << endl;
  	//first.join();                // pauses until first finishes

	return 0;
}
