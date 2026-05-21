#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;

typedef long long ll;
double tbooks=0;

string asLower(string st)
{
    transform(st.begin(), st.end(), st.begin(), ::tolower);
    return st;
}

string asTitle(string st)
{
    if (st.empty())
        return st;

    string tm = asLower(st);
    tm[0] = toupper(tm[0]);

    for (int i = 1; i < tm.size(); i++)
    {
        if (tm[i - 1] == ' ')
            tm[i] = toupper(tm[i]);
    }
    return tm;
}

typedef struct Book
{
    string title;
    string author;
    int year;
    bool status = 1;

    Book *tleft = NULL;
    Book *tright = NULL;
    Book *tnxHash = NULL;
    Book *tnxBST = NULL;

    Book *aleft = NULL;
    Book *aright = NULL;
    Book *anxHash = NULL;
    Book *anxBST = NULL;
} Book;

Book *THT[13681] = {NULL};
Book *AHT[13681] = {NULL};

int hash_fun(string st)
{
    st = asLower(st);
    ll val = 0;
    for (char ch : st)
        val = ((val * 31) + ch) % 13681;
    return val;
}

void Print_Book(Book *b, int ct)
{
    cout << "\n-------- Result # " << ct << " --------\n";
    cout << "Title  : " << asTitle(b->title) << '\n';
    cout << "Author : " << asTitle(b->author) << '\n';
    cout << "Year   : " << b->year << '\n';
    cout << "Status : " << (b->status ? "Available\n" : "Currently Borrowed\n");
    cout << "----------------------------\n";
}

void in_Hash_by_title(Book *nb)
{
    int idx = hash_fun(nb->title);
    if (THT[idx] == NULL)
        THT[idx] = nb;
    else
    {
        nb->tnxHash = THT[idx];
        THT[idx] = nb;
    }
}

Book *in_BST_by_title(Book *root, Book *nb)
{
    if (root == NULL)
        return nb;

    Book *tm = root;
    string nbTit = asLower(nb->title);
    while (1)
    {
        string rTit = asLower(tm->title);
        if (nbTit < rTit)
        {
            if (tm->tleft == NULL)
            {
                tm->tleft = nb;
                break;
            }
            tm = tm->tleft;
        }
        else if (nbTit > rTit)
        {
            if (tm->tright == NULL)
            {
                tm->tright = nb;
                break;
            }
            tm = tm->tright;
        }
        else
        {
            nb->tnxBST = tm->tnxBST;
            tm->tnxBST = nb;
            break;
        }
    }
    return root;
}

void Search_Hash_Title(string tit)
{
    int idx = hash_fun(tit), ct = 0;
    Book *ptrb = THT[idx];
    tit = asLower(tit);

    cout << "Searching for '" << asTitle(tit) << "' in Hash table...\n";

    while (ptrb != NULL)
    {
        if (asLower(ptrb->title) == tit)
        {
            ct++;
            Print_Book(ptrb, ct);
        }
        ptrb = ptrb->tnxHash;
    }

    if (!ct)
        cout << "No books are found with title '" << asTitle(tit) << "'\n";
    else
        cout << "Total books found with '" << asTitle(tit) << "' is : " << ct << '\n';
}

void Search_BST_Title(Book *root, string tit)
{
    if (root == NULL)
    {
        cout << "No books are found with title '" << asTitle(tit) << "'\n";
        return;
    }
    string rTit = asLower(root->title);

    if (tit < rTit)
        Search_BST_Title(root->tleft, tit);
    else if (tit > rTit)
        Search_BST_Title(root->tright, tit);
    else
    {
        int ct = 1;
        Print_Book(root, ct);

        Book *ptrb = root->tnxBST;
        while (ptrb != NULL)
        {
            ct++;
            Print_Book(ptrb, ct);
            ptrb = ptrb->tnxBST;
        }

        cout << "Total books found with '" << asTitle(tit) << "' is : " << ct << '\n';
    }
}

void Analyze_Collisions(int type)
{
    int i, emp = 0, oneb = 0, moreb = 0, mxlen = 0;
    for (i = 0; i < 13681; i++)
    {
        Book *ptrb = (type == 1 ? THT[i] : AHT[i]);
        if (ptrb == NULL)
            emp++;
        else
        {
            int len = 0;
            while (ptrb != NULL)
            {
                len++;
                ptrb = (type == 1 ? ptrb->tnxHash : ptrb->anxHash);
            }

            if (len == 1)
                oneb++;
            else
                moreb++;
            mxlen = max(mxlen, len);
        }
    }

    double loadfactor = (tbooks)/13681;
    string tname = (type == 1 ? "Title" : "Author");
    cout << "\n=== " << tname << " Hash Table Collision Analysis ===\n";
    cout << "Total Table Size       : " << 13681 << '\n';
    cout << "Empty Slots            : " << emp << '\n';
    cout << "Slots with 1 Book      : " << oneb << '\n';
    cout << "Slots with collisions  : " << moreb << '\n';
    cout << "Longest Chain Length   : " << mxlen << '\n';
    cout << "Load Factor            : " << loadfactor << '\n';
    cout << "========================================\n";
}

void in_Hash_by_Author(Book *nb)
{
    int idx = hash_fun(nb->author);
    if (AHT[idx] == NULL)
        AHT[idx] = nb;
    else
    {
        nb->anxHash = AHT[idx];
        AHT[idx] = nb;
    }
}

Book *in_BST_by_Author(Book *root, Book *nb)
{
    if (root == NULL)
        return nb;

    Book *tm = root;
    string nbAut = asLower(nb->author);
    while (1)
    {
        string rAut = asLower(tm->author);
        if (nbAut < rAut)
        {
            if (tm->aleft == NULL)
            {
                tm->aleft = nb;
                break;
            }
            tm = tm->aleft;
        }
        else if (nbAut > rAut)
        {
            if (tm->aright == NULL)
            {
                tm->aright = nb;
                break;
            }
            tm = tm->aright;
        }
        else
        {
            nb->anxBST = tm->anxBST;
            tm->anxBST = nb;
            break;
        }
    }
    return root;
}

void Search_Hash_Author(string aut)
{
    int idx = hash_fun(aut), ct = 0;
    Book *ptrb = AHT[idx];
    aut = asLower(aut);

    cout << "Searching for '" << asTitle(aut) << "' books in Hash table...\n";

    while (ptrb != NULL)
    {
        if (asLower(ptrb->author) == aut)
        {
            ct++;
            Print_Book(ptrb, ct);
        }
        ptrb = ptrb->anxHash;
    }

    if (!ct)
        cout << "NO books are found with author '" << asTitle(aut) << "'\n";
    else
        cout << "Total books found with '" << asTitle(aut) << "' is : " << ct << '\n';
}

void Search_BST_Author(Book *root, string aut)
{
    if (root == NULL)
    {
        cout << "No books are found with author '" << asTitle(aut) << "'\n";
        return;
    }
    string rAut = asLower(root->author);

    if ((aut < rAut))
        Search_BST_Author(root->aleft, aut);
    else if (aut > rAut)
        Search_BST_Author(root->aright, aut);
    else
    {
        int ct = 1;
        Print_Book(root, ct);

        Book *ptrb = root->anxBST;
        while (ptrb != NULL)
        {
            ct++;
            Print_Book(ptrb, ct);
            ptrb = ptrb->anxBST;
        }

        cout << "Total books found with '" << asTitle(aut) << "' is " << ct << '\n';
    }
}

void Search_Hash_Both(string tit, string aut)
{
    int idx = hash_fun(tit), ct = 0;
    Book *ptr = THT[idx];
    tit = asLower(tit), aut = asLower(aut);

    cout << "Searching in Hash table for ' " << asTitle(tit) << "' and '" << asTitle(aut) << "'\n";

    while (ptr != NULL)
    {
        if (asLower(ptr->title) == tit && asLower(ptr->author) == aut)
        {
            ct++;
            Print_Book(ptr, ct);
        }
        ptr = ptr->tnxHash;
    }

    if (!ct)
        cout << "No book is found for this combination\n";
    else
        cout << "Total books found with '" << asTitle(tit) << "' wrriten by '" << asTitle(aut) << "' is : " << ct << '\n';
}

void Search_BST_Both(Book *root, string tit, string aut)
{
    if (root == NULL)
    {
        cout << "No book is found for this combination\n";
        return;
    }
    string rTit = asLower(root->title);

    if (tit < rTit)
        Search_BST_Both(root->tleft, tit, aut);
    else if (tit > rTit)
        Search_BST_Both(root->tright, tit, aut);
    else
    {
        int ct = 0;
        if (asLower(root->author) == aut)
        {
            ct++;
            Print_Book(root, ct);
        }

        Book *ptr = root->tnxBST;
        while (ptr != NULL)
        {
            if (asLower(ptr->author) == aut)
            {
                ct++;
                Print_Book(ptr, ct);
            }
            ptr = ptr->tnxBST;
        }
        if (!ct)
            cout << "Found the title, but not this specific author\n";
        else
            cout << "Total books found with '" << asTitle(tit) << "' wrriten by '" << asTitle(aut) << "' is : " << ct << '\n';
    }
}

void Clean_Memory()
{
    for (int i = 0; i < 13681; i++)
    {
        Book *ptr = THT[i];
        while (ptr != NULL)
        {
            Book *ptrn = ptr->tnxHash;
            delete ptr;
            ptr = ptrn;
        }
        THT[i] = NULL;
    }
}

void saveBookToFile(Book *b)
{
    ofstream file("library_data.txt", ios::app); // 'ios::app' like append
    if (file.is_open())
    {
        file << b->title << "|" << b->author << "|" << b->year << "|" << b->status << '\n';
        file.close();
    }
}

int safestoi(string s)
{
    int res = 0;
    for (char c : s)
    {
        if (c >= '0' && c <= '9')
            res = (res * 10) + (c - '0');
    }
    return res;
}

void loadBooksFromFile(Book *&tRoot, Book *&aRoot)
{
    ifstream file("library_data.txt");
    string tit, aut, year, stat;

    if (file.is_open())
    {
        cout << "Loading books from database...\n";
        while (getline(file, tit, '|') && getline(file, aut, '|') && getline(file, year, '|') && getline(file, stat))
        {
            if (tit.empty())
                continue;

            Book *nb = new Book();
            nb->title = tit;
            nb->author = aut;
            nb->year = safestoi(year);
            nb->status = stat == "1" ? 1 : 0;
            tbooks++;

            in_Hash_by_title(nb);
            in_Hash_by_Author(nb);
            tRoot = in_BST_by_title(tRoot, nb);
            aRoot = in_BST_by_Author(aRoot, nb);
        }
        file.close();
        cout << "Database loaded successfully!\n";
    }
    else
    {
        cout << "No existing database found. A new one will be created.\n";
    }
}

void Rewrite_file()
{
    ofstream file("library_data.txt", ios::trunc);
    if (file.is_open())
    {
        for (int i = 0; i < 13681; i++)
        {
            Book *ptr = THT[i];
            while (ptr != NULL)
            {
                file << ptr->title << "|" << ptr->author << "|" << ptr->year << "|" << ptr->status << '\n';
                ptr = ptr->tnxHash;
            }
        }
        file.close();
    }
}

void Borrow(string tit, string aut)
{
    int idx = hash_fun(tit);
    Book *ptr = THT[idx];

    while (ptr != NULL)
    {
        if (asLower(ptr->title) == tit && asLower(ptr->author) == aut)
        {
            if (ptr->status)
            {
                ptr->status = 0;
                cout << "You borrowed '" << asTitle(tit) << "' wrriten by '" << asTitle(aut) << "'\n";
                Rewrite_file();
                return;
            }
            else
            {
                cout << "'" << asTitle(tit) << "' wrriten by '" << asTitle(aut) << "' is currently borrowed by someone else\n";
                break;
            }
        }
        ptr = ptr->tnxHash;
    }
    cout << "Your book isn't found in our Library\n";
}

void Return(string tit, string aut)
{
    int idx = hash_fun(tit);
    Book *ptr = THT[idx];

    while (ptr != NULL)
    {
        if (asLower(ptr->title) == tit && asLower(ptr->author) == aut)
        {
            if (!(ptr->status))
            {
                ptr->status = 1;
                cout << "You returned '" << asTitle(tit) << "' wrriten by '" << asTitle(aut) << "'\n";
                Rewrite_file();
                return;
            }
            else
            {
                cout << "'" << asTitle(tit) << "' wrriten by '" << asTitle(aut) << "' is already in the library\n";
                return;
            }
        }
        ptr = ptr->tnxHash;
    }
    cout << "This book is not belong to our library!\n";
}

void importFromCSV(Book *&tRoot, Book *&aRoot)
{
    ifstream file("books.csv");
    string tit, aut, year, dummyLine;
    int importedCount = 0;

    if (file.is_open())
    {
        cout << "Importing perfectly clean dataset...\n";
        getline(file, dummyLine);

        while (getline(file, tit, ',') && getline(file, aut, ',') && getline(file, year))
        {
            if (tit.empty())
                continue;

            Book *nb = new Book();
            nb->title = tit;
            nb->author = aut;
            nb->year = safestoi(year);
            nb->status = 1;

            in_Hash_by_title(nb);
            in_Hash_by_Author(nb);
            tRoot = in_BST_by_title(tRoot, nb);
            aRoot = in_BST_by_Author(aRoot, nb);

            importedCount++;
        }
        file.close();
        Rewrite_file();
        cout << "Successfully imported " << importedCount << " books!\n";
    }
    else
    {
        cout << "Could not find 'books.csv'.\n";
    }
}

int main()
{
    Book *troot = NULL, *aroot = NULL;

    loadBooksFromFile(troot, aroot);

    int ch = 0;
    string tit, aut;
    while (ch != 11)
    {
        cout << "\n=============================\n";
        cout << "LIBRARY BOOK SEARCH ENGINE\n";
        cout << "=============================\n";
        cout << "1. Add new Book\n";
        cout << "2. Search bt title (HT)\n";
        cout << "3. Search by title (BST)\n";
        cout << "4. Search by author (HT)\n";
        cout << "5. Search by author (BST)\n";
        cout << "6. Search by title and author (HT)\n";
        cout << "7. Search by title and author (BST)\n";
        cout << "8. Analyze hash Collisions\n";
        cout << "9. Borrow Book\n";
        cout << "10. Return Book\n";
        cout << "11. Exit\n";
        cout << "24. Import new csv file books\n";
        cout << "Enter your choice (1-11) : ";
        cin >> ch;

        switch (ch)
        {
        case 1:
        {
            Book *nb = new Book();
            cout << "\nEnter Title : ";
            getline(cin >> ws, nb->title);
            cout << "Enter Author : ";
            getline(cin >> ws, nb->author);
            cout << "Enter Year : ";
            cin >> nb->year;
            tbooks++;

            in_Hash_by_title(nb);
            in_Hash_by_Author(nb);
            troot = in_BST_by_title(troot, nb);
            aroot = in_BST_by_Author(aroot, nb);

            saveBookToFile(nb);
            cout << "Book added and saved successfully\n";
            break;
        }
        case 2:
            cout << "\nEnter Title to search : ";
            getline(cin >> ws, tit);
            Search_Hash_Title(tit);
            break;
        case 3:
            cout << "\nEnter Title to search : ";
            getline(cin >> ws, tit);
            tit = asLower(tit);
            Search_BST_Title(troot, tit);
            break;
        case 4:
            cout << "\nEnter Author to search : ";
            getline(cin >> ws, aut);
            Search_Hash_Author(aut);
            break;
        case 5:
            cout << "\nEnter Author to search : ";
            getline(cin >> ws, aut);
            aut = asLower(aut);
            Search_BST_Author(aroot, aut);
            break;
        case 6:
            cout << "\nEnter Title to search : ";
            getline(cin >> ws, tit);
            cout << "Enter Author to search : ";
            getline(cin >> ws, aut);
            Search_Hash_Both(tit, aut);
            break;
        case 7:
            cout << "\nEnter Title to search : ";
            getline(cin >> ws, tit);
            cout << "Enter Author to search : ";
            getline(cin >> ws, aut);
            tit = asLower(tit), aut = asLower(aut);
            Search_BST_Both(troot, tit, aut);
            break;
        case 8:
            int t;
            cout << "\nEnter 1 for Title Analysis, 2 for Author Analysis : ";
            cin >> t;
            if (t == 1 || t == 2)
                Analyze_Collisions(t);
            else
                cout << "Invalid Input!\n";
            break;
        case 9:
            cout << "\nEnter Title to borrow : ";
            getline(cin >> ws, tit);
            cout << "Enter Author to borrow : ";
            getline(cin >> ws, aut);
            tit = asLower(tit), aut = asLower(aut);
            Borrow(tit, aut);
            break;
        case 10:
            cout << "\nEnter Title to return : ";
            getline(cin >> ws, tit);
            cout << "Enter Author to return : ";
            getline(cin >> ws, aut);
            tit = asLower(tit), aut = asLower(aut);
            Return(tit, aut);
            break;
        case 11:
            cout << "\nExiting... Your data is stored safely.\n";
            Clean_Memory();
            break;
        case 24:
            importFromCSV(troot, aroot);
            break;
        default:
            cout << "\nInvalid choice! Try again\n";
            break;
        }
    }
    return 0;
}