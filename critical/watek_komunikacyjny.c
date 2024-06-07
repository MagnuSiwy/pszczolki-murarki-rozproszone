#include "main.h"
#include "watek_komunikacyjny.h"

/* wątek komunikacyjny; zajmuje się odbiorem i reakcją na komunikaty */
void *startKomWatek(void *ptr)
{
    MPI_Status status;
    int is_message = FALSE;
    packet_t pakiet;
    /* Obrazuje pętlę odbierającą pakiety o różnych typach */
    while ( stan!=Dead ) {
	debug("czekam na recv");
        MPI_Recv( &pakiet, 1, MPI_PAKIET_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        
        pthread_mutex_lock(&clock_mutex);
        if (pakiet.ts > zegar) {
            zegar = pakiet.ts;
        }
        zegar++;
        pthread_mutex_unlock(&clock_mutex);
    
        switch ( status.MPI_TAG ) {
	    case REED_REQUEST: 
            debug("Ktoś coś prosi. A niech ma!")
            if(stan != Dead)
		        sendPacket( 0, status.MPI_SOURCE, REED_ACK );
	    break;
        case REED_RELEASE: 
            debug("Ktoś zwalnia trzcinę")
            //pthread_mutex_lock(&bee_mutex);
		    deadBees++;
            //pthread_mutex_unlock(&bee_mutex);
	    break;
	    case REED_ACK: 
            debug("Dostałem ACK od %d, mam już %d", status.MPI_SOURCE, ackCount);
	        ackCount++; /* czy potrzeba tutaj muteksa? Będzie wyścig, czy nie będzie? Zastanówcie się. */
	    break;
	    default:
	    break;
        }
    }
}
