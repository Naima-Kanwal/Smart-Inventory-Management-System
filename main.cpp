#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cctype>
#include <ctime>
#include <windows.h>   // for SetConsoleTextAttribute (Dev C++ / Windows)

using namespace std;

// --- Helper: int/double to string (replaces to_string for old compilers) ---
template<typename T>
string intToStr(T val) {
    ostringstream oss;
    oss << val;
    return oss.str();
}

// --- Helper: Console Colors ------------------------------------------------
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void resetColor() { setColor(7); }

// --- Helper: Print a horizontal line ---------------------------------------
void printLine(char c = '-', int len = 60) {
    for (int i = 0; i < len; i++) cout << c;
    cout << endl;
}

// ---------------------------------------------------------------------------
// ABSTRACT BASE CLASS: Product
// Demonstrates: Abstraction, Encapsulation, Static Members,
//               Constructors/Destructors, Friend Function
// ---------------------------------------------------------------------------
class Product {
private:
    int    id;
    string name;
    double price;
    int    quantity;
    string category;

protected:
    static int totalProducts;   // Static member: shared across all objects

public:
    // Default Constructor
    Product() : id(0), name(""), price(0.0), quantity(0), category("") {}

    // Parameterized Constructor
    Product(int id, string name, double price, int qty, string cat)
        : id(id), name(name), price(price), quantity(qty), category(cat) {
        totalProducts++;
    }

    // Copy Constructor
    Product(const Product& p)
        : id(p.id), name(p.name), price(p.price),
          quantity(p.quantity), category(p.category) {
        totalProducts++;
    }

    // Virtual Destructor (needed for proper cleanup in inheritance)
    virtual ~Product() {
        totalProducts--;
    }

    // -- Getters (Encapsulation) ------------------------------------------
    int    getId()       const { return id; }
    string getName()     const { return name; }
    double getPrice()    const { return price; }
    int    getQuantity() const { return quantity; }
    string getCategory() const { return category; }

    // -- Setters (Encapsulation) ------------------------------------------
    void setId(int i)          { id = i; }
    void setName(string n)     { name = n; }
    void setPrice(double p)    { if (p >= 0) price = p; }
    void setQuantity(int q)    { if (q >= 0) quantity = q; }
    void setCategory(string c) { category = c; }

    // -- Static method ----------------------------------------------------
    static int getTotalProducts() { return totalProducts; }

    // -- Operator Overloading: == compares by ID --------------------------
    bool operator==(const Product& other) const {
        return this->id == other.id;
    }

    // -- Pure Virtual Function ? makes Product abstract -------------------
    virtual void displayDetails() const = 0;

    // -- Virtual function for extra info (overridden in subclasses) --------
    virtual string getExtraInfo() const { return "N/A"; }

    // -- Friend Function: can access private members -----------------------
    friend double calculateTotalValue(const Product& p);

    // -- Common display row ------------------------------------------------
    void displayRow() const {
        cout << left
             << setw(5)  << id
             << setw(20) << name
             << setw(12) << category
             << setw(10) << fixed << setprecision(2) << price
             << setw(8)  << quantity
             << setw(15) << getExtraInfo()
             << endl;
    }

    // -- Save to file (CSV format) -----------------------------------------
    virtual void saveToFile(ofstream& ofs) const {
        ofs << id << "," << name << "," << price << ","
            << quantity << "," << category << "," << getExtraInfo() << "\n";
    }
};

// Static member definition
int Product::totalProducts = 0;

// -- Friend Function implementation ------------------------------------------
double calculateTotalValue(const Product& p) {
    return p.price * p.quantity;   // accesses private members directly
}


// ---------------------------------------------------------------------------
// DERIVED CLASS: Electronics  (Inheritance from Product)
// ---------------------------------------------------------------------------
class Electronics : public Product {
private:
    int warrantyMonths;

public:
    Electronics() : Product(), warrantyMonths(0) {}

    Electronics(int id, string name, double price, int qty, int warranty)
        : Product(id, name, price, qty, "Electronics"), warrantyMonths(warranty) {}

    int getWarranty() const { return warrantyMonths; }
    void setWarranty(int w) { warrantyMonths = w; }

    // Override pure virtual function (Polymorphism)
    void displayDetails() const {
        setColor(11); // Cyan
        cout << "\n  [ELECTRONICS PRODUCT]\n";
        resetColor();
        cout << "  ID       : " << getId()       << "\n"
             << "  Name     : " << getName()     << "\n"
             << "  Price    : Rs. " << fixed << setprecision(2) << getPrice() << "\n"
             << "  Quantity : " << getQuantity() << "\n"
             << "  Warranty : " << warrantyMonths << " months\n";
    }

    // Override to show warranty in table
    string getExtraInfo() const {
        return intToStr(warrantyMonths) + " mo warranty";
    }

    void saveToFile(ofstream& ofs) const {
        ofs << "E," << getId() << "," << getName() << "," << getPrice() << ","
            << getQuantity() << ",Electronics," << warrantyMonths << "\n";
    }
};


// ---------------------------------------------------------------------------
// DERIVED CLASS: Grocery  (Inheritance from Product)
// ---------------------------------------------------------------------------
class Grocery : public Product {
private:
    string expiryDate;

public:
    Grocery() : Product(), expiryDate("") {}

    Grocery(int id, string name, double price, int qty, string expiry)
        : Product(id, name, price, qty, "Grocery"), expiryDate(expiry) {}

    string getExpiry() const { return expiryDate; }
    void setExpiry(string e)  { expiryDate = e; }

    // Override pure virtual function (Polymorphism)
    void displayDetails() const {
        setColor(10); // Green
        cout << "\n  [GROCERY PRODUCT]\n";
        resetColor();
        cout << "  ID          : " << getId()       << "\n"
             << "  Name        : " << getName()     << "\n"
             << "  Price       : Rs. " << fixed << setprecision(2) << getPrice() << "\n"
             << "  Quantity    : " << getQuantity() << "\n"
             << "  Expiry Date : " << expiryDate    << "\n";
    }

    string getExtraInfo() const {
        return "Exp: " + expiryDate;
    }

    void saveToFile(ofstream& ofs) const {
        ofs << "G," << getId() << "," << getName() << "," << getPrice() << ","
            << getQuantity() << ",Grocery," << expiryDate << "\n";
    }
};


// ---------------------------------------------------------------------------
// CLASS: Employee  (separate hierarchy, not inheriting Product)
// ---------------------------------------------------------------------------
class Employee {
protected:
    int    empId;
    string empName;
    string role;

public:
    Employee() : empId(0), empName(""), role("") {}
    Employee(int id, string name, string r) : empId(id), empName(name), role(r) {}
    virtual ~Employee() {}

    int    getEmpId()   const { return empId; }
    string getEmpName() const { return empName; }
    string getRole()    const { return role; }

    virtual void displayEmployee() const {
        cout << "  ID   : " << empId   << "\n"
             << "  Name : " << empName << "\n"
             << "  Role : " << role    << "\n";
    }
};

// -- Derived: Manager ---------------------------------------------------------
class Manager : public Employee {
private:
    string department;

public:
    Manager() : Employee(), department("") {}
    Manager(int id, string name, string dept)
        : Employee(id, name, "Manager"), department(dept) {}

    void displayEmployee() const {
        cout << "  ID         : " << empId      << "\n"
             << "  Name       : " << empName    << "\n"
             << "  Role       : " << role       << "\n"
             << "  Department : " << department << "\n";
    }
};


// ---------------------------------------------------------------------------
// CLASS: Inventory  (manages the collection of products)
// ---------------------------------------------------------------------------
class Inventory {
private:
    static const int MAX = 100;
    Product* products[MAX];   // Array of base-class pointers (Polymorphism)
    int count;
    string filename;

    // -- Shift array left after deletion ----------------------------------
    void shiftLeft(int index) {
        delete products[index];
        for (int i = index; i < count - 1; i++)
            products[i] = products[i + 1];
        count--;
    }

public:
    Inventory(string file = "inventory.csv") : count(0), filename(file) {
        for (int i = 0; i < MAX; i++) products[i] = NULL;
        loadFromFile();
    }

    ~Inventory() {
        saveToFile();
        for (int i = 0; i < count; i++) delete products[i];
    }

    // -- Add Product -------------------------------------------------------
    bool addProduct(Product* p) {
        if (count >= MAX) {
            cout << "  [ERROR] Inventory full!\n";
            return false;
        }
        // Check duplicate ID using operator==
        for (int i = 0; i < count; i++) {
            if (*products[i] == *p) {
                cout << "  [ERROR] Product with ID " << p->getId() << " already exists!\n";
                return false;
            }
        }
        products[count++] = p;
        return true;
    }

    // -- Search by ID ------------------------------------------------------
    Product* searchById(int id) const {
        for (int i = 0; i < count; i++)
            if (products[i]->getId() == id) return products[i];
        return NULL;
    }

    // -- Search by Name (partial match) ------------------------------------
    void searchByName(const string& keyword) const {
        bool found = false;
        printLine();
        cout << left << setw(5) << "ID" << setw(20) << "Name"
             << setw(12) << "Category" << setw(10) << "Price"
             << setw(8) << "Qty" << setw(15) << "Extra Info" << "\n";
        printLine();
        for (int i = 0; i < count; i++) {
            string n = products[i]->getName();
            // Simple case-insensitive partial match
            string lowerN = n, lowerK = keyword;
            for (int ci = 0; ci < (int)lowerN.size(); ci++) lowerN[ci] = tolower(lowerN[ci]);
            for (int ci = 0; ci < (int)lowerK.size(); ci++) lowerK[ci] = tolower(lowerK[ci]);
            if (lowerN.find(lowerK) != string::npos) {
                products[i]->displayRow();
                found = true;
            }
        }
        if (!found) {
            setColor(12);
            cout << "  No products found matching \"" << keyword << "\"\n";
            resetColor();
        }
        printLine();
    }

    // -- Update Product ----------------------------------------------------
    bool updateProduct(int id) {
        Product* p = searchById(id);
        if (!p) return false;

        int choice;
        cout << "\n  What to update?\n"
             << "  1. Name\n  2. Price\n  3. Quantity\n  0. Cancel\n"
             << "  Choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string newName;
            cout << "  New Name: ";
            getline(cin, newName);
            p->setName(newName);
        } else if (choice == 2) {
            double newPrice;
            cout << "  New Price: ";
            cin >> newPrice;
            p->setPrice(newPrice);
        } else if (choice == 3) {
            int newQty;
            cout << "  New Quantity: ";
            cin >> newQty;
            p->setQuantity(newQty);
        }
        return true;
    }

    // -- Delete Product ----------------------------------------------------
    bool deleteProduct(int id) {
        for (int i = 0; i < count; i++) {
            if (products[i]->getId() == id) {
                shiftLeft(i);
                return true;
            }
        }
        return false;
    }

    // -- Display All (uses virtual displayRow ? Polymorphism) --------------
    void displayAll() const {
        if (count == 0) {
            setColor(14);
            cout << "\n  Inventory is empty.\n";
            resetColor();
            return;
        }
        printLine('=');
        setColor(14);
        cout << left << setw(5) << "ID" << setw(20) << "Name"
             << setw(12) << "Category" << setw(10) << "Price"
             << setw(8) << "Qty" << setw(15) << "Extra Info" << "\n";
        resetColor();
        printLine('=');
        for (int i = 0; i < count; i++)
            products[i]->displayRow();
        printLine();
        cout << "  Total Products: " << count << "\n";
    }

    // -- Low Stock Report --------------------------------------------------
    void lowStockReport(int threshold = 5) const {
        setColor(12);
        cout << "\n  === LOW STOCK REPORT (Qty <= " << threshold << ") ===\n";
        resetColor();
        bool any = false;
        for (int i = 0; i < count; i++) {
            if (products[i]->getQuantity() <= threshold) {
                products[i]->displayRow();
                any = true;
            }
        }
        if (!any) {
            setColor(10);
            cout << "  All products have sufficient stock.\n";
            resetColor();
        }
    }

    // -- Total Inventory Value (uses friend function) -----------------------
    void displayTotalValue() const {
        double total = 0;
        for (int i = 0; i < count; i++)
            total += calculateTotalValue(*products[i]);
        setColor(10);
        cout << "\n  Total Inventory Value: Rs. "
             << fixed << setprecision(2) << total << "\n";
        resetColor();
    }

    // -- Save to File ------------------------------------------------------
    void saveToFile() const {
        ofstream ofs(filename.c_str());
        if (!ofs) return;
        for (int i = 0; i < count; i++)
            products[i]->saveToFile(ofs);
        ofs.close();
    }

    // -- Load from File ----------------------------------------------------
    void loadFromFile() {
        ifstream ifs(filename.c_str());
        if (!ifs) return;

        string line;
        while (getline(ifs, line)) {
            if (line.empty()) continue;

            // Parse CSV: type,id,name,price,qty,category,extra
            string fields[7];
            int f = 0;
            string token;
            for (int ci = 0; ci < (int)line.size(); ci++) {
                char ch = line[ci];
                if (ch == ',' && f < 6) { fields[f++] = token; token = ""; }
                else token += ch;
            }
            fields[f] = token;

            char type   = fields[0][0];
            int    id   = atoi(fields[1].c_str());
            string name = fields[2];
            double price= atof(fields[3].c_str());
            int    qty  = atoi(fields[4].c_str());
            string extra= fields[6];

            if (type == 'E') {
                int warranty = atoi(extra.c_str());
                products[count++] = new Electronics(id, name, price, qty, warranty);
            } else if (type == 'G') {
                products[count++] = new Grocery(id, name, price, qty, extra);
            }
        }
        ifs.close();
    }

    int getCount() const { return count; }
};


// ---------------------------------------------------------------------------
// CLASS: LoginSystem
// ---------------------------------------------------------------------------
class LoginSystem {
private:
    struct User { string username, password; };
    static const int MAX_USERS = 5;
    User users[MAX_USERS];
    int userCount;

public:
    LoginSystem() : userCount(0) {
        // Default admin credentials
        addUser("admin", "admin123");
        addUser("manager", "pass456");
    }

    void addUser(string u, string p) {
        if (userCount < MAX_USERS) {
            users[userCount].username = u;
            users[userCount].password = p;
            userCount++;
        }
    }

    bool login() {
        string u, p;
        int attempts = 3;
        while (attempts--) {
            cout << "\n  Username: ";
            cin >> u;
            cout << "  Password: ";
            cin >> p;
            for (int i = 0; i < userCount; i++) {
                if (users[i].username == u && users[i].password == p) {
                    setColor(10);
                    cout << "\n  Login successful! Welcome, " << u << ".\n";
                    resetColor();
                    return true;
                }
            }
            setColor(12);
            cout << "  Incorrect credentials. Attempts left: " << attempts << "\n";
            resetColor();
        }
        return false;
    }
};


// ---------------------------------------------------------------------------
// MENU FUNCTIONS
// ---------------------------------------------------------------------------
void printHeader() {
    system("cls");
    setColor(14);
    printLine('=');
    cout << "   SMART INVENTORY MANAGEMENT SYSTEM\n";
    printLine('=');
    resetColor();
}

void addProductMenu(Inventory& inv) {
    printHeader();
    cout << "\n  ADD NEW PRODUCT\n";
    printLine();
    cout << "  1. Electronics\n  2. Grocery\n  0. Back\n  Choice: ";
    int type;
    cin >> type;
    if (type == 0) return;

    int id, qty;
    string name;
    double price;

    cout << "  Product ID   : "; cin >> id;
    cin.ignore();
    cout << "  Product Name : "; getline(cin, name);
    cout << "  Price (Rs.)  : "; cin >> price;
    cout << "  Quantity     : "; cin >> qty;

    if (type == 1) {
        int warranty;
        cout << "  Warranty (months): "; cin >> warranty;
        Electronics* e = new Electronics(id, name, price, qty, warranty);
        if (inv.addProduct(e)) {
            setColor(10);
            cout << "\n  Electronics product added successfully!\n";
            resetColor();
        } else {
            delete e;
        }
    } else if (type == 2) {
        string expiry;
        cin.ignore();
        cout << "  Expiry Date (DD/MM/YYYY): "; getline(cin, expiry);
        Grocery* g = new Grocery(id, name, price, qty, expiry);
        if (inv.addProduct(g)) {
            setColor(10);
            cout << "\n  Grocery product added successfully!\n";
            resetColor();
        } else {
            delete g;
        }
    }
    cout << "\n  Press Enter to continue...";
    cin.ignore(); cin.get();
}

void searchMenu(Inventory& inv) {
    printHeader();
    cout << "\n  SEARCH PRODUCT\n";
    printLine();
    cout << "  1. Search by ID\n  2. Search by Name\n  0. Back\n  Choice: ";
    int ch; cin >> ch;

    if (ch == 1) {
        int id;
        cout << "  Enter Product ID: "; cin >> id;
        Product* p = inv.searchById(id);
        if (p) {
            p->displayDetails();           // Virtual call ? Polymorphism
            cout << "  Total Value: Rs. "
                 << fixed << setprecision(2)
                 << calculateTotalValue(*p) << "\n";  // Friend function
        } else {
            setColor(12);
            cout << "  Product not found.\n";
            resetColor();
        }
    } else if (ch == 2) {
        string keyword;
        cin.ignore();
        cout << "  Enter keyword: "; getline(cin, keyword);
        inv.searchByName(keyword);
    }
    cout << "\n  Press Enter to continue...";
    cin.ignore(); cin.get();
}

void updateMenu(Inventory& inv) {
    printHeader();
    cout << "\n  UPDATE PRODUCT\n";
    printLine();
    int id;
    cout << "  Enter Product ID to update: "; cin >> id;
    if (inv.updateProduct(id)) {
        setColor(10);
        cout << "\n  Product updated successfully!\n";
        resetColor();
    } else {
        setColor(12);
        cout << "  Product not found!\n";
        resetColor();
    }
    cout << "\n  Press Enter to continue...";
    cin.ignore(); cin.get();
}

void deleteMenu(Inventory& inv) {
    printHeader();
    cout << "\n  DELETE PRODUCT\n";
    printLine();
    int id;
    cout << "  Enter Product ID to delete: "; cin >> id;
    char confirm;
    cout << "  Are you sure? (y/n): "; cin >> confirm;
    if (confirm == 'y' || confirm == 'Y') {
        if (inv.deleteProduct(id)) {
            setColor(10);
            cout << "\n  Product deleted successfully!\n";
            resetColor();
        } else {
            setColor(12);
            cout << "  Product not found!\n";
            resetColor();
        }
    }
    cout << "\n  Press Enter to continue...";
    cin.ignore(); cin.get();
}

void reportsMenu(Inventory& inv) {
    printHeader();
    cout << "\n  REPORTS\n";
    printLine();
    cout << "  1. Low Stock Report\n  2. Total Inventory Value\n"
         << "  3. Total Product Count\n  0. Back\n  Choice: ";
    int ch; cin >> ch;
    if (ch == 1) {
        int t;
        cout << "  Enter low stock threshold [default 5]: "; cin >> t;
        inv.lowStockReport(t);
    } else if (ch == 2) {
        inv.displayTotalValue();
    } else if (ch == 3) {
        cout << "\n  Total Products in Inventory: "
             << Product::getTotalProducts() << "\n";   // Static member usage
    }
    cout << "\n  Press Enter to continue...";
    cin.ignore(); cin.get();
}

void mainMenu(Inventory& inv) {
    int choice = -1;
    while (choice != 0) {
        printHeader();
        cout << "\n  MAIN MENU\n";
        printLine();
        cout << "  1. Add Product\n"
             << "  2. Display All Products\n"
             << "  3. Search Product\n"
             << "  4. Update Product\n"
             << "  5. Delete Product\n"
             << "  6. Reports\n"
             << "  0. Exit\n";
        printLine();
        cout << "  Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addProductMenu(inv);   break;
            case 2:
                printHeader();
                cout << "\n  ALL PRODUCTS\n";
                inv.displayAll();
                cout << "\n  Press Enter to continue...";
                cin.ignore(); cin.get();
                break;
            case 3: searchMenu(inv);   break;
            case 4: updateMenu(inv);   break;
            case 5: deleteMenu(inv);   break;
            case 6: reportsMenu(inv);  break;
            case 0:
                setColor(14);
                cout << "\n  Saving data and exiting...\n";
                resetColor();
                break;
            default:
                setColor(12);
                cout << "  Invalid choice!\n";
                resetColor();
        }
    }
}


// ---------------------------------------------------------------------------
// MAIN
// ---------------------------------------------------------------------------
int main() {
    printHeader();

    // Login System
    LoginSystem ls;
    cout << "\n  Please login to continue.\n";
    cout << "  (Default: admin / admin123)\n";
    if (!ls.login()) {
        setColor(12);
        cout << "\n  Too many failed attempts. Exiting.\n";
        resetColor();
        return 1;
    }

    // Pre-load some sample data if inventory is empty
    Inventory inv("inventory.csv");
    if (inv.getCount() == 0) {
        inv.addProduct(new Electronics(101, "Samsung TV 43\"",  85000.0, 10, 24));
        inv.addProduct(new Electronics(102, "HP Laptop 15\"",  120000.0,  5, 12));
        inv.addProduct(new Electronics(103, "USB-C Charger",    2500.0,  3, 6));
        inv.addProduct(new Grocery(201, "Basmati Rice 5kg",   1200.0, 50, "12/2025"));
        inv.addProduct(new Grocery(202, "Nestle Milk 1L",      180.0,  4, "06/2025"));
        inv.addProduct(new Grocery(203, "Sunflower Oil 1L",    550.0, 20, "03/2026"));
        cout << "\n  Sample inventory loaded.\n";
    }

    // Run main menu
    mainMenu(inv);

    return 0;
}
