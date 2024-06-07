#include "main.h"
#include "watek_glowny.h"

void mainLoop()
{
    srandom(rank);
    int tag;
    int perc;

    while (stan != Dead) {
		switch (stan) {
			case InRunReed: 
			perc = random()%100;
			if ( perc < 25 ) {
				debug("Perc: %d", perc);
				println("Ubiegam się o trzcinę")
				debug("Zmieniam stan na wysyłanie");
				packet_t *pkt = malloc(sizeof(packet_t));
				pkt->data = perc;
				ackCount = 0;
				for (int i=0;i<=size-1;i++)
				if (i!=rank)
					sendPacket( pkt, i, REED_REQUEST);
				changeState( WaitForReed ); 
				free(pkt);
			}
			debug("Skończyłem myśleć");
			break;
			case WaitForReed:
			println("Czekam na wejście na trzcinę");
			// tutaj zapewne jakiś semafor albo zmienna warunkowa
			// bo aktywne czekanie jest BUE
			println("dostałem %d ack, %d", ackCount, size - deadBees - 1);
			if (ackCount == size - 1) 
			{
				changeState(InRunFlower);
			}
			break;
			case InRunFlower:
			// tutaj zapewne jakiś muteks albo zmienna warunkowa
				println("Ubiegam się o kwiat");
				sleep(5);
			//if ( perc < 25 ) {
				debug("Perc: %d", perc);
				println("Zwalniam trzcinę")
				debug("Zmieniam stan na wysyłanie");
				packet_t *pkt = malloc(sizeof(packet_t));
				pkt->data = perc;
				for (int i=0;i<=size-1;i++)
				if (i!=rank)
					sendPacket( pkt, (rank+1)%size, REED_RELEASE);
				changeState( Dead );
				free(pkt);
			//}
			break;
			default: 
			break;
        }
        sleep(SEC_IN_STATE);
    }
}
