/*********************************************************************************************************  
This is to certify that this project is my own work, based on my personal efforts in studying and applying 
the concepts learned. I have constructed the functions and their respective algorithms and corresponding 
code by myself.  The program was run, tested, and debugged by my own efforts.  I further certify that I 
have not copied in part or whole or otherwise plagiarized the work of other students and/or persons. 

                                                                 Olivares, Marc Yuri G., DLSU ID# 12264131 
*********************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef char string10[11]; // string of 10 characters
typedef char string11[12]; // string of 11 characters
typedef char string15[16]; // string of 15 characters
typedef char string20[21]; // string of 20 characters
typedef char string30[31]; // string of 30 characters

typedef struct itemTag itemType;
typedef struct userTag userType;
typedef struct dateTag dateType;
typedef struct transactionTag transactionType;

struct itemTag 
{
    int       productID;
    string20  itemName;
    string15  category;
    string30  description;
    int       quantity;
    float     price;
    int       sellerID; // same as userID
};

struct userTag
{
    int      userID;
    string10 password;
    string30 address;
    string11 number;
    string20 name;
    itemType items[20];
    itemType cart[10];
};

struct dateTag
{
    int month,
        day,
        year;
};

struct transactionTag
{
    dateType date;
    itemType bought[5];
    int      buyerID,
             sellerID;
    float    total;
};

/* This function is used to take a string input from the user that includes spaces.
It will take a string input until it encounters a newline character '\n'
@param *str is the pointer to the variable of the string input
*/
void 
getString (char * str) 
{
  int i = 0;
  char ch;
  
  do {
    scanf("%c", &ch);

    if (ch != '\n') {
      str[i] = ch;
      i++;
    }
  } while (ch != '\n');

  str[i] = '\0';
}

/* This function registers a user into the system and is added into the user[] array.
@param user[] is the array of users
@param nUsers is the number of users in the user[] array
*/
void 
registerUser (userType user[], 
              int nUsers)
{
    char cDump;
    int i = 0;
    int tempID; // temporary variable for userID input
    int found = 0; // flag used to check if tempID has a matching userID in the user[] array.

    printf("\nREGISTER\n");

    // check if userID is unique
    do 
    {
        printf("User ID: ");
        scanf("%d%c", &tempID, &cDump);

        for (i = 0; i < nUsers; i++) 
        {
            if (tempID == user[i].userID) // if ID is taken
            { 
                printf("\nUser ID taken.\n");
                found = 1;
            }
        }

        // if ID is unique
        if (!found) 
        {
            user[nUsers].userID = tempID;

            printf("Password: ");
            getString(user[nUsers].password);
            
            printf("Address: ");
            getString(user[nUsers].address);

            printf("Number: ");
            getString(user[nUsers].number);

            printf("Name: ");
            getString(user[nUsers].name);

            printf("\nSuccessfully registered %s ID %d\n", user[nUsers].name, user[nUsers].userID);
        }
    } while (found); // keeps asking for ID until unique ID is given
}

/* This function is used to login as a user. It asks for the userID and password,
then checks if it matches any registered user in the user[] array.
@param user[] is the array of users
@param nUsers is the number of users in the user[] array
@return userID if found
@return -1 if not
*/
int 
loginUser (userType user[], 
           int nUsers)
{
    char cDump;
    int id = -1;
    int logID = -1; // ID used in login
    string10 logPw; // Password used in login
    int found = 0; // flag used to check if logID has a matching userID in the user[] array.
    int i = 0;

    printf("\nLOGIN\n");

    printf("\nUser ID: ");
    scanf("%d%c", &logID, &cDump);
    
    printf("Password: ");
    getString(logPw);

    // check if logID has a matching userID in the user[] array
    while (found == 0 && i <= nUsers) 
    {
        if (logID == user[i].userID && strcmp(logPw, user[i].password) == 0) 
        {
            id = logID;
            found = 1;
        }
        i++;
    }

    return id;
}

/* This function is used to find the user in tbe user[] array
that matches the inputted userID.
@param user[] is the array of users
@param nUsers is the number of users in the user[] array
@param curID is the inputted userID being searched
@return index of the user in the user[] array if found
@return -1 if a match is not found
*/
int 
findUser (userType user[], 
          int nUsers, 
          int curID) 
{
    int i = 0;
    int found = 0; // flag used to check for ID being searched in the user[] array
    int curUser = -1; // index of user in user[]

    // check for ID being searched in the user[] array
    while (!found && i < nUsers) 
    {
        if (curID == user[i].userID) 
        {
            curUser = i;
            found = 1;
        }
        i++;
    }

    return curUser;
}

/* This function counts the items that a user is selling.
@param user[] is the array of users
@param curUser is the current user whose number of items is being counted
@return the number of items the user has
*/
int 
countItems (userType user[], 
            int curUser) 
{
    int i;
    int nItems = 0; // number of items

    // check item array for matching userID
    for (i = 0; i < 20; i++) 
    {
        if (user[curUser].items[i].sellerID == user[curUser].userID) 
            nItems++; // if matching, increment nItems by one
    }

    return nItems;
}

/* This function is for adding an item to the user's item[] array.
It doesn't allow adding when the number of items is already 20.
It takes in input for the item's details.
@param user[] is the array of users
@param nUsers is the number of users in the user[] array
@param curID is the userID of the registered user
*/
void 
sellAddItem (userType user[], 
             int nUsers, 
             int curID) 
{
    char cDump; 
    int curUser = findUser(user, nUsers, curID); // current user
    int nItems = countItems(user, curUser); // number of items of current user
    int tempProductID; // variable taking productID, is checked first if it's available
    int i, j;
    int IDavailable; // flag to check if inputted productID is available
    int nTempItems; // used in checking productID availability to iterate over products being checked
    
    printf("\nNumber of items: %d\n", nItems);

    if (nItems < 20) 
    {
        printf("\nINPUT ITEM DETAILS\n");

        // product ID, should be unique regardless of seller
        printf("Product ID: ");
        scanf("%d%c", &tempProductID, &cDump);

        i = 0;
        IDavailable = 1;

        // see if product ID is available
        for (i = 0; i < nUsers; i++) 
        {
            nTempItems = countItems(user, i);

            for (j = 0; j < nTempItems; j++) 
            {
                if (tempProductID == user[i].items[j].productID)
                    IDavailable = 0;
            }
        }

        // only ask for other details if productID is available
        if (IDavailable) 
        {
            user[curUser].items[nItems].productID = tempProductID;

            printf("Item name: ");
            getString(user[curUser].items[nItems].itemName);

            printf("Category: ");
            getString(user[curUser].items[nItems].category);

            printf("Description: ");
            getString(user[curUser].items[nItems].description);

            printf("Quantity: ");
            scanf("%d", &user[curUser].items[nItems].quantity);

            printf("Price: ");
            scanf("%f%c", &user[curUser].items[nItems].price, &cDump);

            user[curUser].items[nItems].sellerID = curID;
        } else
            printf("\nProduct ID not available\n");
    }
}

/* This function swaps two items in an item[] array.
@param *item1 is the pointer to the first item
@param *item2 is the pointer to the second item
*/
void 
swapItems(itemType *item1, 
          itemType *item2) 
{
  itemType temp;

  temp = *item1;
  *item1 = *item2;
  *item2 = temp;
}

/* This function shows all of the items being sold by the registered user.
It is presented in table format, and sorted in increasing order based on productID.
Numbers are right justified while strings are left justified.
@param user[] is the array of users
@param nUsers is the number of users in the user[] array
@param curID is the userID of the registered user
*/
void 
sellShowMyProducts (userType user[], 
                    int nUsers, 
                    int curID)
{
    int curUser = findUser(user, nUsers, curID); // index of user in user[] array
    int nItems = countItems(user, curUser); // number of items curUser has
    int i, j, min;

    // sort products based on productID in ascending order
    for (i = 0; i < nItems - 1; i++) 
    {
        min = i;
        for (j = i + 1; j < nItems; j++) 
        {
            if (user[curUser].items[j].productID < user[curUser].items[min].productID) 
            {
                min = j;
            }
        }
        swapItems(&user[curUser].items[min], &user[curUser].items[i]);
    }

    // display products in table format
    for (i = 0; i < nItems; i++) 
    {
        printf("\n%-10s - ", "Product ID"); printf("%20d", user[curUser].items[i].productID);
        printf("\n%-10s - ", "Item Name");  printf("%s", user[curUser].items[i].itemName);
        printf("\n%-10s - ", "Category");   printf("%s", user[curUser].items[i].category);
        printf("\n%-10s - ", "Price");      printf("%20.2f", user[curUser].items[i].price);
        printf("\n%-10s - ", "Quantity");   printf("%20d\n", user[curUser].items[i].quantity);
    }
}

/* This function is used to edit the stock of items of the registered user.
It allows adding to the quantity and changing the price, name, category, and description
of the product selected by productID.
@param user[] is the array of users
@param nUsers is the number of users in the user[] array
@param curID is the userID of the registered user
*/
void 
sellEditStock (userType user[], 
               int nUsers, 
               int curID) 
{
    char cDump; 
    int editID; // productID of item to be edited
    int editStockRunning; // flag used to check if Edit Stock Menu is still running
    int editStockChoice; // Edit Stock Menu choice
    int curUser = findUser(user, nUsers, curID); // index of user in user[] array
    int nItems = countItems(user, curUser); // number of items curUser has
    int replenish; // number to be added to item quantity
    float newPrice; // new price of item
    string20 newName; // new name of item
    string15 newCategory; // new category of item
    string30 newDescription; // new description of item
    int found = 0; // flag used to check if item was found in items[]
    int curItem; // index of item being edited in items[] array
    int i = 0;

    sellShowMyProducts(user, nUsers, curID);

    printf("\nProduct ID of item to be edited: ");
    scanf("%d", &editID);

    // search for item in items[] with matching product ID
    while (!found && i < nItems) 
    {
        if (editID == user[curUser].items[i].productID) 
        {
            curItem = i;
            found = 1;
        } else {
            i++;
        }
    }

    // only go into Edit Stock menu if product is found
    if (!found) 
    {
        printf("\nError: Product ID not found. Redirecting back to Sell Menu.\n");
    } else 
    {
        editStockRunning = 1;

        while (editStockRunning) 
        {
            printf("\nEDIT STOCK MENU\n");
            printf("\n[1] Replenish\n");
            printf("[2] Change Price\n");
            printf("[3] Change Item Name\n");
            printf("[4] Change Category\n");
            printf("[5] Change Description\n");
            printf("[6] Finish Editing\n");

            printf("\nEdit Stock Menu Choice: ");
            scanf("%d%c", &editStockChoice, &cDump);

            switch (editStockChoice) 
            {
                case 1: // replenish
                    printf("\nInput the quantity to be ADDED to existing quantity of item: ");
                    scanf("%d%c", &replenish, &cDump);

                    user[curUser].items[curItem].quantity = user[curUser].items[curItem].quantity + replenish;
                    break;
                case 2: // change price
                    printf("\nInput new unit price of item: ");
                    scanf("%f%c", &newPrice, &cDump);
                    
                    user[curUser].items[curItem].price = newPrice;
                    break;
                case 3: // change item name
                    printf("\nInput new item name: ");
                    getString(newName);

                    strcpy(user[curUser].items[curItem].itemName, newName);
                    break;
                case 4: // change category
                    printf("\nInput new category: ");
                    getString(newCategory);

                    strcpy(user[curUser].items[curItem].category, newCategory);
                    break;
                case 5: // change description
                    printf("\nInput new item description: ");
                    getString(newDescription);

                    strcpy(user[curUser].items[curItem].description, newDescription);
                    break;
                case 6: // finish editing
                    editStockRunning = 0;
                    break;
                default:
                    printf("\nInvalid input. Try again.\n");
            }
        }
    }
}

/* This function shows the items being sold by the registered user that
has a quantity less than 5. It shows these products one by one.
The user can press [N] to go to the next product.
The user can press [X] to exit the view.
@param user[] is the array of users
@param nUsers is the number of users in the user[] array
@param curID is the userID of the registered user
*/
void 
sellShowLowStock (userType user[], 
                  int nUsers, 
                  int curID) 
{
    char cDump; 
    int curUser = findUser(user, nUsers, curID); // index of user in user[] array
    int nItems = countItems(user, curUser); // number of items user has
    int i = 0;
    int lowStockRunning = 1; // flag used to check if Low Stock Menu is still running
    char lowStockChoice = 'a'; // Show Low Stock Menu
    int valid = 0; // flag used to check if control choice is valid

    while (i < nItems && lowStockRunning) 
    {
        valid = 0;
        lowStockChoice = 'b';
        if (user[curUser].items[i].quantity < 5) 
        {
            printf("\n%-11s - ", "Product ID");  printf("%20d", user[curUser].items[i].productID);
            printf("\n%-11s - ", "Item Name");   printf("%s", user[curUser].items[i].itemName);
            printf("\n%-11s - ", "Category");    printf("%s", user[curUser].items[i].category);
            printf("\n%-11s - ", "Price");       printf("%20.2f", user[curUser].items[i].price);
            printf("\n%-11s - ", "Quantity");    printf("%20d", user[curUser].items[i].quantity);
            printf("\n%-11s - ", "Description"); printf("%s", user[curUser].items[i].description);

            while (!valid) 
            {
                valid = 0;
                lowStockChoice = 'c';
                printf("\n\n[N] Next product");
                printf("\n[X] Exit");
                printf("\nInput: ");
                scanf("%c%c", &lowStockChoice, &cDump);

                if (lowStockChoice == 'N' || lowStockChoice == 'n') 
                {
                    i++;
                    valid = 1;
                } else if (lowStockChoice == 'X' || lowStockChoice == 'x') 
                {
                    lowStockRunning = 0;
                    valid = 1;
                } else
                    printf("\nInvalid input. Try again.\n");
            }
        } else
            i++;

        if (i >= nItems)
            printf("\nNo more items to show.\n");
    }
}

/* This function swaps two users in a user[] array.
@param *user1 is the pointer to the first user
@param *user2 is the pointer to the second user
*/
void 
swapUsers (userType *user1, 
           userType *user2) 
{
  userType temp;

  temp = *user1;
  *user1 = *user2;
  *user2 = temp;
}

/* This function displays all of the products available for sale, one seller at a time.
It is sorted in increasing order based on SellerID.
The user can press [N] to go to the next seller.
The user can press [X] to exit the view.
@param user[] is the array of users
@param nUsers is the number of users in the user[] array
*/
void
buyViewAll (userType user[], 
            int nUsers) 
{
    char cDump; 
    char buyViewRunning = 1; // flag used to check if View All menu is running
    char buyViewChoice = 'a'; // View All menu choice
    int i, j, min;
    int valid = 0; // flag used to check if control choice is valid
    int nItems; // number of items user has

    // sort users in increasing order of sellerID
    for (i = 0; i < nUsers - 1; i++) 
    {
        min = i;
        for (j = i + 1; j < nUsers; j++) 
        {
            if (user[j].userID < user[min].userID)
                min = j;
        }
        swapUsers(&user[min], &user[i]);
    }

    i = 0;
    while (buyViewRunning && i < nUsers) 
    {
        nItems = countItems(user, i);
        if (nItems > 0) 
        {
            printf("\n%-10s - ", "SELLER ID"); printf("%20d", user[i].userID);
            sellShowMyProducts(user, nUsers, user[i].userID);

            do 
            {
                valid = 0;
                buyViewChoice = 'c';
                printf("\n[N] Next seller");
                printf("\n[X] Exit");
                printf("\nInput: ");
                scanf("%c%c", &buyViewChoice, &cDump);

                if (buyViewChoice == 'N' || buyViewChoice == 'n') 
                {
                    valid = 1;
                } else if (buyViewChoice == 'X' || buyViewChoice == 'x') 
                {
                    buyViewRunning = 0;
                    valid = 1;
                } else 
                {
                    printf("\nInvalid input. Try again.\n");
                }
            } while (!valid);
        }

        if (i >= nUsers - 1) 
        {
            printf("\nNo more users to show.\n");
            buyViewRunning = 0;
        }
        i++;
    }
}

/* This function takes in an int sellerID input then displays all of the products being sold by that seller.
It is presented in table format, and sorted in increasing order based on productID.
Numbers are right justified while strings are left justified.
@param user[] is the array of users
@param nUsers is the number of users in the user[] array
*/
void 
buyShowSellerAll (userType user[], 
                  int nUsers) 
{
    char cDump; 
    int showID; // sellerID inputted
    int curSeller; // index of seller in user[]

    printf("\nSeller ID: ");
    scanf("%d%c", &showID, &cDump);
    curSeller = findUser(user, nUsers, showID);

    // display if user with inputted sellerID exists
    if (curSeller != -1) 
    {
        printf("\n%-10s - ", "Seller ID"); printf("%20d", user[curSeller].userID);
        sellShowMyProducts(user, nUsers, showID);
        printf("\n");
    } else
        printf("\nInvalid input. Redirecting back to Buy Menu.\n");
}

/* This function takes in a category string input, then displays all of the products that
fit the inputted category.
The user can press [N] to go to the next product.
The user can press [X] to exit the view.
@param user[] is the array of users
@param nUsers is the number of users in the user[] array
*/
void 
buyCategorySearch (userType user[], 
                   int nUsers) 
{
    char cDump; 
    string15 cateSearch; // category being searched
    string15 lowerSearch; // lowercase cateSearch
    string15 lowerCate; // lowercase current category in iteration
    int csRunning = 1; // flag used to check if Category Search is running
    char csChoice = 'a'; // category search control choice
    int valid; // flag used to check if control choice is valid
    int same; // flag used to check if 
    int i, j, k;
    int nItems; // number of items user has

    printf("\nCategory: ");
    getString(cateSearch);

    i = 0;
    while (i < nUsers && csRunning) 
    {
        valid = 0;
        csChoice = 'b';
        nItems = countItems(user, i);

        j = 0; 
        while (j < nItems && csRunning) 
        {
            valid = 0;

            // turns cateSearch to all lowercase
            for (k = 0; k < strlen(cateSearch); k++) 
            {
                lowerSearch[k] = tolower(cateSearch[k]);
            }

            // turns current category to all lowercase
            for (k = 0; k < strlen(user[i].items[j].category); k++) 
            {
                lowerCate[k] = tolower(user[i].items[j].category[k]);
            }

            // if category being searched and current category are the same
            same = strcmp(lowerCate, lowerSearch);
            if (same == 0) 
            {
                printf("\n%-11s - ", "Product ID");  printf("%20d", user[i].items[j].productID);
                printf("\n%-11s - ", "Item Name");   printf("%s", user[i].items[j].itemName);
                printf("\n%-11s - ", "Price");       printf("%20.2f", user[i].items[j].price);
                printf("\n%-11s - ", "Quantity");    printf("%20d", user[i].items[j].quantity);
                printf("\n%-11s - ", "Description"); printf("%s", user[i].items[j].description);

                do 
                {
                    csChoice = 'c';
                    printf("\n\n[N] Next product");
                    printf("\n[X] Exit");
                    printf("\nInput: ");
                    scanf("%c%c", &csChoice, &cDump);

                    if (csChoice == 'N' || csChoice == 'n') 
                    {
                        valid = 1;
                    } else if (csChoice == 'X' || csChoice == 'x') 
                    {
                        valid = 1;
                        csRunning = 0;
                    } else
                        printf("\nInvalid input. Try again.\n");
                } while (!valid);
            }
            j++;
        }
        i++;
    }
}

/* This function takes in a name string input, then displays all of the products that
contain that substring in its name.
The user can press [N] to go to the next product.
The user can press [X] to exit the view.
@param user[] is the array of users
@param nUsers is the number of users in the user[] array
*/
void 
buyNameSearch (userType user[], 
               int nUsers) 
{
    char cDump;
    string20 searchName; // name being searched
    string20 lowerSearch; // lowercase searchName
    string20 lowerName; // lowercase current name in iteration
    int i, j, k;
    int nItems; // number of items user has
    char *pch; // pointer variable used to check if strstr was successful
    int nsRunning = 1; // flag used to check if Name Search is still running
    char nsChoice = 'a'; // Name Search choice
    int validChoice = 0; // flag used to check if choice is valid

    printf("\nProduct keywords: ");
    getString(searchName);

    // substring search
    i = 0;
    while (i < nUsers && nsRunning) 
    {
        nItems = countItems(user, i);
        j = 0;
        while (j < nItems && nsRunning) 
        {
            for (k = 0; k < strlen(searchName); k++) 
                lowerSearch[k] = tolower(searchName[k]);

            for (k = 0; k < strlen(user[i].items[j].itemName); k++) 
                lowerName[k] = tolower(user[i].items[j].itemName[k]);

            pch = strstr(lowerName, lowerSearch);
            if (pch != NULL) 
            {
                printf("\n%-11s - ", "Product ID");  printf("%20d", user[i].items[j].productID);
                printf("\n%-11s - ", "Item Name");   printf("%s", user[i].items[j].itemName);
                printf("\n%-11s - ", "Category");    printf("%s", user[i].items[j].category);
                printf("\n%-11s - ", "Price");       printf("%20.2f", user[i].items[j].price);
                printf("\n%-11s - ", "Quantity");    printf("%20d", user[i].items[j].quantity);
                printf("\n%-11s - ", "Description"); printf("%s", user[i].items[j].description);

                do 
                {
                    nsChoice = 'b';
                    printf("\n\n[N] Next product");
                    printf("\n[X] Exit");
                    printf("\nInput: ");
                    scanf("%c%c", &nsChoice, &cDump);

                    if (nsChoice == 'N' || nsChoice == 'n') 
                    {
                        validChoice = 1;
                    } else if (nsChoice == 'X' || nsChoice == 'x') 
                    {
                        validChoice = 1;
                        nsRunning = 0;
                    } else 
                    {
                        printf("\nInvalid input. Try again.\n");
                    }
                } while (!validChoice);
            }
            j++;
        }
        i++;
    }
}

/* This function counts the number of items in user[curUser]'s cart.
@param user[] is the array of users
@param curUser is the user whose number of items in cart is being counted
@return the number of items in the user's cart
*/
int 
countCart (userType user[], 
           int nUsers,
           int curUser) 
{
    int i, j;
    int nCart = 0; // number of items

    // increment nCart by 1 if current cart item sellerID is equal to userID
    for (i = 0; i < 10; i++) {
        for (j = 0; j < nUsers; j++) {
            if (user[curUser].cart[i].sellerID == user[j].userID) {
                nCart++;
            }
        }
    }

    return nCart;
}

/* This function asks the user for the productID of the product to be added to cart, and the quantity.
It makes sure that the user has less than 10 items in their cart, and that the product has available quantity.
@param user[] is the array of users
@param nUsers is the number of users in the user[] array
@param curID is the userID of the current user
*/
void 
buyAddToCart (userType user[], 
              int nUsers, 
              int curID) 
{
    int searchID; // product ID being searched
    int buyQuan; // quantity of items to be added to cart
    int own = 0; // flag used to check if product is sold by current buyer
    int i, j;
    int nItems; // number of items user has
    int curUser = findUser(user, nUsers, curID); // index of current user in user[]
    int nCart = countCart(user, nUsers, curUser); // number of items in curUser's cart

    if (nCart > 9) {
        // suggest Edit Cart or Check Out first before adding more
        printf("\nError: Your cart already has 10 items and is already full.\n");
        printf("EDIT CART or CHECK OUT first before adding more items.\n");
    } else {
        // input productID
        printf("\nInput product ID: ");
        scanf("%d", &searchID);

        // check if buyer isn't buying their own product
        i = 0;
        while (i < nUsers && !own) {
            nItems = countItems(user, i);
            j = 0;
            while (j < nItems && !own) {
                if (searchID == user[i].items[j].productID) {
                    if (user[curUser].userID == user[i].userID) {
                        printf("\nError: Sellers cannot buy their own products.\n");
                        own = 1;
                    }
                }
                j++;
            }
            i++;
        }

        // only continue if buying is valid
        if (!own) {
            // input quantity
            printf("Quantity: ");
            scanf("%d", &buyQuan);
        
            for (i = 0; i < nUsers; i++) {
                nItems = countItems(user, i);
                for (j = 0; j < nItems; j++) {
                    if (searchID == user[i].items[j].productID) {
                        // check if quantity available
                        if (buyQuan > user[i].items[j].quantity)
                            printf("\nError: Not enough stock available\n");
                        else // add to cart
                            user[curUser].cart[nCart] = user[i].items[j];
                            user[curUser].cart[nCart].quantity = buyQuan;
                            nCart++;
                            
                            printf("\nAdded %d %s to cart successfully.", user[curUser].cart[nCart - 1].quantity, user[curUser].cart[nCart - 1].itemName);
                            printf("\nItems in cart: %d\n", nCart);
                    }
                }
            }
        }
    }
}

/* This function displays all the items in the user's cart, then presents options for editing, namely,
removing all items by a specific seller, removing a specific item, and editing an item's quantity.
@param user[] is the array of users
@param nUsers is the number of users in the user[] array
@param curID is the userID of the current user
*/
void 
buyEditCart (userType user[], 
             int nUsers, 
             int curID) 
{
    int editCartRunning = 1; // flag used to check if Edit Cart Menu is still running
    int editCartChoice; // the user's Edit Cart Menu choice
    int curUser = findUser(user, nUsers, curID); // index of current user in user[]
    int nCart = countCart(user, nUsers, curUser); // number of items in curUser's cart
    int removeSeller; // sellerID whose items are to be removed
    int removeProduct; // productID of item to be removed
    int editProduct; //productID of item to be edited
    int quantity; // new quantity
    int found; // flag used to check if product is found
    int i, j, min;

    // sort products based on productID in ascending order
    if (nCart > 1) {
        for (i = 0; i < nCart - 1; i++) {
            min = i;
            for (j = i + 1; j < nCart; j++) {
                if (user[curUser].cart[j].productID < user[curUser].cart[min].productID) {
                    min = j;
                }
            }
            swapItems(&user[curUser].cart[min], &user[curUser].cart[i]);
        }
    }

    // display products in table format
    for (i = 0; i < nCart; i++) {
        printf("\n%-10s - ", "Product ID"); printf("%20d", user[curUser].cart[i].productID);
        printf("\n%-10s - ", "Item Name");  printf("%s", user[curUser].cart[i].itemName);
        printf("\n%-10s - ", "Category");   printf("%s", user[curUser].cart[i].category);
        printf("\n%-10s - ", "Price");      printf("%20.2f", user[curUser].cart[i].price);
        printf("\n%-10s - ", "Quantity");   printf("%20d", user[curUser].cart[i].quantity);
        printf("\n%-10s - ", "Seller ID");   printf("%20d\n", user[curUser].cart[i].sellerID);
    }

    while (editCartRunning) {
        printf("\nEDIT CART MENU\n");
        printf("\n[1] Remove All Items from Seller\n");
        printf("[2] Remove Specific Item\n");
        printf("[3] Edit Quantity\n");
        printf("[4] Finish Edit Cart\n");
        printf("\nEdit Cart Menu Choice: ");
        scanf("%d", &editCartChoice);

        switch (editCartChoice) {
            case 1: // Remove All Items from Seller
                // input seller ID
                printf("\nSeller ID: ");
                scanf("%d", &removeSeller);

                // delete all items in the cart of this seller
                for (i = nCart - 1; i > -1; i--) {
                    if (user[curUser].cart[i].sellerID == removeSeller) {
                        for (j = i; j < nCart; j++) {
                            printf("\nItems from Seller %d removed.\n", removeSeller);
                            user[curUser].cart[j] = user[curUser].cart[j + 1];
                        }
                        nCart--;
                    }
                }

                for (i = 0; i < nCart; i++) {
                    printf("\n%-10s - ", "Product ID"); printf("%20d", user[curUser].cart[i].productID);
                    printf("\n%-10s - ", "Item Name");  printf("%s", user[curUser].cart[i].itemName);
                    printf("\n%-10s - ", "Category");   printf("%s", user[curUser].cart[i].category);
                    printf("\n%-10s - ", "Price");      printf("%20.2f", user[curUser].cart[i].price);
                    printf("\n%-10s - ", "Quantity");   printf("%20d", user[curUser].cart[i].quantity);
                    printf("\n%-10s - ", "Seller ID");   printf("%20d\n", user[curUser].cart[i].sellerID);
                }
                break;
            case 2: // Remove Specific Item
                // input product ID
                printf("\nProduct ID: ");
                scanf("%d", &removeProduct);

                // item will be removed
                for (i = nCart - 1; i > -1; i--) {
                    if (user[curUser].cart[i].productID == removeProduct) {
                        for (j = i; j < nCart; j++) {
                            printf("\nProduct %d removed.\n", removeProduct);
                            user[curUser].cart[j] = user[curUser].cart[j + 1];
                        }
                        nCart--;
                    }
                }

                for (i = 0; i < nCart; i++) {
                    printf("\n%-10s - ", "Product ID"); printf("%20d", user[curUser].cart[i].productID);
                    printf("\n%-10s - ", "Item Name");  printf("%s", user[curUser].cart[i].itemName);
                    printf("\n%-10s - ", "Category");   printf("%s", user[curUser].cart[i].category);
                    printf("\n%-10s - ", "Price");      printf("%20.2f", user[curUser].cart[i].price);
                    printf("\n%-10s - ", "Quantity");   printf("%20d", user[curUser].cart[i].quantity);
                    printf("\n%-10s - ", "Seller ID");   printf("%20d\n", user[curUser].cart[i].sellerID);
                }
                break;
            case 3: // Edit Quantity
                // input product ID
                found = 0;
                i = 0;
                printf("\nProduct ID: ");
                scanf("%d", &editProduct);

                // find product and change quantity
                while (!found && i < nCart) {
                    if (user[curUser].cart[i].productID == editProduct) {
                        found = 1;
                        printf("New quantity: ");
                        scanf("%d", &quantity);
                        user[curUser].cart[i].quantity = quantity;
                    }
                    i++;
                }

                if (i == nCart && !found)
                    printf("\nProduct ID not found.\n");

                for (i = 0; i < nCart; i++) {
                    printf("\n%-10s - ", "Product ID"); printf("%20d", user[curUser].cart[i].productID);
                    printf("\n%-10s - ", "Item Name");  printf("%s", user[curUser].cart[i].itemName);
                    printf("\n%-10s - ", "Category");   printf("%s", user[curUser].cart[i].category);
                    printf("\n%-10s - ", "Price");      printf("%20.2f", user[curUser].cart[i].price);
                    printf("\n%-10s - ", "Quantity");   printf("%20d", user[curUser].cart[i].quantity);
                    printf("\n%-10s - ", "Seller ID");   printf("%20d\n", user[curUser].cart[i].sellerID);
                }
                break;
            case 4: // Finish Edit Cart
                editCartRunning = 0;
                break;
            default:
                printf("\nInvalid input. Try again.\n");
        }
    }
}

/* This function counts the number of transactions a user has.
@param transaction[] is the array of transactions
@param user[] is the array of users
@param curUser is the current user whose number of transactions is being counted
*/
int 
countTransactions (transactionType transaction[], 
                   userType user[], 
                   int curUser) 
{
    int i;
    int nTransactions = 0; // number of transactions

    // increment nTransactions if userID is equal to current transaction's buyerID
    for (i = 0; i < 10; i++) {
        if (user[curUser].userID == transaction[i].buyerID) {
            nTransactions++;
        }
    }

    return nTransactions;
}

/* This function counts the number of items in a specific transaction.
@param transaction[] is the array of transactions
@param curTran is the current transaction whose number of items is being counted
*/
int 
countTransactionItems (transactionType transaction[], 
                       int curTran) 
{
    int i;
    int nTransactionItems = 0; // number of items in transaction

    // increment nTransactionItems if transaction sellerID is equal to item sellerID
    for (i = 0; i < 5; i++) {
        if (transaction[curTran].sellerID == transaction[curTran].bought[i].sellerID)
            nTransactionItems++;
    }

    return nTransactionItems;
}

/* This function handles the check out menu. It checks if the items in the user's cart had changes in price or
if it's not available anymore. It allows checking out of all items in the cart, items by a specific seller,
and a specific item. It displays a receipt of each transaction, and saves it in Transactions.dat.
@param user[] is the array of users
@param nUsers is the number of users in the user[] array
@param curID is the userID of the current user
*/
void 
buyCheckOut (userType user[], 
             int nUsers, 
             int curID) 
{
    int checkOutRunning; // flag used to check if Check Out Menu is still running
    int checkOutChoice; // the user's Check Out Menu choice
    dateType today; // date today
    int i, j, k, l;
    int curUser = findUser(user, nUsers, curID); // index of current user in user[]
    int nCart = countCart(user, nUsers, curUser); // number of items in curUser's cart
    string20 sellerName; // name of seller in transaction
    checkOutRunning = 1; // flag used to check if Check Out is running
    transactionType transaction[10]; // array of transactions
    int nTransactions; // number of transactions user has
    int nTransactionItems; // number of items in transaction
    int nItems; // number of items user has
    int sellerID; // sellerID being searched
    int productID; // productID being searched
    int itemFound; // flag used to check if item is found
    int transactionFound; // flag used to check if transaction is found
    FILE *transactionFile; // file pointer for Transactions.dat

    // input date
    printf("\nMonth (MM): ");
    scanf("%d", &today.month);
    printf("Day (DD): ");
    scanf("%d", &today.day);
    printf("Year (YYYY): ");
    scanf("%d", &today.year);

    // check if quantity or price changed
    for (i = 0; i < nCart; i++) {
        for (j = 0; j < nUsers; j++) {
            int nItems = countItems(user, j);
            for (k = 0; k < nItems; k++) {
                if (user[curUser].cart[i].productID == user[j].items[k].productID) {
                    if (user[curUser].cart[i].quantity > user[j].items[k].quantity) {
                        printf("\nSeller's quantity of %s is insufficient.\n", user[j].items[k].itemName);
                        printf("Quantity of %s in cart will now be reduced from %d to %d (stock seller has left).\n", 
                            user[curUser].cart[i].itemName, user[curUser].cart[i].quantity, user[j].items[k].quantity);

                        user[curUser].cart[i].quantity = user[j].items[k].quantity;
                        printf("\nREMINDER: You can still go to EDIT CART before Checking Out.\n");
                    } else if (user[curUser].cart[i].price != user[j].items[k].price) {
                        printf("\nSeller's price of %s has changed.\n", user[j].items[k].itemName);
                        printf("%s now costs %.2f (Old price: %.2f).\n", user[j].items[k].itemName, user[j].items[k].price, 
                            user[curUser].cart[i].price);

                        user[curUser].cart[i].price = user[j].items[k].price;
                        printf("\nREMINDER: You can still go to EDIT CART before Checking Out.\n");
                    }
                }
            }
        }
    }

    // initialize transactions
    for (i = 0; i < 10; i++) {
        transaction[i].date = today;
        transaction[i].buyerID = -1;
        transaction[i].sellerID = -1;
        transaction[i].total = 0;
    }

    while (checkOutRunning) {
        printf("\nCHECK OUT MENU\n");
        printf("\n[1] All\n");
        printf("[2] By a Specific Seller\n");
        printf("[3] Specific Item\n");
        printf("[4] Exit Check Out\n");

        printf("\nCheck Out Menu Choice: ");
        scanf("%d", &checkOutChoice);

        switch (checkOutChoice) {
            case 1: // All
                if (nCart > 0) {
                    // assign items in cart to transactions by seller
                    for (i = 0; i < nCart; i++) {
                        j = 0;
                        nTransactions = countTransactions(transaction, user, curUser);
                        transactionFound = 0;
                        while (j <= nTransactions && !transactionFound) {
                            nTransactionItems = countTransactionItems(transaction, j);
                            if ((transaction[j].sellerID == -1 || transaction[j].sellerID == user[curUser].cart[i].sellerID) && nTransactionItems < 5) {
                                transaction[j].bought[nTransactionItems] = user[curUser].cart[i];
                                transaction[j].buyerID = curID;
                                transaction[j].sellerID = user[curUser].cart[i].sellerID;
                                transaction[j].total += (user[curUser].cart[i].price * user[curUser].cart[i].quantity);
                                transactionFound = 1;
                            }
                        }
                    }

                    // display summary of transaction
                    nTransactions = countTransactions(transaction, user, curUser);
                    for (i = 0; i < nTransactions; i++) {
                        printf("------------------------------------\n");
                        printf("%23s", "TRANSACTION ");
                        printf("%d\n", i + 1);
                        nTransactionItems = countTransactionItems(transaction, i);
                        for (j = 0; j < nTransactionItems; j++) {
                            printf("\n%-13s - ", "Quantity");    printf("%20d", transaction[i].bought[j].quantity);
                            printf("\n%-13s - ", "Product ID");  printf("%20d", transaction[i].bought[j].productID);
                            printf("\n%-13s - ", "Item Name");   printf("%s", transaction[i].bought[j].itemName);
                            printf("\n%-13s - ", "Unit Price");  printf("%20.2f", transaction[i].bought[j].price);
                            printf("\n%-13s - ", "Total Price"); printf("%20.2f\n", transaction[i].bought[j].price * transaction[i].bought[j].quantity);

                            // add to binary file Transactions.dat
                            transactionFile = fopen("Transactions.dat", "ab");
                            
                            if (transactionFile != NULL) {
                                // write the transaction and its items to file
                                fwrite(&transaction[i], sizeof(transactionType), 1, transactionFile);
                            }

                            fclose(transactionFile);
                            
                        }
                        printf("\n%-13s - ", "Total Amount"); printf("%20.2f", transaction[i].total);
                        printf("\n%-13s - ", "Seller ID"); printf("%20d", transaction[i].sellerID);

                        strcpy(sellerName, user[findUser(user, nUsers, transaction[i].sellerID)].name);
                        printf("\n%-13s - ", "Seller name"); printf("%s\n", sellerName);
                    }

                    nTransactions = countTransactions(transaction, user, curUser);
                    for (i = 0; i < nTransactions; i++) {
                        nTransactionItems = countTransactionItems(transaction, i);
                        for (j = 0; j < nTransactionItems; j++) {
                            // update product quantity in Items array
                            for (k = 0; k < nUsers; k++) {
                                if (user[k].userID == transaction[i].sellerID) {
                                    nItems = countItems(user, k);
                                    for (l = 0; l < nItems; l++) {
                                        if (user[k].items[l].productID == transaction[i].bought[j].productID) {
                                            user[k].items[l].quantity -= transaction[i].bought[j].quantity;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // remove items from cart
                    for (i = nCart - 1; i > -1; i--) {
                        for (j = i; j < nCart; j++) {
                            user[curUser].cart[j] = user[curUser].cart[j + 1];
                        }
                        nCart--;
                    }
                }
                break;
            case 2: // By a Specific Seller
                if (nCart > 0) {
                    printf("\nInput Seller ID: ");\
                    scanf("%d", &sellerID);

                    nCart = countCart(user, nUsers, curUser);

                    // assign items in cart to transactions by seller
                    for (i = 0; i < nCart; i++) {
                        if (user[curUser].cart[i].sellerID == sellerID) {
                            j = 0;
                            nTransactions = countTransactions(transaction, user, curUser);
                            transactionFound = 0;
                            while (j <= nTransactions && !transactionFound) {
                                nTransactionItems = countTransactionItems(transaction, j);
                                if ((transaction[j].sellerID == -1 || transaction[j].sellerID == sellerID) && nTransactionItems < 5) {
                                    transaction[j].bought[nTransactionItems] = user[curUser].cart[i];
                                    transaction[j].buyerID = user[curUser].userID;
                                    transaction[j].sellerID = user[curUser].cart[i].sellerID;
                                    transaction[j].total += (user[curUser].cart[i].price * user[curUser].cart[i].quantity);
                                }
                            }
                        }
                    }

                    // display summary of transaction
                    nTransactions = countTransactions(transaction, user, curUser);
                    for (i = 0; i < nTransactions; i++) {
                        printf("------------------------------------\n");
                        printf("%23s", "TRANSACTION ");
                        printf("%d\n", i + 1);
                        nTransactionItems = countTransactionItems(transaction, i);
                        for (j = 0; j < nTransactionItems; j++) {
                            printf("\n%-13s - ", "Quantity");    printf("%20d", transaction[i].bought[j].quantity);
                            printf("\n%-13s - ", "Product ID");  printf("%20d", transaction[i].bought[j].productID);
                            printf("\n%-13s - ", "Item Name");   printf("%s", transaction[i].bought[j].itemName);
                            printf("\n%-13s - ", "Unit Price");  printf("%20.2f", transaction[i].bought[j].price);
                            printf("\n%-13s - ", "Total Price"); printf("%20.2f\n", transaction[i].bought[j].price * transaction[i].bought[j].quantity);

                            // add to binary file Transactions.dat
                            transactionFile = fopen("Transactions.dat", "ab");
                            
                            if (transactionFile != NULL) {
                                // write the transaction and its items to file
                                fwrite(&transaction[i], sizeof(transactionType), 1, transactionFile);
                            }

                            fclose(transactionFile);
                        }
                        printf("\n%-13s - ", "Total Amount"); printf("%20.2f", transaction[i].total);
                        printf("\n%-13s - ", "Seller ID"); printf("%20d", transaction[i].sellerID);

                        strcpy(sellerName, user[findUser(user, nUsers, transaction[i].sellerID)].name);
                        printf("\n%-13s - ", "Seller name"); printf("%s\n", sellerName);
                    }

                    for (i = 0; i < nTransactions; i++) {
                        nTransactionItems = countTransactionItems(transaction, i);
                        for (j = 0; j < nTransactionItems; j++) {
                            // update product quantity in Items array
                            for (k = 0; k < nUsers; k++) {
                                if (user[k].userID == transaction[i].sellerID) {
                                    nItems = countItems(user, k);
                                    for (l = 0; l < nItems; l++) {
                                        if (user[k].items[l].productID == transaction[i].bought[j].productID) {
                                            user[k].items[l].quantity -= transaction[i].bought[j].quantity;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // remove items from cart
                    for (i = nCart - 1; i > -1; i--) {
                        if (user[curUser].cart[i].sellerID == sellerID) {
                            for (j = i; j < nCart; j++) {
                                user[curUser].cart[j] = user[curUser].cart[j + 1];
                            }
                            nCart--;
                        }
                    }
                }
                break;
            case 3: // Specific Item
                if (nCart > 0) {
                    printf("Input Product ID: ");
                    scanf("%d", &productID);

                    nCart = countCart(user, nUsers, curUser);

                    i = 0;

                    // assign item to transaction
                    itemFound = 0;
                    while (i < nCart && !itemFound) {
                        if (user[curUser].cart[i].productID == productID) {
                            itemFound = 1;
                            j = 0;
                            nTransactions = countTransactions(transaction, user, curUser);

                            transactionFound = 0;
                            while (j <= nTransactions && !transactionFound) {
                                nTransactionItems = countTransactionItems(transaction, j);
                                if (transaction[j].sellerID == -1 && nTransactionItems == 0) {
                                    transactionFound = 1;
                                    transaction[j].bought[nTransactionItems] = user[curUser].cart[i];
                                    transaction[j].buyerID = user[curUser].userID;
                                    transaction[j].sellerID = user[curUser].cart[i].sellerID;
                                    transaction[j].total += (user[curUser].cart[i].price * user[curUser].cart[i].quantity);
                                }
                                j++;
                            }
                        }
                        i++;
                    }

                    // display summary of transaction
                    nTransactions = countTransactions(transaction, user, curUser);
                    for (i = 0; i < nTransactions; i++) {
                        printf("------------------------------------\n");
                        printf("%23s", "TRANSACTION ");
                        printf("%d\n", i + 1);
                        nTransactionItems = countTransactionItems(transaction, i);
                        for (j = 0; j < nTransactionItems; j++) {
                            printf("\n%-13s - ", "Quantity");    printf("%20d", transaction[i].bought[j].quantity);
                            printf("\n%-13s - ", "Product ID");  printf("%20d", transaction[i].bought[j].productID);
                            printf("\n%-13s - ", "Item Name");   printf("%s", transaction[i].bought[j].itemName);
                            printf("\n%-13s - ", "Unit Price");  printf("%20.2f", transaction[i].bought[j].price);
                            printf("\n%-13s - ", "Total Price"); printf("%20.2f\n", transaction[i].bought[j].price * transaction[i].bought[j].quantity);

                            // add to binary file Transactions.dat
                            transactionFile = fopen("Transactions.dat", "ab");
                            
                            if (transactionFile != NULL) {
                                // Write the transaction structure and its items to the file
                                fwrite(&transaction[i], sizeof(transactionType), 1, transactionFile);
                            }

                            fclose(transactionFile);
                        }
                        printf("\n%-13s - ", "Total Amount"); printf("%20.2f", transaction[i].total);
                        printf("\n%-13s - ", "Seller ID"); printf("%20d", transaction[i].sellerID);

                        strcpy(sellerName, user[findUser(user, nUsers, transaction[i].sellerID)].name);
                        printf("\n%-13s - ", "Seller name"); printf("%s\n", sellerName);
                    }

                    for (i = 0; i < nTransactions; i++) {
                        nTransactionItems = countTransactionItems(transaction, i);
                        for (j = 0; j < nTransactionItems; j++) {
                            // update product quantity in Items array
                            for (k = 0; k < nUsers; k++) {
                                if (user[k].userID == transaction[i].sellerID) {
                                    nItems = countItems(user, k);
                                    for (l = 0; l < nItems; l++) {
                                        if (user[k].items[l].productID == transaction[i].bought[j].productID) {
                                            user[k].items[l].quantity -= transaction[i].bought[j].quantity;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // remove items from cart
                    for (i = nCart - 1; i > -1; i--) {
                        if (user[curUser].cart[i].productID == productID) {
                            for (j = i; j < nCart; j++) {
                                user[curUser].cart[j] = user[curUser].cart[j + 1];
                            }
                            nCart--;
                        }
                    }
                }
                break;
            case 4: // Exit Check Out
                checkOutRunning = 0;
                break;
            default:
                printf("\nInvalid input. Try again.\n");
        }
    }
}

/* This function shows all the users arranged by user ID, including each one's userID, password, name, address, and phone number.
@param user[] is the array of users
@param nUsers is the number of users in the user[] array
*/
void 
adminUsers (userType user[], 
            int nUsers) 
{
    int i, j, min;

    // sort users by userID
    for (i = 0; i < nUsers - 1; i++) {
        min = i;
        for (j = i + 1; j < nUsers; j++) {
            if (user[j].userID < user[min].userID) {
                min = j;
            }
        }
        swapUsers(&user[min], &user[i]);
    }

    printf("\nUSERS\n");

    for (i = 0; i < nUsers; i++) {
        printf("\n%-12s - ", "User ID");      printf("%30d", user[i].userID);
        printf("\n%-12s - ", "Password");     printf("%s", user[i].password);
        printf("\n%-12s - ", "Name");         printf("%s", user[i].name);
        printf("\n%-12s - ", "Address");      printf("%s", user[i].address);
        printf("\n%-12s - ", "Phone Number"); printf("%30s\n", user[i].number);
    }
}

/* This function shows all the users who have items to be sold, including each one's
userID, password, name, address, phone number, and number of items for sale.
@param user[] is the array of users
@param nUsers is the number of users in the user[] array
*/
void 
adminSellers (userType user[], 
              int nUsers) 
{
    int i, j, min;
    int nItems; // number of items user has

    // sort users by userID
    for (i = 0; i < nUsers - 1; i++) {
        min = i;
        for (j = i + 1; j < nUsers; j++) {
            if (user[j].userID < user[min].userID) {
                min = j;
            }
        }
        swapUsers(&user[min], &user[i]);
    }

    printf("\nSELLERS\n");

    for (i = 0; i < nUsers; i++) {
        if (user[i].items[0].sellerID == user[i].userID) {
            nItems = countItems(user, i);
            printf("\n%-24s - ", "User ID");                  printf("%30d", user[i].userID);
            printf("\n%-24s - ", "Password");                 printf("%s", user[i].password);
            printf("\n%-24s - ", "Name");                     printf("%s", user[i].name);
            printf("\n%-24s - ", "Address");                  printf("%s", user[i].address);
            printf("\n%-24s - ", "Phone Number");             printf("%30s", user[i].number);
            printf("\n%-24s - ", "Number of Items for Sale"); printf("%30d\n", nItems);
        }
    }
}

/* This function checks if the given current date falls between the given start and end dates.
@param start is the start date
@param end is the end date
@param current is the date being checked
*/
int 
compareDates (dateType start, 
              dateType end, 
              dateType current) 
{
    int valid = 0;

    if (current.year > start.year && current.year < end.year)
        valid = 1;
    else if (current.year == start.year && current.month > start.month)
        valid = 1;
    else if (current.year == start.year && current.month == start.month && current.day >= start.day)
        valid = 1;
    
    return valid;
}

/* This function checks the contents of Transactions.dat whose dates fall within the duration 
from the start to the end dates, and calculates the total amount of all those transactions.
*/
void 
adminTotalSales () 
{
    dateType startDate; // start date
    dateType endDate; // end date
    transactionType transaction; // takes transaction data from file
    double totalSales = 0; // total sales during given interval
    FILE *transactionFile; // file pointer for Transactions.dat

    printf("\nSTART DATE: \n");
    printf("Month (MM): ");
    scanf("%d", &startDate.month);
    printf("Day (DD): ");
    scanf("%d", &startDate.day);
    printf("Year (YYYY): ");
    scanf("%d", &startDate.year);

    printf("\nEND DATE: \n");
    printf("Month (MM): ");
    scanf("%d", &endDate.month);
    printf("Day (DD): ");
    scanf("%d", &endDate.day);
    printf("Year (YYYY): ");
    scanf("%d", &endDate.year);
    
    transactionFile = fopen("Transactions.dat", "rb");
    
    if (transactionFile != NULL) {
        while (fread(&transaction, sizeof(transactionType), 1, transactionFile) == 1) {
            if (compareDates(startDate, endDate, transaction.date) == 1)
                totalSales += transaction.total;
        }
    }

    fclose(transactionFile);

    printf("\nTotal sales from %d/%d/%d to %d/%d/%d: %.2f\n", startDate.month, startDate.day, startDate.year,
                                                               endDate.month, endDate.day, endDate.year, totalSales);
}

/* This function checks the contents of Transactions.dat whose dates fall within the duration 
from the start to the end dates, and displays the total sales for each seller in table format.
@param user[] is the array of users
@param nUsers is the number of users in the user[] array
*/
void 
adminSellersSales (userType user[], 
                   int nUsers) 
{
    dateType startDate; // start date
    dateType endDate; // end date
    transactionType transactions[100]; // array of transactions
    int nTransactions = 0; // number of transactions
    int nSellers = 0; // number of sellers
    int sellers[100]; // array of sellers
    int found = 0; // flag used to check if transaction's seller is found
    int i, j;
    double sales = 0; // amount of sales seller has during interval
    string20 name; // name of seller
    FILE *transactionFile; // file pointer for Transaction.dat

    printf("\nSTART DATE: \n");
    printf("Month (MM): ");
    scanf("%d", &startDate.month);
    printf("Day (DD): ");
    scanf("%d", &startDate.day);
    printf("Year (YYYY): ");
    scanf("%d", &startDate.year);

    printf("\nEND DATE: \n");
    printf("Month (MM): ");
    scanf("%d", &endDate.month);
    printf("Day (DD): ");
    scanf("%d", &endDate.day);
    printf("Year (YYYY): ");
    scanf("%d", &endDate.year);
    
    transactionFile = fopen("Transactions.dat", "rb");

    if (transactionFile != NULL) {
        while (fread(&transactions[nTransactions], sizeof(transactionType), 1, transactionFile) == 1) {
            nTransactions++;
        }
    }

    fclose(transactionFile);

    // look for transaction's seller
    for (i = 0; i < nTransactions; i++) {
        found = 0;
        for (j = 0; j < nSellers; j++) {
            if (transactions[i].sellerID == sellers[j]) {
                found = 1;
            }
        }

        if (!found) {
            sellers[nSellers] = transactions[i].sellerID;
            nSellers++;
        }
    }

    // if transaction date falls between interval, add total price to total sales for interval
    for (i = 0; i < nSellers; i++) {
        sales = 0;
        for (j = 0; j < nTransactions; j++) {
            if (sellers[i] == transactions[j].sellerID) {
                if (compareDates(startDate, endDate, transactions[j].date) == 1)
                    sales += transactions[j].total;
            }
        }

        // add seller name to array of sellers with valid transactions
        for (j = 0; j < nUsers; j++) {
            if (sellers[i] == user[j].userID) {
                strcpy(name, user[j].name);
            }
        }

        if (sales > 0) {
            printf("\n%-11s - ", "Seller ID");   printf("%d", sellers[i]);
            printf("\n%-11s - ", "Seller Name"); printf("%s", name);
            printf("\n%-11s - ", "Total Sales"); printf("%.2f\n", sales);
        }
    }
}

/* This function checks the contents of Transactions.dat whose dates fall within the duration 
from the start to the end dates, and displays the total amount bought for each buyer.
@param user[] is the array of users
@param nUsers is the number of users in the user[] array
*/
void 
adminShopaholics (userType user[], 
                  int nUsers) 
{
    dateType startDate; // start date
    dateType endDate; //  end date
    transactionType transactions[100]; // array of transactions
    int nTransactions = 0; // number of transactions
    int nBuyers = 0; // number of buyers
    int buyers[100]; // array of buyers
    int found = 0; // flag used to check if transaction's buyer is found
    int i, j;
    double bought = 0; // total bought during interval
    string20 name; // name of buyer
    FILE *transactionFile; // file pointer for Transaction.dat

    printf("\nSTART DATE: \n");
    printf("Month (MM): ");
    scanf("%d", &startDate.month);
    printf("Day (DD): ");
    scanf("%d", &startDate.day);
    printf("Year (YYYY): ");
    scanf("%d", &startDate.year);

    printf("\nEND DATE: \n");
    printf("Month (MM): ");
    scanf("%d", &endDate.month);
    printf("Day (DD): ");
    scanf("%d", &endDate.day);
    printf("Year (YYYY): ");
    scanf("%d", &endDate.year);
    
    transactionFile = fopen("Transactions.dat", "rb");

    if (transactionFile != NULL) {
        while (fread(&transactions[nTransactions], sizeof(transactionType), 1, transactionFile) == 1) {
            nTransactions++;
        }
    }

    fclose(transactionFile);

    // look for transaction's buyer
    for (i = 0; i < nTransactions; i++) {
        found = 0;
        for (j = 0; j < nBuyers; j++) {
            if (transactions[i].buyerID == buyers[j]) {
                found = 1;
            }
        }
         
        
        if (!found) {
            buyers[nBuyers] = transactions[i].buyerID;
            nBuyers++;
        }
    }

    // if transaction falls between interval, add total price to user's total bought
    for (i = 0; i < nBuyers; i++) {
        bought = 0;
        for (j = 0; j < nTransactions; j++) {
            if (buyers[i] == transactions[j].buyerID) {
                if (compareDates(startDate, endDate, transactions[j].date) == 1)
                    bought += transactions[j].total;
            }
        }

        // add buyer's name to array of buyers
        for (j = 0; j < nUsers; j++) {
            if (buyers[i] == user[j].userID) {
                strcpy(name, user[j].name);
            }
        }

        if (bought > 0) {
            printf("\n%-12s - ", "Seller ID");   printf("%d", buyers[i]);
            printf("\n%-12s - ", "Seller Name"); printf("%s", name);
            printf("\n%-12s - ", "Total Bought"); printf("%.2f\n", bought);
        }
    }
}

int 
main ()
{
    char cDump;             
    userType user[100];   // array of users
    itemType items[2000]; // array of items
    int mainRunning = 1;  // flag used to check if Main Menu is still running
    int mainChoice;       //  user's Main Menu choice
    int nUsers = 0;       //  number of users in user[]
    int nItems = 0;       // number of items
    int totalItems = 0;   // total items in Items.txt and available
    int curID = -1;       // userID of the registered user
    int userRunning;      // flag used to check if User Menu is still running
    int userChoice;       // user's User Menu choice
    int sellRunning;      // flag used to check if Sell Menu is still running
    int sellChoice;       // user's Sell Menu choice
    int buyRunning;       // flag used to check if Buy Menu is still running
    int buyChoice;        // user's Buy Menu choice
    string10 password;    // variable of the inputted password during Admin login
    int adminRunning;     // flag used to check if Admin Menu is still running
    int adminChoice;      // user's Admin Menu choice
    char filename[20];    // used for generating files
    FILE *cartFile;       // file pointer for user's .bag
    int curUser;          // index of current user in user[]
    int nCart;            // number of items curUser has
    int i, j;             
    FILE *userFile;       // file pointer for Users.txt
    FILE *itemFile;       // file pointer for Items.txt

    // import Users.txt
    userFile = fopen("Users.txt", "r");
    if (userFile != NULL) {
        while ((fscanf(userFile, "%d %[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n", &user[nUsers].userID, user[nUsers].password,
                                                       user[nUsers].name,
                                                       user[nUsers].address,
                                                       user[nUsers].number) == 5) && nUsers < 100) {
            nUsers++;
        }
    }

    fclose(userFile);

    // import Items.txt
    itemFile = fopen("Items.txt", "r");
    if (itemFile != NULL) {
        while ((fscanf(itemFile, "%d %d\n%[^\n]\n%[^\n]\n%[^\n]\n%d %f\n", &items[totalItems].productID, &items[totalItems].sellerID,
                                                              items[totalItems].itemName,
                                                              items[totalItems].category,
                                                              items[totalItems].description,
                                                              &items[totalItems].quantity, &items[totalItems].price) == 7) && totalItems < 2000) {
            totalItems++;
        }
    }

    fclose(itemFile);

    // assign items from items[] to user.items[] based on sellerID
    for (i = 0; i < nUsers; i++) {
        for (j = 0; j < totalItems; j++) {
            nItems = countItems(user, i);
            if (user[i].userID == items[j].sellerID) {
                user[i].items[nItems] = items[j];
            }
        }
    }

    while (mainRunning) {
        printf("\nMAIN MENU\n");
        printf("\n[1] Register as a User\n");
        printf("[2] User Menu\n");
        printf("[3] Admin Menu\n");
        printf("[4] Exit\n");
        printf("\nMain Menu Choice: ");
        scanf("%d%c", &mainChoice, &cDump);

        switch (mainChoice) {
        case 1: // register user
            registerUser(user, nUsers);
            nUsers++;
            break;
        case 2: // user menu
            curID = loginUser(user, nUsers);

            if (curID == -1) {
                printf("\nError: Invalid login details. Redirecting back to main menu.\n");
                userRunning = 0;
            } else {
                userRunning = 1;
            }

            curUser = findUser(user, nUsers, curID);

            while (userRunning) {
                printf("\nUSER MENU\n");
                printf("\n[1] Sell Menu\n");
                printf("[2] Buy Menu\n");
                printf("[3] Exit User Menu\n");
                printf("\nUser Menu Choice: ");
                scanf("%d", &userChoice);

                switch (userChoice) {
                    case 1: // sell menu
                        sellRunning = 1;
                        
                        while (sellRunning) {
                            printf("\nSELL MENU\n");
                            printf("\n[1] Add New Item\n");
                            printf("[2] Edit Stock\n");
                            printf("[3] Show My Products\n");
                            printf("[4] Show My Low Stock Products\n");
                            printf("[5] Exit Sell Menu\n");
                            printf("\nSell Menu Choice: ");
                            scanf("%d%c", &sellChoice, &cDump);

                            switch (sellChoice) {
                                case 1: // add new item
                                    sellAddItem(user, nUsers, curID);
                                    break;
                                case 2: // edit stock
                                    sellEditStock(user, nUsers, curID);
                                    break;
                                case 3: // show my products
                                    sellShowMyProducts(user, nUsers, curID);
                                    break;
                                case 4: // show my low stock products
                                    sellShowLowStock(user, nUsers, curID);
                                    break;
                                case 5: // exit sell menu
                                    sellRunning = 0;
                                    break;
                                default:
                                    printf("\nInvalid input. Try again.\n");
                            }
                        }
                        break;
                    case 2: // buy menu
                         // generate the filename based on the user ID
                        sprintf(filename, "%d.bag", curID);

                        // import user's .bag file
                        cartFile = fopen(filename, "rb");
                        if (cartFile != NULL) {
                            fread(user[curUser].cart, sizeof(itemType), 10, cartFile);
                            printf("Cart for user %d loaded successfully\n", curID);
                        }

                        fclose(cartFile);

                        buyRunning = 1;

                        while (buyRunning) {
                            printf("\nBUY MENU\n");
                            printf("\n[1] View All Products\n");
                            printf("[2] Show All Products of a Specific Seller\n");
                            printf("[3] Search Products by Category\n");
                            printf("[4] Search Products by Name\n");
                            printf("[5] Add to Cart\n");
                            printf("[6] Edit Cart\n");
                            printf("[7] Check Out Menu\n");
                            printf("[8] Exit Buy Menu\n");
                            printf("\nBuy Menu Choice: ");
                            scanf("%d%c", &buyChoice, &cDump);

                            switch (buyChoice) {
                                case 1: // View All Products
                                    buyViewAll(user, nUsers);
                                    break;
                                case 2: // Show All Products by a Specific Seller
                                    buyShowSellerAll(user, nUsers);
                                    break;
                                case 3: // Search Products by Category
                                    buyCategorySearch(user, nUsers);
                                    break;
                                case 4: // Search Products by Name
                                    buyNameSearch(user, nUsers);
                                    break;
                                case 5: // Add to Cart
                                    buyAddToCart(user, nUsers, curID);
                                    break;
                                case 6: // Edit Cart
                                    buyEditCart(user, nUsers, curID);
                                    break;
                                case 7: // Check Out Menu
                                    buyCheckOut(user, nUsers, curID);
                                    break;
                                case 8: // Exit Buy Menu
                                    buyRunning = 0;
                                    break;
                            }
                        }
                        break;
                    case 3: // exit user menu
                        nCart = countCart(user, nUsers, curUser);
                        printf("%d", nCart);

                        if (nCart > 0) {
                            // add to user's .bag file
                            sprintf(filename, "%d.bag", curID);

                            cartFile = fopen(filename, "wb");
                            if (cartFile != NULL) {
                                fwrite(user[curUser].cart, sizeof(itemType), nCart, cartFile);
                            }

                            fclose(cartFile);
                        }
                        
                        userRunning = 0;
                        break;
                    default:
                        printf("\nInvalid input. Try again.\n");
                }
            }
            break;
        case 3: // admin menu
            printf("\nAdmin Password: ");
            getString(password);
            
            int valid = strcmp(password, "H3LLo?");

            adminRunning = 0;

            if (valid != 0) {
                printf("\nUnauthorized access not allowed\n");
            } else {
                adminRunning = 1;

                while (adminRunning) {
                    printf("\nADMIN MENU\n");
                    printf("\n[1] Show All Users\n");
                    printf("[2] Show All Sellers\n");
                    printf("[3] Show Total Sales in Given Duration\n");
                    printf("[4] Show Sellers' Sales\n");
                    printf("[5] Show Shopaholics\n");
                    printf("[6] Back to Main Menu\n");

                    printf("\nAdmin Menu Choice: ");
                    scanf("%d", &adminChoice);

                    switch (adminChoice) {
                        case 1: // Show All Users
                            adminUsers(user, nUsers);
                            break;
                        case 2: // Show All Sellers
                            adminSellers(user, nUsers);
                            break;
                        case 3: // Show Total Sales in Given Duration
                            adminTotalSales();
                            break;
                        case 4: // Show Sellers' Sales
                            adminSellersSales(user, nUsers);
                            break;
                        case 5: // Show Shopaholics
                            adminShopaholics(user, nUsers);
                            break;
                        case 6: // Back to Main Menu
                            adminRunning = 0;
                            break;
                        default:
                            printf("\nInvalid input. Try again.\n");
                    }
                }
            }
            break;
        case 4: // exit
            // save user data to Users.txt
            userFile = fopen("Users.txt", "w");
            if (userFile != NULL) {
                for (i = 0; i < nUsers; i++) {
                    fprintf(userFile, "%d %s\n", user[i].userID, user[i].password);
                    fprintf(userFile, "%s\n", user[i].name);
                    fprintf(userFile, "%s\n", user[i].address);
                    fprintf(userFile, "%s\n\n", user[i].number);
                }
            }

            fclose(userFile);

            // save item data to Items.txt
            itemFile = fopen("Items.txt", "w");
            if (itemFile != NULL) {
                for (i = 0; i < nUsers; i++) {
                    nItems = countItems(user, i);
                    for (j = 0; j < nItems; j++) {
                        fprintf(itemFile, "%d %d\n", user[i].items[j].productID, user[i].items[j].sellerID);
                        fprintf(itemFile, "%s\n", user[i].items[j].itemName);
                        fprintf(itemFile, "%s\n", user[i].items[j].category);
                        fprintf(itemFile, "%s\n", user[i].items[j].description);
                        fprintf(itemFile, "%d %.2f\n\n", user[i].items[j].quantity, user[i].items[j].price);
                    }
                }
            }

            fclose(itemFile);

            mainRunning = 0;
            break;
        default:
            printf("\nInvalid input. Try again.\n");
        }
    }

    return 0;
}