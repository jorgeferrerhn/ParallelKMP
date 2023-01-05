#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

int main(int argc, char* argv[]){
    




	int myid,numprocs;

	//MPI_Finalize();


    

	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD ,&numprocs); 

	MPI_Comm_rank(MPI_COMM_WORLD,&myid);


    MPI_Group g1,g2;
    MPI_Group group;


    int i, *ranks, g1size,g2size,thread0;
    
    MPI_Comm_group( MPI_COMM_WORLD, &g1 );
    MPI_Group_size( g1, &g1size );


    printf("Gsize: %d\n",g1size);
    ranks = (int *)malloc( (g1size-1) * sizeof(int) );
    for (i=1; i<g1size; i++){
        ranks[i-1] = i;
        printf("%d\n",ranks[i-1]);
    }
    MPI_Group_incl( g1, g1size-1, ranks, &g2 );
    MPI_Group_size( g2, &g2size );


    printf("G2 size: %d\n",g2size);


    int destrank = 0;
    MPI_Group_incl(g1, 1, &destrank, &group);
    MPI_Group_size( group, &thread0 );
    printf("Thread 0 group size: %d\n",thread0);

    //g2 appartiene tutti tranne il 0


	char buf[1024]; //window buffer
    //MPI_Win window;

    //comm group contains all proccesses
    MPI_Group comm_group;



    


    
    //father group
    int father[1] = {0};

    char my_string[1024];

    MPI_Win window;
    
    //printf("Number of proccesses: %d\n",numprocs);

    char window_buffer[1024];

    MPI_Win_create(window_buffer,1024*numprocs,1024,MPI_INFO_NULL,MPI_COMM_WORLD,&window);
    

    int n ;

	
	while(1){


        if (myid != 0){ //access epoch

       
            MPI_Win_start(group,0,window); //start access epoch

            printf("Readline\n");


            printf("Faccio le operazioni\n");

            sprintf(my_string,"Sono il thread %d",myid);

            MPI_Put(&my_string, sizeof(my_string)+1, MPI_CHAR, 0, 0, 1, MPI_CHAR, window);

            MPI_Win_complete(window); //end access epoch

            printf("Sendmessage\n");

        }
        
        if (myid == 0){ //exposure epoch


            MPI_Win_post(g2,0,window); //start exposure epoch

            printf("Thread %d gets info from here: %s\n",myid,window_buffer);

            MPI_Win_wait(window); //stop exposure epoch
            
            

           


        }

    }
    



}