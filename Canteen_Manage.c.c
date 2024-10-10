#include <stdio.h>
#include <string.h>

#define MAX_ITEMS 10 // Preprocessor command -> define; MAX_ITEMS=CONSTANT

// Structure to store item details
struct Item {
    char name[30];
    int price;
};

// Structure to store customer details
struct Customer {
    char name[50];
    char mobile[15];
};

// Enumeration for meal types
enum MealType {
    BREAKFAST = 1,
    LUNCH,
    DINNER
};

// Structure for the order details
struct Order {
    int itemIndex;
    int quantity;
};

// Function prototypes
void showMenu(struct Item items[], int size, const char* category);
void takeOrder(struct Item items[], int size, struct Order orders[], int *orderCount);
void showBill(struct Item items[], struct Order orders[], int orderCount, const char* category, struct Customer customer);
void viewBillMenu(struct Item breakfastItems[], struct Order breakfastOrders[], int *breakfastCount,
                 struct Item lunchItems[], struct Order lunchOrders[], int *lunchCount,
                 struct Item dinnerItems[], struct Order dinnerOrders[], int *dinnerCount,
                 struct Customer customer);

int main() {
    // Define Customer structure
    struct Customer customer;

    // Prompt for customer details
    printf("Welcome to the Canteen Management System\n");
    printf("Enter customer name: ");
    // Use fgets to read the name, allowing spaces
    fgets(customer.name, sizeof(customer.name), stdin);
    // Remove trailing newline character if present
    size_t len = strlen(customer.name);
    if (len > 0 && customer.name[len - 1] == '\n') {
        customer.name[len - 1] = '\0';
    }

    printf("Enter mobile number: ");
    scanf("%s", customer.mobile);

    // Separate items for Breakfast, Lunch, and Dinner
    struct Item breakfastItems[MAX_ITEMS] = {
        {"Pancakes", 100},
        {"Omelette", 80},
        {"Toast", 50},
        {"Coffee", 70},
        {"Tea", 40}
    };

    struct Item lunchItems[MAX_ITEMS] = {
        {"Burger", 150},
        {"Pizza", 250},
        {"Salad", 180},
        {"Sandwich", 120},
        {"Juice", 90}
    };

    struct Item dinnerItems[MAX_ITEMS] = {
        {"Pasta", 200},
        {"Steak", 350},
        {"Fish Curry", 300},
        {"Rice Bowl", 150},
        {"Soup", 120}
    };

    // Separate orders for Breakfast, Lunch, and Dinner
    struct Order breakfastOrders[MAX_ITEMS];
    int breakfastOrderCount = 0;

    struct Order lunchOrders[MAX_ITEMS];
    int lunchOrderCount = 0;

    struct Order dinnerOrders[MAX_ITEMS];
    int dinnerOrderCount = 0;

    int choice, mealChoice;

    while (1) {
        printf("\n1. Show Breakfast Menu\n2. Show Lunch Menu\n3. Show Dinner Menu\n4. Place Order\n5. View Bill\n6. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            // Clear invalid input
            while (getchar() != '\n');
            printf("Invalid input! Please enter a number between 1 and 6.\n");
            continue;
        }

        switch (choice) {
            case 1:
                showMenu(breakfastItems, 5, "Breakfast");
                break;
            case 2:
                showMenu(lunchItems, 5, "Lunch");
                break;
            case 3:
                showMenu(dinnerItems, 5, "Dinner");
                break;
            case 4:
                printf("\nSelect the meal type to order from:\n1. Breakfast\n2. Lunch\n3. Dinner\n");
                printf("Enter your choice: ");
                if (scanf("%d", &mealChoice) != 1) {
                    // Clear invalid input
                    while (getchar() != '\n');
                    printf("Invalid input! Please enter a number between 1 and 3.\n");
                    break;
                }

                switch (mealChoice) {
                    case BREAKFAST:
                        takeOrder(breakfastItems, 5, breakfastOrders, &breakfastOrderCount);
                        break;
                    case LUNCH:
                        takeOrder(lunchItems, 5, lunchOrders, &lunchOrderCount);
                        break;
                    case DINNER:
                        takeOrder(dinnerItems, 5, dinnerOrders, &dinnerOrderCount);
                        break;
                    default:
                        printf("Invalid meal choice! Please select between 1 and 3.\n");
                }
                break;
            case 5:
                viewBillMenu(breakfastItems, breakfastOrders, &breakfastOrderCount,
                            lunchItems, lunchOrders, &lunchOrderCount,
                            dinnerItems, dinnerOrders, &dinnerOrderCount,
                            customer);
                break;
            case 6:
                printf("Thank you for visiting the canteen!\n");
                return 0;
            default:
                printf("Invalid choice! Please enter a number between 1 and 6.\n");
        }
    }
    return 0;
}

// Function to display the menu based on meal category
void showMenu(struct Item items[], int size, const char* category) {
    printf("\n--- %s Menu ---\n", category);
    for (int i = 0; i < size; i++) {
        printf("%d. %s - Rs. %d\n", i + 1, items[i].name, items[i].price);
    }
}

// Function to take multiple orders
void takeOrder(struct Item items[], int size, struct Order orders[], int *orderCount) {
    int itemIndex, quantity;

    printf("\nEnter the item number you want to order (Enter 0 to finish):\n");

    while (1) {
        printf("Item number: ");
        if (scanf("%d", &itemIndex) != 1) {
            // Clear invalid input
            while (getchar() != '\n');
            printf("Invalid input! Please enter a valid item number.\n");
            continue;
        }

        if (itemIndex == 0) {
            break; // Exit loop if 0 is entered
        }

        if (itemIndex < 1 || itemIndex > size) {
            printf("Invalid item number! Please try again.\n");
            continue; // Skip invalid input
        }

        printf("Enter the quantity for %s: ", items[itemIndex - 1].name);
        if (scanf("%d", &quantity) != 1 || quantity <= 0) {
            // Clear invalid input
            while (getchar() != '\n');
            printf("Invalid quantity! Please enter a positive number.\n");
            continue;
        }

        // Check if the item is already ordered
        int found = 0;
        for (int i = 0; i < *orderCount; i++) {
            if (orders[i].itemIndex == (itemIndex - 1)) {
                orders[i].quantity += quantity;
                found = 1;
                break;
            }
        }

        if (!found) {
            // Add the item and quantity to the order
            if (*orderCount >= MAX_ITEMS) {
                printf("Maximum order limit reached for this meal!\n");
                break;
            }
            orders[*orderCount].itemIndex = itemIndex - 1;
            orders[*orderCount].quantity = quantity;
            (*orderCount)++;
        }

        printf("Item added! You can continue ordering or enter 0 to finish.\n");
    }

    printf("Your order has been placed!\n");
}

// Function to display the bill for a specific meal type
void showBill(struct Item items[], struct Order orders[], int orderCount, const char* category, struct Customer customer) {
    if (orderCount == 0) {
        printf("\nNo orders placed for %s.\n", category);
        return;
    }

    int total = 0;

    // Display Customer Details
    printf("\n--- %s Bill ---\n", category);
    printf("Customer Name: %s\n", customer.name);
    printf("Mobile Number: %s\n\n", customer.mobile);

    // Display Ordered Items
    for (int i = 0; i < orderCount; i++) {
        int index = orders[i].itemIndex;
        int quantity = orders[i].quantity;
        int price = items[index].price * quantity;

        printf("%s x %d = Rs. %d\n", items[index].name, quantity, price);
        total += price;
    }
    printf("\n%s Total: Rs. %d\n", category, total);
    printf("---------------------\n");
    printf("Total Amount for %s: Rs. %d\n", category, total);
}

// Function to handle the View Bill sub-menu
void viewBillMenu(struct Item breakfastItems[], struct Order breakfastOrders[], int *breakfastCount,
                 struct Item lunchItems[], struct Order lunchOrders[], int *lunchCount,
                 struct Item dinnerItems[], struct Order dinnerOrders[], int *dinnerCount,
                 struct Customer customer) {
    int billChoice;

    while (1) {
        printf("\n--- View Bill ---\n");
        printf("1. View Breakfast Bill\n2. View Lunch Bill\n3. View Dinner Bill\n4. Cancel\n");
        printf("Enter your choice: ");
        if (scanf("%d", &billChoice) != 1) {
            // Clear invalid input
            while (getchar() != '\n');
            printf("Invalid input! Please enter a number between 1 and 4.\n");
            continue;
        }

        switch (billChoice) {
            case 1:
                showBill(breakfastItems, breakfastOrders, *breakfastCount, "Breakfast", customer);
                // Clear Breakfast orders after billing
                *breakfastCount = 0;
                break;
            case 2:
                showBill(lunchItems, lunchOrders, *lunchCount, "Lunch", customer);
                // Clear Lunch orders after billing
                *lunchCount = 0;
                break;
            case 3:
                showBill(dinnerItems, dinnerOrders, *dinnerCount, "Dinner", customer);
                // Clear Dinner orders after billing
                *dinnerCount = 0;
                break;
            case 4:
                printf("Returning to main menu.\n");
                return;
            default:
                printf("Invalid choice! Please enter a number between 1 and 4.\n");
        }
    }
}
