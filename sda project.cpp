#include <iostream>
#include <string>
#include <sstream>
using namespace std;

// === 1. Singleton ===
class Logger {
private:
    static Logger* instance;
    Logger() {}
public:
    static Logger* getInstance() {
        if (!instance) instance = new Logger();
        return instance;
    }
    void log(string msg) {
        cout << "[LOG]: " << msg << endl;
    }
};
Logger* Logger::instance = NULL;

// === Product & Cart ===
class Product {
public:
    string name;
    double price;
    int stock;
    Product() {}
    Product(string n, double p, int s) : name(n), price(p), stock(s) {}
    void display() {
        cout << name << " | $" << price << " | Stock: " << stock << endl;
    }
};

class CartItem {
public:
    Product product;
    int quantity;
    CartItem() {}
    CartItem(Product p, int q) : product(p), quantity(q) {}
    double getTotal() { return quantity * product.price; }
    void display() { cout << product.name << " x" << quantity << " = $" << getTotal() << endl; }
};

#define MAX_PRODUCTS 20
#define MAX_CART 20
Product inventory[MAX_PRODUCTS];
int productCount = 0;
CartItem cart[MAX_CART];
int cartCount = 0;
// === 2. Factory Method ===
class PaymentMethod {
public:
    virtual void pay(double amount) = 0;
    virtual ~PaymentMethod() {}
};

class CreditCard : public PaymentMethod {
public:
    void pay(double amount) {
        cout << "Paid $" << amount << " via Credit Card.\n";
    }
};

class PayPal : public PaymentMethod {
public:
    void pay(double amount) {
        cout << "Paid $" << amount << " via PayPal.\n";
    }
};

class PaymentFactory {
public:
    static PaymentMethod* create(string type) {
        if (type == "CreditCard") return new CreditCard();
        else if (type == "PayPal") return new PayPal();
        return NULL;
    }
};

// === 3. Builder ===
class Order {
public:
    string customer;
    double total;
    void display() {
        cout << "\nOrder for: " << customer << "\n";
        for (int i = 0; i < cartCount; i++) cart[i].display();
        cout << "Total: $" << total << "\n";
    }
};

class OrderBuilder {
    Order order;
public:
    void setCustomer(string name) { order.customer = name; }
    void buildTotal() {
        order.total = 0;
        for (int i = 0; i < cartCount; i++) order.total += cart[i].getTotal();
    }
    Order getOrder() { return order; }
};

// === 4. Strategy ===
class ShippingStrategy {
public:
    virtual string method() = 0;
};

class StandardShipping : public ShippingStrategy {
public:
    string method() { return "Standard Shipping"; }
};

// === 5. Command ===
class Command {
public:
    virtual void execute() = 0;
};

class AddToCartCommand : public Command {
    int id, qty;
public:
    AddToCartCommand(int i, int q) : id(i), qty(q) {}
    void execute() {
        if (cartCount >= MAX_CART) {
            Logger::getInstance()->log("Cart is full.");
            return;
        }

        if (id > 0 && id <= productCount && inventory[id - 1].stock >= qty) {
            cart[cartCount++] = CartItem(inventory[id - 1], qty);
            inventory[id - 1].stock -= qty;
            Logger::getInstance()->log("Item added to cart.");
        }
        else {
            Logger::getInstance()->log("Invalid product or insufficient stock.");
        }
    }
};

// === 6. Observer ===
class Observer {
public:
    virtual void update(string msg) = 0;
};

class EmailNotifier : public Observer {
public:
    void update(string msg) {
        cout << "[Email] " << msg << endl;
    }
};

class Subject {
    Observer* observers[5];
    int count = 0;
public:
    void attach(Observer* o) {
        if (count < 5) observers[count++] = o;
    }
    void notify(string msg) {
        for (int i = 0; i < count; i++) observers[i]->update(msg);
    }
};

// === 7. State ===
class OrderState {
public:
    virtual void handle(Subject* s) = 0;
};

class PaidState : public OrderState {
public:
    void handle(Subject* s) {
        s->notify("Order marked as PAID.");
    }
};

// === 8. Template Method ===
class CheckoutTemplate {
public:
    void checkout(Order order, PaymentMethod* pmt) {
        order.display();
        applyDiscount(order);
        pay(order, pmt);
    }
    virtual void applyDiscount(Order& o) {
        Logger::getInstance()->log("No discount applied.");
    }
    virtual void pay(Order& o, PaymentMethod* pmt) = 0;
};

class BasicCheckout : public CheckoutTemplate {
public:
    void pay(Order& o, PaymentMethod* pmt) {
        pmt->pay(o.total);
    }
};

// === 9. Facade ===
class CheckoutFacade {
public:
    void complete(Order order, PaymentMethod* payment) {
        BasicCheckout process;
        process.checkout(order, payment);
        Logger::getInstance()->log("Order complete.");
    }
};

// === 10. Composite ===
class Category {
    string name;
    Category* children[3];
    int count = 0;
public:
    Category(string n) : name(n) {}
    void add(Category* c) { if (count < 3) children[count++] = c; }
    void display(string prefix = "") {
        cout << prefix << name << endl;
        for (int i = 0; i < count; i++) children[i]->display(prefix + "  ");
    }
};

// === 11. Adapter ===
class ExternalGateway {
public:
    void externalPay(float amt) {
        cout << "[External Gateway] Paid $" << amt << endl;
    }
};

class ExternalAdapter : public PaymentMethod {
    ExternalGateway* gateway;
public:
    ExternalAdapter(ExternalGateway* g) : gateway(g) {}
    void pay(double amt) {
        gateway->externalPay((float)amt);
    }
};

// === Menus ===
void adminMenu() {
    string pass;
    cout << "Admin password: "; getline(cin, pass);
    if (pass != "admin123") return;

    int choice;
    do {
        cout << "\nAdmin Menu\n1. Add Product\n2. View Products\n3. Back\nChoice: ";
        cin >> choice; cin.ignore();
        if (choice == 1 && productCount < MAX_PRODUCTS) {
            string name; double price; int stock;
            cout << "Name: "; getline(cin, name);
            cout << "Price: "; cin >> price;
            cout << "Stock: "; cin >> stock; cin.ignore();
            inventory[productCount++] = Product(name, price, stock);
        }
        else if (choice == 2) {
            for (int i = 0; i < productCount; i++) {
                cout << i + 1 << ". "; inventory[i].display();
            }
        }
    } while (choice != 3);
}

void userMenu() {
    string name; cout << "Your name: "; getline(cin, name);
    int choice;
    do {
        cout << "\nUser Menu\n1. View Products\n2. Add to Cart\n3. View Cart\n4. Checkout\n5. Back\nChoice: ";
        cin >> choice; cin.ignore();
        if (choice == 1) {
            for (int i = 0; i < productCount; i++) {
                cout << i + 1 << ". "; inventory[i].display();
            }
        }
        else if (choice == 2) {
            int id, qty;
            cout << "Product #: "; cin >> id;
            cout << "Quantity: "; cin >> qty; cin.ignore();
            AddToCartCommand cmd(id, qty);
            cmd.execute();
        }
        else if (choice == 3) {
            for (int i = 0; i < cartCount; i++) cart[i].display();
        }
        else if (choice == 4) {
            OrderBuilder builder;
            builder.setCustomer(name);
            builder.buildTotal();
            Order order = builder.getOrder();

            string method;
            cout << "Pay with (CreditCard/PayPal/External): ";
            getline(cin, method);

            PaymentMethod* pmt = NULL;

            if (method == "External")
                pmt = new ExternalAdapter(new ExternalGateway());
            else
                pmt = PaymentFactory::create(method);

            if (pmt == NULL) {
                Logger::getInstance()->log("Invalid payment method.");
            }
            else {
                CheckoutFacade facade;
                facade.complete(order, pmt);

                Subject sub;
                EmailNotifier email;
                sub.attach(&email);

                PaidState state;
                state.handle(&sub);

                delete pmt;
                cartCount = 0;
            }
        }
    } while (choice != 5);
}

// === Main ===
int main() {
    Logger::getInstance()->log("Online Shopping System");
    int choice;
    do {
        cout << "\n\tMain Menu\n\t1. Admin\n\t2. User\n\t3. Exit\n\tChoice: ";
        cin >> choice; cin.ignore();
        if (choice == 1) adminMenu();
        else if (choice == 2) userMenu();
    } while (choice != 3);
    return 0;
}

