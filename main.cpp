#include<iostream>
#include<sqlite3.h>
#include <string>

using namespace std;

struct CurrencyData {

    double MainMoney;
    double TargetMoney;
    bool isMain;
};


int callback(void *data, int argc, char **argv, char **ColName) {

    CurrencyData* currencyData = static_cast<CurrencyData*>(data);

    if(argc > 0 && argv[0]) {
        double price = stod(argv[0]);
        if(currencyData->isMain) {

            currencyData->MainMoney = price;

        } else {

            currencyData->TargetMoney = price;

        }
    } else {
        
        cout << "Currency not found!" << endl;

    }

        return 0;

}

int main() {

    sqlite3 *db;
    char *ErrMsg = nullptr;
    string MainCurrency;
    string TargetCurrency;
    string Money;

    int rc = sqlite3_open("Database.db", &db);
    
    if(rc != SQLITE_OK) {

        cout << "Error: \n" << sqlite3_errmsg(db);
        return rc;

    }

    cout << "Select your currency\n";
    getline(cin, MainCurrency);
    cout << "Select which currency you want to convert\n";
    getline(cin, TargetCurrency);
    cout << "How much do you have?\n";
    getline(cin, Money);

    double money = stod(Money);

    CurrencyData currencyData;

    currencyData.isMain = true;
    string sqlMain = "SELECT price FROM currency WHERE country ='" + MainCurrency + "'OR abreviation ='" + MainCurrency + "';";

    rc = sqlite3_exec(db, sqlMain.c_str(), callback, &currencyData, &ErrMsg);

    if (rc != SQLITE_OK) {

        cerr << "SQL Error: " << ErrMsg << endl;
        sqlite3_free(ErrMsg);

    }

    currencyData.isMain = false;
    string sqlTarget = "SELECT price FROM currency WHERE country ='" + TargetCurrency + "'OR abreviation ='" + TargetCurrency + "';";

    rc = sqlite3_exec(db, sqlTarget.c_str(), callback, &currencyData, &ErrMsg);

    if (rc != SQLITE_OK) {

        cerr << "SQL Error: " << ErrMsg << endl;
        sqlite3_free(ErrMsg);

    }

    if(currencyData.MainMoney == 0.0 || currencyData.TargetMoney == 0.0){

        cout << "Not found in your database!\n";

    } else {

        double result = (money / currencyData.MainMoney) * currencyData.TargetMoney;

        cout << "Converted amount: " << result << endl;

    }

    sqlite3_close(db);
    return 0;

}