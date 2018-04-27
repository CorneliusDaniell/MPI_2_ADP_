#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"


int main(int argc, char** argv) {

	MPI_Status status;
	int procsSize, idProc, i, oldRandom, oldIdProc, random, lider = 0;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &idProc);
	MPI_Comm_size(MPI_COMM_WORLD, &procsSize);

	srand(time(NULL) + idProc*idProc + idProc);
	random = rand();
	printf("%d - rank %d", random, idProc);
	oldRandom = random;
	oldIdProc = idProc;
	for (i = 0; i < procsSize; i++) {
		if (i != idProc) {
			MPI_Send(&random, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
			MPI_Recv(&oldRandom, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
			if (random < oldRandom) {
				//printf("1");
				lider = 0;
			}
			else if (random == oldRandom) {
				MPI_Send(&idProc, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
				MPI_Recv(&oldIdProc, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
				if (idProc < oldIdProc) {
					lider = 0;
					//printf("2");
				}
			}
		}
	}
	if (lider == 1) {
		printf(" <<<-- This is the leader.");
	}
	MPI_Finalize();
}
