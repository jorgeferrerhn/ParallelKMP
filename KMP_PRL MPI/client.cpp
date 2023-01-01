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
#include <mqueue.h>


#define PORT 8080

using namespace std;


int main(int argc, char const* argv[])
{

	mqd_t q_server;       /* queue of messages for server */
	mqd_t q_client;        /* queue of messages for client */

    struct mq_attr attr;

	auto start = std::chrono::high_resolution_clock::now();

	attr.mq_maxmsg = 1;     
	attr.mq_msgsize = sizeof(char[256]);  

	char processid[256];

    sprintf(processid,  "/Queue-%d", getpid());

	
	q_client = mq_open(processid, O_CREAT|O_RDONLY, 0700, &attr);


	if (q_client == -1) {
		perror("mq_open");
		return -1;
	}

	q_server = mq_open("/SERVER", O_WRONLY);
    
    if (q_server == -1){
		mq_close(q_client);
		perror("mq_open");
		return -1;
	}   

	

	


	int sock = 0, client_fd;
	struct sockaddr_in serv_addr;

	//traffico di pachetti (T)
    vector<string> traffico;

    traffico.push_back("BBBBBAABCC");
	traffico.push_back("ABBAACA");
	traffico.push_back("AAACCCADDA");
    traffico.push_back("BBBAAACBAD");
    traffico.push_back("0");
    traffico.push_back("CCCAABBCCA");
	traffico.push_back("FINISH");
	

 
   
	
        
	char** result = new char*[traffico.size()];

	for (int index = 0; unsigned(index) < traffico.size(); index++) {

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
		
		
		//we will send the message to the server queue

		if (mq_send(q_server, result[index], sizeof(result[index]+1), 0) < 0){
			perror("mq_send");
			return -1;
		}	
		
		
		printf("Text sent\n");


		//aspetta il result
		
		//we will receive from the client queue the response
		if (mq_receive(q_client, buffer, 256, 0) < 0){
			perror("mq_recv");
			return -1;
		}



		
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
