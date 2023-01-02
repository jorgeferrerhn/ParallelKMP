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
#include <chrono>
#include <fstream>
#include "lines.h"
#include <pthread.h>
#include <mpi.h>




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
			char p[100];
			int k = i-j;
			sprintf(p,"%s Found pattern at index %d\n", pat,k);			
			if (strcmp(result,"\n") == 0){
				sprintf(result,p);
			}
			else{
				strcat(result,p);
			}
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


void foo() 
{
  system("./client");
}


int main(int argc, char* argv[]) 
{

	int myid,numprocs;

	printf("Prima di tutto");

	MPI_Status status ;
	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD ,&numprocs); 

	MPI_Comm_rank(MPI_COMM_WORLD,&myid);

	printf("Number of process: %d",numprocs);


	//measure server time execution
	
	auto start = std::chrono::high_resolution_clock::now();
	
	int server_fd, new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);

  	vector<string> strings;

	std::ifstream file("tests/S800.txt");
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



	char sharedBuffer[1024]; //shared buffer between threads

	int n;
    while(1){


		char buffer[1024]; 
		


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


		char r[1024];

		
        char** result = new char*[strings.size()];
		auto size = strings.size();
		int stringSize = static_cast<int>(size);

		//father myid == 0
		//aggiungere tutto al final result
		
		
		//facciamo sappere agli altri thread il numero delle iterazioni che devono eseguire

		printf("MY ID: %d\n",myid);
		if (myid == 0){
			for (int islave = 1; islave < numprocs; islave++){

				n = stringSize;
				printf("Sending n\n");
				MPI_Send(&n,1,MPI_INT,islave,1,MPI_COMM_WORLD);
				printf("N sent\n");

			
			}
		}
		else{
			MPI_Recv(&n,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);
		}
		printf("ERROR\n");

		
		char bufferProprio[1024];
		//i thread fanno i loro calcoli 
		for (int i = myid +1; i<= stringSize; i += numprocs){
			result[i] = const_cast<char*>(strings[i].c_str());
			KMPSearch(result[i],textToAnalize,bufferProprio);
		}

		if (myid != 0){ //figlio
			MPI_Send (&bufferProprio,strlen(bufferProprio)+1, MPI_CHAR,1,0,MPI_COMM_WORLD);
		} else{ //father: aggiunge tutto
			for (int islave = 1; islave < numprocs; islave++){
				MPI_Recv (&bufferProprio,strlen(bufferProprio)+1, MPI_CHAR,islave,2,MPI_COMM_WORLD,&status);
				strcat(sharedBuffer,bufferProprio); //aggiornare il risultato al sharedBuffer

			}


		}
		

			
		
		printf("FOUND PATTERNS:\n%s\n",sharedBuffer);

		if (strcmp(sharedBuffer,"") == 0){
			sprintf(sharedBuffer, "No matches, pattern not found");
		}

		sprintf(buffer,sharedBuffer);
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

	return 0;
}
