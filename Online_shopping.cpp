#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cctype>
#include <ctime>

using namespace std;

// Utility Functions
string trim(const string &str)
{
    size_t start = str.find_first_not_of(" \t");
    if (start == string::npos)
        return "";
    size_t end = str.find_last_not_of(" \t");
    return str.substr(start, end - start + 1);
}

string toLowerCase(const string &str)
{
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// Product Class
class Product
{
private:
    string name;
    double price;
    int stock;
    string category;

public:
    Product(string n, double p, int s, string c) : name(trim(n)), price(p), stock(s), category(trim(c)) {}
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }
    string getCategory() const { return category; }
    void setStock(int s) { stock = s; }
};

struct CartItem
{
    Product product;
    int quantity;
    CartItem(Product p, int q) : product(p), quantity(q) {}
};

// Bill Class
class Bill
{
private:
    vector<CartItem> cart;
    double discount;
    double taxRate;

public:
    Bill() : discount(0.0), taxRate(0.0) {}

    void addProduct(const Product &product, int quantity)
    {
        for (CartItem &item : cart)
        {
            if (toLowerCase(item.product.getName()) == toLowerCase(product.getName()))
            {
                item.quantity += quantity;
                return;
            }
        }
        cart.push_back(CartItem(product, quantity));
    }

    void removeProduct(const string &productName, vector<Product> &productList)
    {
        for (auto it = cart.begin(); it != cart.end(); ++it)
        {
            if (toLowerCase(it->product.getName()) == toLowerCase(trim(productName)))
            {
                auto productIt = find_if(productList.begin(), productList.end(),
                                         [&productName](const Product &p)
                                         {
                                             return toLowerCase(trim(p.getName())) == toLowerCase(trim(productName));
                                         });

                if (productIt != productList.end())
                {
                    productIt->setStock(productIt->getStock() + it->quantity);
                }
                cart.erase(it);
                cout << productName << " removed from cart.\n";
                return;
            }
        }
        cout << "Product not found in cart.\n";
    }

    void displayBill()
    {
        ofstream receipt("receipt.txt");
        if (!receipt)
        {
            cerr << "Error creating receipt file!\n";
            return;
        }

        time_t now = time(0);
        char *dt = ctime(&now);

        // Receipt Header
        receipt << "============================================\n";
        receipt << "           ASHISH STORE RECEIPT          \n";
        receipt << "============================================\n";
        receipt << "Date: " << dt;
        receipt << "--------------------------------------------\n";
        receipt << left << setw(25) << "Item" << setw(8) << "Qty"
                << setw(10) << "Price" << "Total\n";

        double subtotal = 0.0;
        cout << "\n--- Current Bill ---\n";

        for (const CartItem &item : cart)
        {
            double itemPrice = item.product.getPrice();
            double itemTotal = itemPrice * item.quantity;
            subtotal += itemTotal;

            // Console Output
            cout << left << setw(20) << item.product.getName()
                 << "x" << setw(3) << item.quantity
                 << "Rs." << fixed << setprecision(2) << itemTotal << endl;

            // Receipt Output
            receipt << left << setw(25) << item.product.getName()
                    << setw(8) << ("x" + to_string(item.quantity))
                    << "Rs." << fixed << setprecision(2) << setw(10) << itemPrice
                    << "Rs." << itemTotal << endl;
        }

        double discountAmt = subtotal * (discount / 100);
        double taxableAmt = subtotal - discountAmt;
        double taxAmt = taxableAmt * (taxRate / 100);
        double total = taxableAmt + taxAmt;

        // Console Totals
        cout << "\nSubtotal: Rs." << subtotal << endl;
        cout << "Discount (" << discount << "%): Rs." << discountAmt << endl;
        cout << "Tax (" << taxRate << "%): Rs." << taxAmt << endl;
        cout << "Total: Rs." << total << endl;

        // Receipt Totals
        receipt << "--------------------------------------------\n";
        receipt << left << setw(35) << "Subtotal:" << "Rs." << subtotal << endl;
        receipt << left << setw(35) << "Discount (" + to_string((int)discount) + "%):"
                << "Rs." << discountAmt << endl;
        receipt << left << setw(35) << "Tax (" + to_string((int)taxRate) + "%):"
                << "Rs." << taxAmt << endl;
        receipt << left << setw(35) << "TOTAL:" << "Rs." << total << endl;
        receipt << "============================================\n";
        receipt.close();

        cout << "\nDetailed receipt saved to receipt.txt\n";
    }

    void setDiscount(double d) { discount = d; }
    void setTaxRate(double t) { taxRate = t; }

    void saveCart(const string &filename)
    {
        ofstream file(filename);
        if (!file)
            return;

        for (const CartItem &item : cart)
        {
            file << item.product.getName() << "," << item.quantity << endl;
        }
        file.close();
        cout << "Cart saved successfully!\n";
    }

    void loadCart(const string &filename, vector<Product> &productList)
    {
        ifstream file(filename);
        if (!file)
            return;

        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string name;
            int quantity;
            if (getline(ss, name, ',') && ss >> quantity)
            {
                auto it = find_if(productList.begin(), productList.end(),
                                  [&name](const Product &p)
                                  {
                                      return toLowerCase(trim(p.getName())) == toLowerCase(trim(name));
                                  });

                if (it != productList.end() && it->getStock() >= quantity)
                {
                    it->setStock(it->getStock() - quantity);
                    addProduct(*it, quantity);
                }
            }
        }
        file.close();
        cout << "Cart loaded successfully!\n";
    }

    bool isEmpty() const { return cart.empty(); }
};

// Wishlist Class
class Wishlist
{
private:
    vector<Product> products;

public:
    void addProduct(const Product &product)
    {
        products.push_back(product);
        cout << product.getName() << " added to wishlist.\n";
    }

    void displayWishlist()
    {
        cout << "\n--- Wishlist ---\n";
        for (const Product &p : products)
        {
            cout << p.getName() << " - Rs." << p.getPrice()
                 << " (" << p.getCategory() << ")\n";
        }
    }

    void moveToCart(Bill &cart, vector<Product> &productList)
    {
        for (const Product &p : products)
        {
            auto it = find_if(productList.begin(), productList.end(),
                              [&p](const Product &prod)
                              {
                                  return toLowerCase(trim(prod.getName())) == toLowerCase(trim(p.getName()));
                              });
            if (it != productList.end() && it->getStock() >= 1)
            {
                cart.addProduct(*it, 1);
                it->setStock(it->getStock() - 1);
            }
            else
            {
                cout << "Product " << p.getName() << " is out of stock.\n";
            }
        }
        products.clear();
        cout << "Wishlist moved to cart.\n";
    }
};

// Read Products from CSV
vector<Product> readCSV(const string &filename)
{
    vector<Product> productList;
    ifstream file(filename);

    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string name, category;
            double price;
            int stock;
            if (getline(ss, name, ',') && (ss >> price) &&
                (ss.ignore(), ss >> stock) && getline(ss, category))
            {
                productList.emplace_back(name, price, stock, category);
            }
        }
        file.close();
    }
    return productList;
}

// Menu System
void displayMenu()
{
    cout << "\n--- A.K.U Store Management System ---\n";
    cout << "1. View All Products\n";
    cout << "2. Add Product to Cart\n";
    cout << "3. Remove Product from Cart\n";
    cout << "4. View Cart\n";
    cout << "5. Save Cart\n";
    cout << "6. Load Cart\n";
    cout << "7. Add Product to Wishlist\n";
    cout << "8. View Wishlist\n";
    cout << "9. Move Wishlist to Cart\n";
    cout << "10. Generate Bill\n";
    cout << "11. Exit\n";
    cout << "Enter your choice: ";
}

int main()
{
    vector<Product> productList = readCSV("products.csv");
    Bill cart;
    Wishlist wishlist;

    int choice;
    do
    {
        displayMenu();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
        {
            cout << "\n--- Available Products ---\n";
            for (const Product &p : productList)
            {
                cout << p.getName() << " - Rs." << p.getPrice()
                     << " (" << p.getCategory() << ") Stock: " << p.getStock() << "\n";
            }
            break;
        }
        case 2:
        {
            string productName;
            int quantity;
            cout << "Enter product name to add to cart: ";
            getline(cin, productName);
            cout << "Enter quantity: ";
            cin >> quantity;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            auto it = find_if(productList.begin(), productList.end(),
                              [&productName](const Product &p)
                              {
                                  return toLowerCase(trim(p.getName())) == toLowerCase(trim(productName));
                              });

            if (it != productList.end() && it->getStock() >= quantity)
            {
                cart.addProduct(*it, quantity);
                it->setStock(it->getStock() - quantity);
                cout << productName << " added to cart.\n";
            }
            else
            {
                cout << "Product not available or insufficient stock.\n";
            }
            break;
        }
        case 3:
        {
            string productName;
            cout << "Enter product name to remove from cart: ";
            getline(cin, productName);
            cart.removeProduct(productName, productList);
            break;
        }
        case 4:
        {
            if (cart.isEmpty())
            {
                cout << "Cart is empty.\n";
            }
            else
            {
                cart.displayBill();
            }
            break;
        }
        case 5:
        {
            cart.saveCart("saved_cart.txt");
            break;
        }
        case 6:
        {
            cart.loadCart("saved_cart.txt", productList);
            break;
        }
        case 7:
        {
            string productName;
            cout << "Enter product name to add to wishlist: ";
            getline(cin, productName);

            auto it = find_if(productList.begin(), productList.end(),
                [&productName](const Product &p)
                {
                    return toLowerCase(trim(p.getName())) == toLowerCase(trim(productName));
                    });

            if (it != productList.end())
            {
                wishlist.addProduct(*it);
            }
            else
            {
                cout << "Product not found.\n";
            }
            break;
        }
        case 8:
        {
            wishlist.displayWishlist();
            break;
        }
        case 9:
        {
            wishlist.moveToCart(cart, productList);
            break;
        }
        case 10:
        {
            if (cart.isEmpty())
            {
                cout << "Cart is empty. Cannot generate bill.\n";
            }
            else
            {
                double discount, taxRate;
                cout << "Enter discount percentage: ";
                cin >> discount;
                cout << "Enter tax rate percentage: ";
                cin >> taxRate;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cart.setDiscount(discount);
                cart.setTaxRate(taxRate);
                cart.displayBill();
            }
            break;
        }
        case 11:
        {
            cout << "Exiting...\n";
            break;
        }
        default:
        {
            cout << "Invalid choice. Please try again.\n";
            break;
        }
        }
    } while (choice != 11);

    return 0;
}