#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 5
#define MAX_SIZE 20

typedef struct NODE {
    int reg_no;
    int age;
    char name[MAX_SIZE];
    char destination[MAX_SIZE];
    char gender;
    char journey_date[11];  // Format: YYYY-MM-DD
    char passenger_class[10];  // e.g., Sleeper, AC
    char special_request[50];  // e.g., wheelchair, meal preference
    struct NODE *next;
} node;

node* deq();
void create();
int reserve();
int cancel(int);
void enq(node*);
void display_confirmed();
void display_waiting_list();
void display_all_passengers();
void save_to_file();
void load_from_file();
void show_seat_availability();

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
    printf("Passenger Class (Sleeper/AC): ");
    scanf("%s", start->passenger_class);
    printf("Special Request (if any): ");
    scanf("%s", start->special_request);

    start->next = NULL;
    num++;
}

int reserve() {
    if (num >= SIZE) {
        printf("Booking Full! Adding to waiting list.\n");
        node *waiting_passenger = (node *)malloc(sizeof(node));
        printf("Name: ");
        scanf("%s", waiting_passenger->name);
        printf("Age: ");
        scanf("%d", &waiting_passenger->age);
        printf("Gender (M/F): ");
        scanf(" %c", &waiting_passenger->gender);
        printf("Destination: ");
        scanf("%s", waiting_passenger->destination);
        printf("Journey Date (YYYY-MM-DD): ");
        scanf("%s", waiting_passenger->journey_date);
        printf("Passenger Class (Sleeper/AC): ");
        scanf("%s", waiting_passenger->passenger_class);
        printf("Special Request (if any): ");
        scanf("%s", waiting_passenger->special_request);

        waiting_passenger->next = NULL;
        enq(waiting_passenger);
        return 0;
    }

    node *new_node = (node *)malloc(sizeof(node));
    new_node->reg_no = ++num;
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
    printf("Passenger Class (Sleeper/AC): ");
    scanf("%s", new_node->passenger_class);
    printf("Special Request (if any): ");
    scanf("%s", new_node->special_request);
    
    new_node->next = NULL;

    if (start == NULL) {
        start = new_node;
    } else {
        node *temp = start;
        while (temp->next != NULL) temp = temp->next;
        temp->next = new_node;
    }

    printf("\nBooking Successful! Enjoy your journey! Your Reg No. is %d\n", new_node->reg_no);
    return new_node->reg_no;
}

int cancel(int reg) {
    node *ptr = start, *prev = NULL;

    if (ptr == NULL) return -1;

    while (ptr != NULL && ptr->reg_no != reg) {
        prev = ptr;
        ptr = ptr->next;
    }

    if (ptr == NULL) return -1;

    if (prev == NULL) start = start->next;
    else prev->next = ptr->next;

    free(ptr);
    num--;
    printf("Reservation cancelled successfully.\n");

    if (count > 0) {
        node *waiting = deq();
        if (waiting) {
            waiting->reg_no = reg;
            if (prev == NULL) {
                waiting->next = start;
                start = waiting;
            } else {
                waiting->next = prev->next;
                prev->next = waiting;
            }
            num++;
            printf("A passenger from the waiting list has been confirmed.\n");
        }
    }
    return 1;
}

void enq(node *new_node) {
    if (rear == NULL) {
        front = rear = new_node;
    } else {
        rear->next = new_node;
        rear = new_node;
    }
    count++;
    printf("Added to waiting list. Position: %d\n", count);
}

node* deq() {
    if (front == NULL) return NULL;
    node *temp = front;
    front = front->next;
    if (front == NULL) rear = NULL;
    count--;
    return temp;
}

void display_confirmed() {
    node *temp = start;
    printf("\nConfirmed Reservations:\n");
    while (temp != NULL) {
        printf("\nReg No: %d\nName: %s\nAge: %d\nGender: %c\nDestination: %s\nJourney Date: %s\nClass: %s\nSpecial Request: %s\n", 
            temp->reg_no, temp->name, temp->age, temp->gender, temp->destination, temp->journey_date, temp->passenger_class, temp->special_request);
        temp = temp->next;
    }
}

void display_waiting_list() {
    node *temp = front;
    if (temp == NULL) {
        printf("\nNo passengers in waiting list.\n");
    } else {
        printf("\nWaiting List:\n");
        int pos = 1;
        while (temp != NULL) {
            printf("\nPosition: %d\nName: %s\nAge: %d\nGender: %c\nDestination: %s\nJourney Date: %s\nClass: %s\nSpecial Request: %s\n", 
                pos++, temp->name, temp->age, temp->gender, temp->destination, temp->journey_date, temp->passenger_class, temp->special_request);
            temp = temp->next;
        }
    }
}

void display_all_passengers() {
    printf("\nAll Passengers:\n");
    display_confirmed();
    display_waiting_list();
}

void save_to_file() {
    FILE *file = fopen("reservations.txt", "w");
    node *temp = start;
    while (temp != NULL) {
        fprintf(file, "%d %s %d %c %s %s %s %s\n", temp->reg_no, temp->name, temp->age, temp->gender, temp->destination, temp->journey_date, temp->passenger_class, temp->special_request);
        temp = temp->next;
    }
    fclose(file);
}

void load_from_file() {
    FILE *file = fopen("reservations.txt", "r");
    if (file != NULL) {
        while (!feof(file)) {
            node *new_node = (node *)malloc(sizeof(node));
            fscanf(file, "%d %s %d %c %s %s %s %s", &new_node->reg_no, new_node->name, &new_node->age, &new_node->gender, new_node->destination, new_node->journey_date, new_node->passenger_class, new_node->special_request);
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

void show_seat_availability() {
    printf("\nAvailable Seats: %d\n", SIZE - num);
    printf("Waiting List Size: %d\n", count);
}

int main() {
    int choice, status = 0, reg = 0;
    load_from_file();
    printf("\n\t\t\t*RAILWAY RESERVATION*\t\t\t\n");
    while (1) {
        printf("\n1. Reserve a Ticket\n2. Cancel Booking\n3. Display Confirmed Passengers\n4. Display Waiting List\n5. Show Seat Availability\n6. Display All Passengers\n7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                status = reserve();
                break;
            case 2:
                printf("Enter the Registration number to cancel: ");
                scanf("%d", &reg);
                status = cancel(reg);
                if (status == -1) printf("Invalid Registration Number.\n");
                break;
            case 3:
                display_confirmed();
                break;
            case 4:
                display_waiting_list();
                break;
            case 5:
                show_seat_availability();
                break;
            case 6:
                display_all_passengers();
                break;
            case 7:
                save_to_file();
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}
