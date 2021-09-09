#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct message {
    char content[1024];
    int seq_num;
};
typedef struct message message;

struct Queue {
	int front, rear, size;
	unsigned capacity;
	message* array;
};

struct Queue* createQueue(unsigned capacity)
{
	struct Queue* queue = (struct Queue*)malloc(
		sizeof(struct Queue));
	queue->capacity = capacity;
	queue->front = queue->size = 0;

	queue->rear = capacity - 1;
	queue->array = (message*)malloc(
		queue->capacity * sizeof(message));
	return queue;
}

int isFull(struct Queue* queue)
{
	return (queue->size == queue->capacity);
}

int isEmpty(struct Queue* queue)
{
	return (queue->size == 0);
}

void enqueue(struct Queue* queue, message packet)
{
	if (isFull(queue))
		return;
	queue->rear = (queue->rear + 1)
				% queue->capacity;
	queue->array[queue->rear] = packet;
	queue->size = queue->size + 1;
	printf("%d enqueued to queue\n", packet.seq_num);
}

message dequeue(struct Queue* queue)
{
	if (isEmpty(queue)) {
		// create a message with content "NULL"
		message packet;
		packet.content[0] = '\0';
		packet.seq_num = -1;
		return packet;
	}
	message packet = queue->array[queue->front];
	queue->front = (queue->front + 1)
				% queue->capacity;
	queue->size = queue->size - 1;
	return packet;
}

message front(struct Queue* queue)
{
	if (isEmpty(queue)) {
		// create a message with content "NULL"
		message packet;
		packet.content[0] = '\0';
		packet.seq_num = -1;
		return packet;
	}
	return queue->array[queue->front];
}

message rear(struct Queue* queue)
{
	if (isEmpty(queue)) {
		// create a message with content "NULL"
		message packet;
		packet.content[0] = '\0';
		packet.seq_num = -1;
		return packet;
	}
	return queue->array[queue->rear];
}