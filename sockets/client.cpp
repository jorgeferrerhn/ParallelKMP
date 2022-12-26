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

    traffico.push_back("DOGDINOSAURPHONERORYUCER");
    traffico.push_back("FISHTELEPHONECATNUMBERALLORA");
    traffico.push_back("MOBILEBICHOLAMPPENPINEAPPLE");
   
	
	
	char hello[1024];


	char buffer[1024] = { 0 };

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

        
	char** result = new char*[traffico.size()];

	for (int index = 0; index < traffico.size(); index++) {
		result[index] = const_cast<char*>(traffico[index].c_str());
		printf("%s\n",result[index]);
		send(sock, result[index], strlen(result[index]), 0);
		printf("Text sent\n");
		valread = read(sock, buffer, 1024);
		printf("%s\n", buffer);
		}


	

	// closing the connected socket
	close(client_fd);
	return 0;
}
