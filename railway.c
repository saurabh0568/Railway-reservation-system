#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SIZE 5
#define MAX_SIZE 20

typedef struct NODE {
	int reg_no;
	int age;
	char name[MAX_SIZE];
	char destination[MAX_SIZE];
	char gender;
	char journey_date[11];  // Format: YYYY-MM-DD
	struct NODE *next;
} node;

node* deq();
void create();
int reserve();
int cancel(int);
void enq(node*);
void display();
void display_waiting_list();
void save_to_file();
void load_from_file();

node *start = NULL;
node *front = NULL;
node *rear = NULL;
int count = 0;
int num = 0;

void create() {
	start = (node *)malloc(sizeof(node));
	start->reg_no = 1;

	printf("Name: ");
	scanf("%s", start->name);
	printf("Age: ");
	scanf("%d", &start->age);
	printf("Gender (M/F): ");
	scanf(" %c", &start->gender);
	printf("Destination: ");
	scanf("%s", start->destination);
	printf("Journey Date (YYYY-MM-DD): ");
	scanf("%s", start->journey_date);

	start->next = NULL;
	num++;
}

int reserve() {
	if (start == NULL) {
		create();
		return 1;
	} else {
		node *temp, *new_node, *prev;
		new_node = (node *)malloc(sizeof(node));
		
		printf("Name: ");
		scanf("%s", new_node->name);
		printf("Age: ");
		scanf("%d", &new_node->age);
		printf("Gender (M/F): ");
		scanf(" %c", &new_node->gender);
		printf("Destination: ");
		scanf("%s", new_node->destination);
		printf("Journey Date (YYYY-MM-DD): ");
		scanf("%s", new_node->journey_date);

		new_node->next = NULL;
		temp = start;
		int i = 1;

		if (temp->reg_no == 0) {
			temp->reg_no = 1;
			strcpy(temp->name, new_node->name);
			temp->age = new_node->age;
			strcpy(temp->destination, new_node->destination);
			strcpy(temp->journey_date, new_node->journey_date);
			temp->gender = new_node->gender;
			num++;
			return 1;
		}
		
		while (temp->next != NULL) {
			if (temp->reg_no != i++) break;
			prev = temp;
			temp = temp->next;
		}

		if (num < SIZE) {
			num++;
			i++;
			if (temp->reg_no == (prev->reg_no + 1) || i == 2) {
				new_node->reg_no = i;
				temp->next = new_node;
				return i;
			} else {
				new_node->next = temp;
				prev->next = new_node;
				new_node->reg_no = (temp->reg_no) - 1;
				return new_node->reg_no;
			}
		} else {
			enq(new_node);
			return 0;
		}
	}
}

int cancel(int reg) {
	node *ptr, *preptr, *new;
	ptr = start;
	preptr = NULL;
	
	if (start == NULL) return -1;
	if (ptr->next == NULL && ptr->reg_no == reg) {
		start = NULL;
		num--;
		free(ptr);
		return 1;
	} else {
		if (reg == 1) {
			ptr->reg_no = 0;
			new = deq(reg);
			if (new != NULL) {
				ptr->reg_no = 1;
				strcpy(ptr->name, new->name);
				ptr->age = new->age;
				strcpy(ptr->destination, new->destination);
				strcpy(ptr->journey_date, new->journey_date);
				ptr->gender = new->gender;
				num++;
				return 1;
			}
		} else {
			while (ptr->reg_no != reg && ptr->next != NULL) {
				preptr = ptr;
				ptr = ptr->next;
			}

			if (ptr->next == NULL && ptr->reg_no != reg) return -1;
			else preptr->next = ptr->next;

			free(ptr);
			new = deq(reg);
			num--;
			if (new != NULL) {
				node *waiting = start;
				while (waiting->reg_no != (new->reg_no - 1)) waiting = waiting->next;
				new->next = waiting->next;
				waiting->next = new;
				num++;
			}
			return 1;
		}
	}
}

void enq(node *new_node) {
	if (rear == NULL) {
		rear = new_node;
		rear->next = NULL;
		front = rear;
	} else {
		node *temp = new_node;
		rear->next = temp;
		temp->next = NULL;
		rear = temp;
	}
	count++;
}

node* deq(int reg) {
	node *front1 = front;
	if (front == NULL) return NULL;
	else {
		count--;
		if (front->next != NULL) {
			front = front->next;
			front1->next = NULL;
			front1->reg_no = reg;
			return front1;
		} else {
			front = NULL;
			rear = NULL;
			front1->reg_no = reg;
			return front1;
		}
	}
}

void display() {
	node *temp = start;
	while (temp != NULL) {
		if (temp->reg_no != 0) {
			printf("\nRegistration Number: %d\n", temp->reg_no);
			printf("Name: %s\n", temp->name);
			printf("Age: %d\n", temp->age);
			printf("Gender: %c\n", temp->gender);
			printf("Destination: %s\n", temp->destination);
			printf("Journey Date: %s\n", temp->journey_date);
		}
		temp = temp->next;
	}
}

void display_waiting_list() {
	node *temp = front;
	if (temp == NULL) {
		printf("\nNo passengers in waiting list.\n");
	} else {
		printf("\nWaiting List:\n");
		while (temp != NULL) {
			printf("\nName: %s\n", temp->name);
			printf("Age: %d\n", temp->age);
			printf("Gender: %c\n", temp->gender);
			printf("Destination: %s\n", temp->destination);
			printf("Journey Date: %s\n", temp->journey_date);
			temp = temp->next;
		}
	}
}

void save_to_file() {
	FILE *file = fopen("reservations.txt", "w");
	node *temp = start;
	while (temp != NULL) {
		fprintf(file, "%d %s %d %c %s %s\n", temp->reg_no, temp->name, temp->age, temp->gender, temp->destination, temp->journey_date);
		temp = temp->next;
	}
	fclose(file);
}

void load_from_file() {
	FILE *file = fopen("reservations.txt", "r");
	if (file != NULL) {
		while (!feof(file)) {
			node *new_node = (node *)malloc(sizeof(node));
			fscanf(file, "%d %s %d %c %s %s", &new_node->reg_no, new_node->name, &new_node->age, &new_node->gender, new_node->destination, new_node->journey_date);
			new_node->next = NULL;
			if (start == NULL) {
				start = new_node;
			} else {
				node *temp = start;
				while (temp->next != NULL) temp = temp->next;
				temp->next = new_node;
			}
			num++;
		}
		fclose(file);
	}
}

int main() {
	int choice, status = 0, canc = 0, reg = 0;
	load_from_file();
	printf("\n\t\t\t*RAILWAY RESERVATION*\t\t\t\t\n");
	while (1) {
		printf("\n\n1. Reserve a ticket\n2. Cancel Booking\n3. Display Passenger Details\n4. Display Waiting List\n5. Exit\n");
		printf("Enter your choice: ");
		scanf("%d", &choice);
		switch (choice) {
			case 1:
				status = reserve();
				if (status == 0) printf("\nBooking Full! You are added to the waiting list. Waiting list number %d\n", count);
				else printf("\nBooking Successful! Enjoy your journey! Your Reg No. is %d\n", status);
				break;
			case 2:
				printf("\nEnter Registration Number to be cancelled: ");
				scanf("%d", &reg);
				if (reg > num) printf("Registration number invalid!\n");
				else {
					canc = cancel(reg);
					if (canc == -1) printf("Registration number invalid!\n");
					else printf("Registration cancelled successfully\n");
				}
				break;
			case 3:
				display();
				break;
			case 4:
				display_waiting_list();
				break;
			case 5:
				save_to_file();
				printf("Goodbye!\n");
				exit(0);
				break;
			default:
				printf("\nInvalid choice!\n");
				break;
		}
	}
	return 0;
}
