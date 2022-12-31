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


#define PORT 8080

using namespace std;


int main(int argc, char const* argv[])
{
	int sock = 0, valread, client_fd;
	struct sockaddr_in serv_addr;

	//traffico di pachetti (T)
    vector<string> traffico;

    traffico.push_back("BBBBBAABCCAAABAAAAAADDAD");
	traffico.push_back("ASDAKSAAABCCCA");
    traffico.push_back("AAAADDADBAAASDBASJS");
    traffico.push_back("0");
    traffico.push_back("11111111111111");
	

 
   
	
        
	char** result = new char*[traffico.size()];

	for (int index = 0; index < traffico.size(); index++) {

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
		
		int a = send(sock, result[index], strlen(result[index]), 0);
		if (a == -1){
			printf("Error");
			
		}
		printf("Text sent\n");

		char recvBuffer[1024];

		//aspetta il result
		valread = read(sock, recvBuffer, 1024);
		printf("RESULT: %s\n", recvBuffer);

		//we flush the buffer and the result
		sprintf(buffer,"");
		sprintf(recvBuffer,"");


		close(client_fd);

		}


	

	// closing the connected socket
	return 0;
}
