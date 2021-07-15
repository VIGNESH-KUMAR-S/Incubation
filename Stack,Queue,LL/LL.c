#include<stdio.h>

typedef struct LL {
	int data;
	struct LL* next;
}node;

node* createNode() {
	node* temp = NULL;
	temp = (node*)malloc(sizeof(node));
	printf("Enter data : ");
	scanf("%d", &temp->data);
	temp->next = NULL;
	return temp;
}

int main(void) {
	node ll;
	node* head = NULL;
	node* tail = NULL;
	int count = 0;
	while (1) {
		int one;
		printf("If you want to create new node then press 1 : ");
		scanf("%d", &one);
		if (one == 1) {
			if (count == 0)
			{
				head = tail = createNode();
			}
			else
			{
				tail = (tail->next = createNode());
			}
			count++;
		}
		else {
			break;
		}
	}

	while (head != NULL) {
		printf("Current Address : %d\n", head);
		printf("Current Data : %d\n", head->data);
		printf("Next Address : %d\n\n", head->next);
		head = head->next;
	}
}