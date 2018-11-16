
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);

    int worldsize;
    MPI_Comm_size(MPI_COMM_WORLD, &worldsize);
    int worldrank;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldrank);

    int i,j,sum=0;
    float x[worldsize];
    int a[worldsize][worldsize], b[worldsize];
    for(i=0;i<worldsize;i++)
	for(j=0;j<worldsize;j++)
		a[i][j] = -1;
    for(i=0;i<worldsize;i++)
    {
	b[i] = rand()%10;
    }
    for(i=0;i<worldsize;i++)
    {
	for(j=0;j<=i;j++)
	{
		a[i][j] = rand()%10;
	}
    }
    if(worldrank == 0)
    {
	x[worldrank] = b[worldrank]/(a[worldrank][worldrank]*1.0);
	printf("x[%d] = %f\n",worldrank,x[worldrank]);
        MPI_Send(&x[worldrank], 1, MPI_FLOAT, 1, 0, MPI_COMM_WORLD);
    }
    else if(worldrank > 0 && worldrank < (worldsize-1))
    {
	for(j=0;j<worldrank;j++)
	{
		MPI_Recv(&x[j], 1, MPI_FLOAT, worldrank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Send(&x[j], 1, MPI_FLOAT, worldrank + 1, 0, MPI_COMM_WORLD);
		sum = sum + a[worldrank][j]*x[j];
	}
	x[worldrank] = (b[worldrank] - sum)/(a[worldrank][worldrank]*1.0);
	printf("x[%d] = %f\n",worldrank,x[worldrank]);
	MPI_Send(&x[worldrank], 1, MPI_FLOAT, worldrank+1, 0, MPI_COMM_WORLD);
		
    }
    else
    {
	for(j=0;j<worldrank;j++)
	{
		MPI_Recv(&x[j], 1, MPI_FLOAT, worldrank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		sum = sum + a[worldrank][j]*x[j];
	}
	x[worldrank] = (b[worldrank] - sum)/(a[worldrank][worldrank]*1.0);
	printf("x[%d] = %f\n",worldrank,x[worldrank]);
    }
   MPI_Finalize();
}
