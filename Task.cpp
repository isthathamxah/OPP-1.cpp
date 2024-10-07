#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

class Bank_Account {
private:
    string account_num;
    string account_holder_name;
    double balance;

public:
    Bank_Account(string acc_num = "", string acc_holder_name = "", double balance = 0.0)
        : account_num(acc_num), account_holder_name(acc_holder_name), balance(balance) {}

    string getAccountNum() const { return account_num; }
    string getAccountHolderName() const { return account_holder_name; }
    double getBalance() const { return balance; }

    void Deposit(double amount) {
        balance += amount;
        cout << "Deposited: " << amount << " New balance: " << balance << endl;
    }

    void Withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            cout << "Withdrawn amount: " << amount << " New balance: " << balance << endl;
        }
        else {
            cout << "Insufficient balance!" << endl;
        }
    }

    void check_balance() const {
        cout << "Your Current Balance is: " << balance << endl;
    }

    void Save_info(fstream& myfile) const {
        if (myfile.is_open()) {
            myfile << account_num << " " << account_holder_name << " " << balance << endl;
        }
        else {
            cout << "Unable to open file!" << endl;
        }
    }
};

class Bank_System {
private:
    vector<Bank_Account> accounts;
    Bank_Account* loggedInUser = nullptr;

public:
    Bank_System() {
        Load_all_info();
    }

    void Load_all_info() {
        fstream myfile;
        myfile.open("Bank_Info.txt", ios::in);
        if (myfile.is_open()) {
            string acc_num, acc_holder_name;
            double balance;
            while (myfile >> acc_num >> acc_holder_name >> balance) {
                accounts.emplace_back(acc_num, acc_holder_name, balance);
            }
            myfile.close();
        }
    }

    void Save_all_info() const {
        fstream myfile;
        myfile.open("Bank_Info.txt", ios::out); // Open in write mode to overwrite existing data
        if (myfile.is_open()) {
            for (const auto& account : accounts) {
                account.Save_info(myfile);
            }
            myfile.close();
        }
        else {
            cout << "Unable to open file for saving!" << endl;
        }
    }

    void SignUp() {
        string acc_num, acc_holder_name;
        double initial_balance;
        cout << "Enter a new account number: ";
        cin >> acc_num;
        cout << "Enter your name: ";
        cin >> acc_holder_name;
        cout << "Enter initial deposit amount: ";
        cin >> initial_balance;

        accounts.emplace_back(acc_num, acc_holder_name, initial_balance);
        cout << "Account created successfully!" << endl;
    }

    bool Login() {
        string acc_num;
        cout << "Enter your account number to login: ";
        cin >> acc_num;

        for (auto& account : accounts) {
            if (account.getAccountNum() == acc_num) {
                loggedInUser = &account;
                cout << "Login successful! Welcome, " << loggedInUser->getAccountHolderName() << "!" << endl;
                return true;
            }
        }
        cout << "Account not found!" << endl;
        return false;
    }

    void Display_all_accounts() const {
        if (accounts.empty()) {
            cout << "No accounts available to display." << endl;
        }
        else {
            cout << "\nAll Bank Accounts:\n";
            for (const auto& account : accounts) {
                cout << "Account Number: " << account.getAccountNum()
                    << " | Account Holder Name: " << account.getAccountHolderName()
                    << " | Balance: " << account.getBalance() << endl;
            }
        }
    }

    void DisplayMenu() {
        if (!loggedInUser) {
            cout << "Please login to access your account." << endl;
            return;
        }

        int choice;
        double amount;
        do {
            cout << "\n1. Deposit\n2. Withdraw\n3. Check Balance\n4. Save Account Information\n5. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                cout << "Enter deposit amount: ";
                cin >> amount;
                loggedInUser->Deposit(amount);
                break;
            case 2:
                cout << "Enter withdrawal amount: ";
                cin >> amount;
                loggedInUser->Withdraw(amount);
                break;
            case 3:
                loggedInUser->check_balance();
                break;
            case 4:
                Save_all_info();
                cout << "Account information saved successfully!" << endl;
                break;
            case 5:
                loggedInUser = nullptr;
                cout << "Logged out successfully!" << endl;
                return;
            default:
                cout << "Invalid choice! Please try again." << endl;
            }
        } while (true);
    }

    void Main_Menu() {
        int choice;
        do {
            cout << "\n1. Sign Up\n2. Login\n3. Display All Accounts\n4. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                SignUp();
                break;
            case 2:
                if (Login()) {
                    DisplayMenu();
                }
                break;
            case 3:
                Display_all_accounts();
                break;
            case 4:
                Save_all_info();
                cout << "Thank you for using the bank system!" << endl;
                return;
            default:
                cout << "Invalid choice! Please try again." << endl;
            }
        } while (true);
    }

    ~Bank_System() {
        Save_all_info();
    }
};

int main() {
    Bank_System bankSystem;
    bankSystem.Main_Menu();
    return 0;
}
