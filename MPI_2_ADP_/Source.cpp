#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"


int main(int argc, char** argv) {

	MPI_Status status;
	int procsSize, idProc, i, oldRandom, oldRank,random;
	bool lider = true;
	srand(time(NULL));
	random = rand();

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &idProc);
	MPI_Comm_size(MPI_COMM_WORLD, &procsSize);


	for (i = 0; i < procsSize; i++) {
		if (i != idProc) {
			MPI_Send(&random, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
			MPI_Recv(&oldRandom, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
			if (random < oldRandom) {
				lider = false;
			}
			else if (random == oldRandom) {
				MPI_Send(&idProc, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
				MPI_Recv(&oldRank, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
				if (idProc < oldRank) {
					lider = false;
				}
			}
		}
	}
	printf("%d - rank %d",random, idProc);
	if (lider==true) {
		printf(" <<<-- Acesta este procesul lider.");
	}
	MPI_Finalize();
}
