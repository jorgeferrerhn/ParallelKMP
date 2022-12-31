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
#include <iostream>
#define PORT 8080

void computeLPSArray(char* pat, int M, int* lps);
using namespace std;


// Prints occurrences of txt[] in pat[]
void KMPSearch(char* pat, char* txt,char* result)
{

	


	int M = strlen(pat);
	int N = strlen(txt);



	// create lps[] that will hold the longest prefix suffix
	// values for pattern
	int lps[M];

	// Preprocess the pattern (calculate lps[] array)
	computeLPSArray(pat, M, lps);

	int i = 0; // index for txt[]
	int j = 0; // index for pat[]
	while ((N - i) >= (M - j)) {
		if (pat[j] == txt[i]) {
			j++;
			i++;
		}

		if (j == M) {
			char *p;
			sprintf(p,"%s Found pattern at index %d\n", pat,i - j);			
			strcat(result,p);


			j = lps[j - 1];
			
		}

		// mismatch after j matches
		else if (i < N && pat[j] != txt[i]) {
			// Do not match lps[0..lps[j-1]] characters,
			// they will match anyway
			if (j != 0)
				j = lps[j - 1];
			else
				i = i + 1;
		}
	}





}

// Fills lps[] for given pattern pat[0..M-1]
void computeLPSArray(char* pat, int M, int* lps)
{

	
	// length of the previous longest prefix suffix
	int len = 0;

	lps[0] = 0; // lps[0] is always 0

	// the loop calculates lps[i] for i = 1 to M-1
	int i = 1;
	while (i < M) {
		if (pat[i] == pat[len]) {
			len++;
			lps[i] = len;
			i++;
		}
		else // (pat[i] != pat[len])
		{
			// This is tricky. Consider the example.
			// AAACAAAA and i = 7. The idea is similar
			// to search step.
			if (len != 0) {
				len = lps[len - 1];

				// Also, note that we do not increment
				// i here
			}
			else // if (len == 0)
			{
				lps[i] = 0;
				i++;
			}
		}
	}
}




int main(int argc, char const* argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);

    //insieme di stringhe (S)
    vector<string> strings;
    strings.push_back("AAAB");
    strings.push_back("AABAA");
    strings.push_back("CCCA");
    strings.push_back("AAAADDAD");
    strings.push_back("BBBBB");



   




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

    while(1){


		char buffer[1024];



        if ((new_socket = accept(server_fd, (struct sockaddr*)&address,(socklen_t*)&addrlen))< 0) {
            printf("accept");
            exit(EXIT_FAILURE);
	    }
        

        valread = read(new_socket, buffer, 1024);
		if (valread == -1){
			printf("Error reading\n");
		}
        printf("TEXT TO ANALIZE:%s\n", buffer);

		char textToAnalize[1024];
		sprintf(textToAnalize,buffer); //we pass the word to a new variable


		char r[1024];

		
        char** result = new char*[strings.size()];
		for (int index = 0; index < strings.size(); index++) {
			result[index] = const_cast<char*>(strings[index].c_str());
			KMPSearch(result[index],textToAnalize,r);
			
		}

		printf("FOUND PATTERNS:\n%s\n",r);

		if (strcmp(r,"") == 0){
			sprintf(r, "No matches, pattern not found");
		}
		int a = send(new_socket, r, strlen(r), 0);
		if (a == -1){
			printf("error sending back\n");
		}
		// Use the result.
        printf("Index message sent\n");

		sprintf(r,"\0"); //we flush the result
		sprintf(textToAnalize,"\0"); //flush textToAnalize
		sprintf(buffer,"\0"); //flush buffer



		

		

    }
	close(new_socket);

	
	// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}
