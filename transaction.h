#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>
#include <iostream>
using namespace std;

enum class TransactionStatus { PAID, COMPLETE, CANCELLED };

class Transaction {
private:
    int transactionId;
    string buyerName;
    string sellerName;
    string itemName;
    int quantity;
    double totalPrice;
    string date;
    TransactionStatus status;

public:
    Transaction(int id, const string& buyer, const string& seller,
                const string& item, int qty, double total, TransactionStatus st)
        : transactionId(id), buyerName(buyer), sellerName(seller),
          itemName(item), quantity(qty), totalPrice(total), status(st) {
        // Buat tanggal otomatis
        time_t now = time(0);
        char buf[80];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        date = buf;
    }

    void setStatus(TransactionStatus newStatus) { status = newStatus; }

    void print() const {
        cout << "ID Transaksi: " << transactionId << endl;
        cout << "Tanggal: " << date << endl;
        cout << "Buyer: " << buyerName << " | Seller: " << sellerName << endl;
        cout << "Item: " << itemName << " (" << quantity << "x)"
             << " | Total: $" << totalPrice << endl;
        cout << "Status: ";
        switch (status) {
            case TransactionStatus::PAID: cout << "PAID"; break;
            case TransactionStatus::COMPLETE: cout << "COMPLETE"; break;
            case TransactionStatus::CANCELLED: cout << "CANCELLED"; break;
        }
        cout << "\n----------------------\n";
    }
};

#endif
