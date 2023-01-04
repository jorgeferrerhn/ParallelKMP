// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h> 
#include <vector>
#include <string>
#include <iostream>
#include "lines.h"
#include <chrono>
#include <fstream>




#define PORT 8080

using namespace std;


int main(int argc, char const* argv[])
{

	auto start = std::chrono::high_resolution_clock::now();


	int sock = 0, client_fd;
	struct sockaddr_in serv_addr;

	//populate list
	
    vector<string> traffico;

	std::ifstream file("tests/T16.txt");
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			// using printf() in all tests for consistency
			traffico.push_back(line.c_str());
			printf("%s\n", line.c_str());
		}
    file.close();
	}


   
        
	auto size = traffico.size();
	int stringSize = static_cast<int>(size);
	char** result = new char*[stringSize];



	printf("%d: ",stringSize);

	for (int index = 0; index < stringSize; index++) {

		char buffer[1024];

		


		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
		}

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(PORT);

		// Convert IPv4 and IPv6 addresses from text to binary
		// form
		if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
			<= 0) {
			printf(
				"\nInvalid address/ Address not supported \n");
			return -1;
		}
		if ((client_fd
		= connect(sock, (struct sockaddr*)&serv_addr,
				sizeof(serv_addr)))
		< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}

		result[index] = const_cast<char*>(traffico[index].c_str());
		printf("TEXT SENT TO ANALIZE: %s\n",result[index]);
		
		
		if ((sendMessage(sock, result[index], strlen(result[index])+1) == -1)){printf("Error en envío\n");break;}  
		
		printf("Text sent\n");


		//aspetta il result

		if ((readLine(sock, buffer, 1024)==-1)){printf("Error en el servidor");break;}

		printf("RESULT: %s\n", buffer);

		//we flush the buffer and the result
		sprintf(buffer,"\n");


		close(client_fd);

		}


	
	//end of time measurement
	auto stop = std::chrono::high_resolution_clock::now();
	
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
	// closing the connected socket
	return 0;
}
