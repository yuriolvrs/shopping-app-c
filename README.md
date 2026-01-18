# Shopping App (C)

## Overview
A terminal-based shopping application written in C (C99) that simulates online buying and selling.  
The program supports user registration, seller and buyer functionalities, inventory management, cart operations, transactions, and admin reporting.

## Features
- **User Management:** Register and log in as buyer, seller, or both with unique user IDs.  
- **Selling Menu:** Add new items (up to 20 per seller), edit stock, update price, category, description, and view low-stock items.  
- **Buying Menu:** Browse all products, search by seller, category, or keywords, add items to cart, edit cart, and check out.  
- **Transactions:** Handles multiple items per transaction, updates inventory, and saves transaction summaries.  
- **Admin Menu:** View all users or sellers, total sales in a given period, sellers’ sales, and buyers’ purchase summaries.  
- **Persistent Data:** Loads and saves user and item data from `Users.txt` and `Items.txt`.

- Language: C (C99)  
- Platform: Terminal / Command Line  
- Compiler: `gcc`  

## Project Context
This project was developed as part of **CCPROG2 – Programming with Structured Data Types** at De La Salle University.  
It demonstrates modular programming, structured data management, file handling, and implementing a mini e-commerce system in C.

## Design Highlights
- Modular function-based design for user, seller, buyer, and admin operations.  
- Arrays and structures used for storing users, items, and transactions.  
- Input validation ensures correct and safe interactions.  
- Sorting and table-formatted display of items, users, and transactions.  
- Cart management allows editing quantities, removing items, and per-seller checkout.
