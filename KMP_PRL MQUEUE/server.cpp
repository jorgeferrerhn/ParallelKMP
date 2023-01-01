// Server side C/C++ program to demonstrate Socket
// programming
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include "lines.h"
#include <mqueue.h>

pthread_mutex_t mutex_mensaje;
int mensaje_no_copiado = true;
pthread_cond_t cond_mensaje;
mqd_t  q_servidor;


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



void* tratar_peticion(void* b){

	//cast buffer to char
	char* buffer; 
	buffer = (char*)b;


	mqd_t q_cliente;                /* cola de mensajes del cliente */
	
	pthread_mutex_lock(&mutex_mensaje);	
	/* ya se puede despertar al servidor*/
	mensaje_no_copiado = false;

	pthread_cond_signal(&cond_mensaje);
	pthread_mutex_unlock(&mutex_mensaje);
	
	q_cliente = mq_open("/CLIENT", O_WRONLY);
	
	if (q_cliente < 0) {
		perror("mq_open");
		mq_close(q_servidor);
		mq_unlink("/SERVER");
		
	}


	printf("TEXT TO ANALIZE:%s\n", buffer);


	char textToAnalize[1024];
	sprintf(textToAnalize,buffer); //we pass the word to a new variable


	if (strcmp(textToAnalize,"FINISH") == 0){
		printf("FINISH\n");
		exit(1);
	}


	char r[1024];

	//insieme di stringhe (S)
    vector<string> strings;
    strings.push_back("AAAB");
    strings.push_back("AABAA");
    strings.push_back("CCCA");
    strings.push_back("AAAADDAD");
    strings.push_back("BBBBB");

	
	char** result = new char*[strings.size()];
	
	for (int index = 0; (unsigned)index < strings.size(); index++) {
		result[index] = const_cast<char*>(strings[index].c_str());
		KMPSearch(result[index],textToAnalize,r);
		
	}

	printf("FOUND PATTERNS:\n%s\n",r);

	if (strcmp(r,"") == 0){
		sprintf(r, "No matches, pattern not found");
	}

	sprintf(buffer,r);

	if (mq_send(q_cliente, buffer, sizeof(buffer), 0) < 0) {
			perror("mq_send");
			mq_close(q_servidor);
			mq_unlink("/SERVER");
		}

		
	
	// Use the result.
	printf("Index message sent\n");

	//flush r
	sprintf(r,"\n");

	return NULL;

		

}

int main(int argc, char const* argv[])
{
	//measure server time execution

	auto start = std::chrono::high_resolution_clock::now();

	pthread_attr_t t_attr;		// atributos de los threads 
   	pthread_t thid;
	   
	struct mq_attr attr;

	attr.mq_maxmsg = 10; //num_threads?               
	attr.mq_msgsize = sizeof(char[1024]);


	q_servidor = mq_open("/SERVER", O_CREAT|O_RDONLY, 0700, &attr);

	if (q_servidor == -1) {
		perror("mq_open");
		return -1;
	}

	pthread_mutex_init(&mutex_mensaje, NULL);
	pthread_cond_init(&cond_mensaje, NULL);
	pthread_attr_init(&t_attr);

	// atributos de los threads, threads independientes
	pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);


	
    while(1){


		char buffer[1024]; 

       
	    if (mq_receive(q_servidor, buffer, 1024, 0) < 0){
			perror("mq_recev");
			return -1;
		}

		printf("Texto a analizar antes de lanzar los hilos: %s\n",buffer);
		//cast buffer to void
		void* b = (void*)buffer;


		if (pthread_create(&thid, &t_attr,&tratar_peticion, &b)== 0) {
			// se espera a que el thread copie el mensaje 
			pthread_mutex_lock(&mutex_mensaje);
			while (mensaje_no_copiado)
				pthread_cond_wait(&cond_mensaje, &mutex_mensaje);
			mensaje_no_copiado = true;
			pthread_mutex_unlock(&mutex_mensaje);
		}   



    }


	//end of time measurement
	auto stop = std::chrono::high_resolution_clock::now();
	
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

	return 0;
}
