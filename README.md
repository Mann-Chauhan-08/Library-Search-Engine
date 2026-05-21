# 📚 Library Book Search Engine

**A High-Performance C++ Search Engine using Dual-Architecture Indexing (BST & Hash Tables)**

[![C++](https://img.shields.io/badge/Language-C++-blue.svg)](https://isocpp.org/)
[![Data Structures](https://img.shields.io/badge/Topic-Data%20Structures-success.svg)](#)

## 📌 Project Overview
Modern library databases manage thousands of records, making traditional $\mathcal{O}(n)$ linear search algorithms highly inefficient. This project is a C++ search engine engineered to index and query a real-world dataset of **13,400+ books**. 

To optimize search queries, the system indexes the dataset simultaneously using two distinct data structures: **Binary Search Trees (BST)** and **Hash Tables (with Separate Chaining)**. This allows a comparative analysis between theoretical $\mathcal{O}(\log n)$ and average-case $\Theta(1)$ retrieval times.

### 🔗 Quick Links
* **[Read the Full Project Report (PDF)](./Library%20Search%20Engine%20Report.pdf)** - *Contains deep mathematical collision analysis, time-complexity charts, and system design.*
* **[Try the Live Demo (OnlineGDB)](https://onlinegdb.com/NmHkHpPAM)** - *Note: The live demo uses a compressed dataset (~500 books) due to online compiler memory limits. Clone this repo to test the full 13k+ dataset.*

## 🚀 Key Engineering Features

* **Dual-Indexing Architecture:** A single `Book` struct is utilized across four different logical frameworks simultaneously using a multi-pointer architecture (`tleft`, `tright`, `tnxHash`, `tnxBST`, etc.) to prevent memory duplication.
* **Iterative BST Insertion:** Refactored standard recursive tree insertion into an **iterative `while(1)` loop**. This successfully eliminates Stack Overflow limits when importing alphabetically pre-sorted real-world CSV datasets.
* **Hash Collision Management:** Implemented a polynomial rolling hash function modulo `13,681` (a prime number) with Separate Chaining. Achieved a highly optimal effective load factor (averaging ~1.78 for Titles).
* **Robust ETL Pipeline:** Engineered a custom, crash-proof `safestoi()` function to safely bypass standard C++ library limitations when handling hidden carriage returns (`\r`) and messy CSV anomalies.
* **Real-World Borrow/Return:** Integrated a "soft-deletion" pointer mechanism (using a `status` boolean) to reflect library check-outs without destroying pointer integrity, automatically rewriting state changes to persistent local storage.

## 🛠️ How to Run Locally

1. Clone this repository:
   ```bash
   git clone [https://github.com/YOUR-USERNAME/Library-Search-Engine.git](https://github.com/YOUR-USERNAME/Library-Search-Engine.git)

2. Compile the C++ source code:
   ```bash
   g++ main.cpp -o library_search

3. Run the executable:
   ```bash
   ./library_search

4. On the very first run, select Option 24 to parse the raw CSV file and build the local database.
   ```bash
   library_data.txt
