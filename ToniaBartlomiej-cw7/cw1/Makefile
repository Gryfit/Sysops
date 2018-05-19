all: clients barber

clients: clients.c
	gcc clients.c -o clients -Wall -lpthread -lrt

barber: barber.c
	gcc barber.c -o barber -Wall -lpthread -lrt

clean:
	rm {clients,barber} 2> /dev/null
