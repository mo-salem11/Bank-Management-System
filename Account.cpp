#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include "Account.h"
using namespace std;

void Introduction();
void MainMenu();

void Write_Account();
void Display_SpecialAccount(int);
void Display_All_Accounts();
void Delete_Account(int);
void Modify_Account(int);
void Deposit_Account(int);
void Withdraw_Account(int);

int main()
{
    int Acc_Num = 0;
    double amount; // Declaration of the amount variable
    Introduction();
    MainMenu();

    short choice;
    cout << "\n\t\tSelect Your Option [ 1 - 8 ]  : ";
    cin >> choice;

    while (choice != 8)
    {
        Acc_Num = 0;

        switch (choice)
        {
            case 1:
                Write_Account();
                break;
            case 2:
                cout << "\n\t\tEnter The Account Number: ";
                cin >> Acc_Num;
                Deposit_Account(Acc_Num);
                break;
            case 3:
                cout << "\n\t\tEnter The Account Number: ";
                cin >> Acc_Num;
                Withdraw_Account(Acc_Num);
                break;
            case 4:
                cout << "\n\t\tEnter Account Number: ";
                cin >> Acc_Num;
                Display_SpecialAccount(Acc_Num);
                break;
            case 5:
                Display_All_Accounts();
                break;
            case 6:
                cout << "\n\t\tEnter The Account Number: ";
                cin >> Acc_Num;
                Delete_Account(Acc_Num);
                break;
            case 7:
                cout << "\n\t\tEnter The Account Number: ";
                cin >> Acc_Num;
                Modify_Account(Acc_Num);
                break;
            case 8:
                exit(0);
            default:
                cout << "\a\n\n\t\tEnter a Valid Choice\n";
                MainMenu();
                break;
        }

        MainMenu();
        cout << "\n\t\tSelect Your Option [ 1 - 8 ]  : ";
        cin >> choice;
    }

    return 0;
}

void Introduction()
{
    cout << "\n\t\t#################################\n"
         << "\t\t#\t\t\t\t#\n"
         << "\t\t# \tBank\t\t\t#\n"
         << "\t\t#\t\t\t\t#\n"
         << "\t\t#\t     Management\t\t#\n"
         << "\t\t#\t\t\t\t#\n"
         << "\t\t#\t\t\tSystem\t#\n"
         << "\t\t#\t\t\t\t#\n"
         << "\t\t#################################\n\n"
         << "\t\tMade by : Mohamed Hashem\n\n"
         << "\t\tCollege : Faculty of Science Menoufia University\n\n"
         << "\t\tDate    : 30 / 6 / 2020\n\n"
         << "\t\tPress Enter To Continue ....";
    cin.get();
}

void MainMenu()
{
    system("cls");
    cout << "\n\n\t\tMain Menu\n\n"
         << "\t\t1 - New Account\n"
         << "\t\t2 - Deposit Amount\n"
         << "\t\t3 - Withdraw Amount\n"
         << "\t\t4 - Display Special Account\n"
         << "\t\t5 - Display All Accounts\n"
         << "\t\t6 - Delete an Account\n"
         << "\t\t7 - Modify an Account\n"
         << "\t\t8 - Exit\n";
}

void Write_Account()
{
    Account acc;
    ofstream outFile("Accounts.dat", ios::app | ios::binary);
    if (!outFile) {
        cerr << "\a\n\t\tFile could not open! Press any key...\n";
        cin.get();
        return;
    }
    acc.create_New_Account();
    outFile.write(reinterpret_cast<char*>(&acc), sizeof(Account));
    outFile.close();
}

void Display_SpecialAccount(int accNumber)
{
    Account acc;
    ifstream readFile("Accounts.dat", ios::binary);
    if (!readFile) {
        cerr << "\a\n\t\tFile could not open! Press any key...\n";
        cin.get();
        return;
    }

    cout << "\n\t\tAccount Details\n";
    bool found = false;
    while (readFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getId() == accNumber) {
            acc.showAccount();
            found = true;
        }
    }
    readFile.close();

    if (!found) {
        cerr << "\a\n\t\tThis record was not found! Press any key...\n";
        cin.get();
    }
}

void Display_All_Accounts()
{
    Account acc;
    ifstream file("Accounts.dat", ios::binary);
    if (!file) {
        cerr << "\a\n\t\tFile could not open! Press any key...\n";
        cin.get();
        return;
    }

    cout << "\n\t\t#################################################\n";
    cout << "\t\t# Ac.ID " << setw(8) << ' ' << "UserName " << setw(15) << ' ' << "Balance #\n";

    while (file.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        acc.showAccount();
    }
    file.close();
}

void Delete_Account(int accNumber)
{
    Account acc;
    ifstream inFile("Accounts.dat", ios::binary);
    ofstream outFile("Temp.dat", ios::binary);
    if (!inFile || !outFile) {
        cerr << "\a\n\t\tFile could not open! Press any key...\n";
        cin.get();
        return;
    }

    bool found = false;
    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getId() != accNumber) {
            outFile.write(reinterpret_cast<char*>(&acc), sizeof(Account));
        } else {
            found = true;
        }
    }
    inFile.close();
    outFile.close();

    remove("Accounts.dat");
    rename("Temp.dat", "Accounts.dat");

    if (found) {
        cout << "\n\t\tRecord Deleted...\n";
    } else {
        cerr << "\a\n\t\tThis record was not found! Press any key...\n";
        cin.get();
    }
}

void Modify_Account(int accNumber)
{
    Account acc;
    fstream file("Accounts.dat", ios::in | ios::out | ios::binary);
    if (!file) {
        cerr << "\a\n\t\tFile could not open! Press any key...\n";
        cin.get();
        return;
    }

    cout << "\n\t\tAccount Details\n";
    bool found = false;
    while (file.read(reinterpret_cast<char*>(&acc), sizeof(Account)) && !found) {
        if (acc.getId() == accNumber) {
            acc.showAccount();
            cout << "\n\t\tEnter the new details of the account\n\n";
            acc.modifyAccount();

            file.seekp(-static_cast<int>(sizeof(Account)), ios::cur);
            file.write(reinterpret_cast<char*>(&acc), sizeof(Account));

            cout << "\n\t\tRecord Modified...\n";
            found = true;
        }
    }
    file.close();

    if (!found) {
        cerr << "\a\n\t\tThis record was not found! Press any key...\n";
    }}
