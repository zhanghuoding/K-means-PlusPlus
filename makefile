CC = gcc -std=c99 -g -w
client = k_meanspp


all:$(client)

$(client):$(client).c
	$(CC) -o $(client) $(client).c -lpthread -lm

clean:
	rm $(client) 
