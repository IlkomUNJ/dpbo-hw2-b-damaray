#include <iostream>
#include <vector>
#include <string>
#include <limits>

#include "bank_customer.h"
#include "buyer.h"
#include "seller.h"
#include "transaction.h"

using namespace std;

vector<Buyer*> buyers;
vector<seller*> sellers;
vector<BankCustomer*> customers;
vector<Transaction> transactions;

void sellerMenu(seller* s);
void buyItem(Buyer* b);

int readInt(const string &prompt) {
    int x;
    while (true) {
        cout << prompt;
        if (cin >> x) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        } else {
            cout << "Input tidak valid. Masukkan angka.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

double readDouble(const string &prompt) {
    double x;
    while (true) {
        cout << prompt;
        if (cin >> x) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        } else {
            cout << "Input tidak valid. Masukkan angka (desimal boleh pakai titik).\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

string readLine(const string &prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

void registerUser() {
    int id = static_cast<int>(customers.size()) + 1;
    string name = readLine("Masukkan nama : ");
    double initialDeposit = readDouble("Setor awal: ");

    BankCustomer* cust = new BankCustomer(id, name, initialDeposit);
    customers.push_back(cust);

    Buyer* b = new Buyer(id, name, *cust);
    buyers.push_back(b);

    cout << "Akun Buyer berhasil dibuat dengan ID: " << id << endl;
}

Buyer* loginUser() {
    int id = readInt("Masukkan ID akun: ");
    for (auto bptr : buyers) {
        if (bptr->getId() == id) {
            cout << "Login berhasil. Halo, " << bptr->getName() << "!\n";
            return bptr;
        }
    }
    cout << "Akun tidak ditemukan.\n";
    return nullptr;
}

void sellerMenu(seller* s) {
    int choice;
    do {
        cout << "\n=== Seller Menu ===\n";
        cout << "1. Tambah Barang ke Inventory\n";
        cout << "2. Lihat Semua Barang\n";
        cout << "3. Logout\n";
        choice = readInt("Pilihan: ");

        if (choice == 1) {
            int id = readInt("Masukkan ID Barang: ");
            string name = readLine("Nama Barang: ");
            int qty = readInt("Jumlah Barang: ");
            double price = readDouble("Harga Barang: ");

            s->addNewItem(id, name, qty, price);
            cout << "Barang berhasil ditambahkan ke inventory!\n";
        } else if (choice == 2) {
            s->displayInventory();
        } else if (choice == 3) {
            cout << "Logout seller...\n";
        } else {
            cout << "Pilihan tidak dikenal.\n";
        }
    } while (choice != 3);
}

void buyItem(Buyer* buyer) {
    if (sellers.empty()) {
        cout << "Belum ada seller yang menjual barang.\n";
        return;
    }

    cout << "\n=== Daftar Seller ===\n";
    for (auto s : sellers)
        cout << s->getSellerId() << " - " << s->getSellerName() << endl;

    int sid = readInt("Masukkan ID seller yang ingin dibeli: ");
    seller* chosen = nullptr;
    for (auto s : sellers)
        if (s->getSellerId() == sid) chosen = s;

    if (!chosen) {
        cout << "Seller tidak ditemukan.\n";
        return;
    }

    chosen->displayInventory();
    int itemId = readInt("Masukkan ID barang: ");
    int qty = readInt("Jumlah yang ingin dibeli: ");

    // Cari item
    Item* selected = nullptr;
    for (auto& item : chosen->getItems()) {
        if (item.getId() == itemId) {
            selected = &item;
            break;
        }
    }

    if (!selected) {
        cout << "Item tidak ditemukan.\n";
        return;
    }

    double total = selected->getPrice() * qty;
    cout << "Total harga: $" << total << endl;

    // Proses pembayaran via bank
    if (buyer->getAccount().getBalance() < total) {
        cout << "Saldo tidak cukup!\n";
        return;
    }

    buyer->getAccount().withdrawBalance(total);
    chosen->getAccount().addBalance(total);
    selected->setQuantity(selected->getQuantity() - qty);

    int tid = static_cast<int>(transactions.size()) + 1;
    Transaction t(tid, buyer->getName(), chosen->getSellerName(),
                  selected->getName(), qty, total, TransactionStatus::PAID);
    transactions.push_back(t);

    cout << "Pembelian berhasil!\n";
}

void buyerMenu(Buyer* b) {
    int choice;
    do {
        cout << "\n=== Buyer Menu ===\n";
        cout << "1. Cek Akun Bank\n";
        cout << "2. Deposit\n";
        cout << "3. Withdraw\n";
        cout << "4. Upgrade jadi Seller\n";
        cout << "5. Beli barang dari Seller\n";
        cout << "6. Logout\n";
        choice = readInt("Pilihan: ");

        if (choice == 1) {
            b->getAccount().printInfo();
        } else if (choice == 2) {
            double amt = readDouble("Jumlah deposit: ");
            b->getAccount().addBalance(amt);
            cout << "Deposited.\n";
        } else if (choice == 3) {
            double amt = readDouble("Jumlah withdraw: ");
            if (b->getAccount().withdrawBalance(amt)) {
                cout << "Withdraw sukses.\n";
            }
        } else if (choice == 4) {
            int sid = static_cast<int>(sellers.size()) + 1;
            string storeName = readLine("Masukkan nama toko: ");
            seller* s_ptr = new seller(*b, sid, storeName);
            sellers.push_back(s_ptr);
            cout << "Akun Seller berhasil dibuat! (ID seller: " << sid << ")\n";
            sellerMenu(s_ptr);
        } else if (choice == 5) {
            buyItem(b);
        } else if (choice == 6) {
            cout << "Logout buyer...\n";
        } else {
            cout << "Pilihan tidak dikenal.\n";
        }
    } while (choice != 6);
}

// ===== Tambahan Fitur Quiz =====

void viewAllBuyers() {
    cout << "\n=== Daftar Semua Buyers ===\n";
    if (buyers.empty()) {
        cout << "(Belum ada buyer yang terdaftar)\n";
        return;
    }
    for (auto b : buyers) {
        cout << "ID: " << b->getId()
             << " | Nama: " << b->getName()
             << " | Saldo: $" << b->getAccount().getBalance()
             << endl;
    }
}

void viewAllSellers() {
    cout << "\n=== Daftar Semua Sellers ===\n";
    if (sellers.empty()) {
        cout << "(Belum ada seller yang terdaftar)\n";
        return;
    }
    for (auto s : sellers) {
        cout << "Seller ID: " << s->getSellerId()
             << " | Nama Seller: " << s->getSellerName()
             << " | Jumlah Item: " << s->getItemCount()
             << endl;
    }
}

void seekUserById() {
    int id = readInt("Masukkan ID yang ingin dicari: ");
    bool found = false;

    for (auto b : buyers) {
        if (b->getId() == id) {
            cout << "=== Data Buyer ===\n";
            cout << "ID: " << b->getId() << " | Nama: " << b->getName()
                 << " | Saldo: $" << b->getAccount().getBalance() << endl;
            found = true;
        }
    }
    for (auto s : sellers) {
        if (s->getSellerId() == id) {
            cout << "=== Data Seller ===\n";
            s->displayInventory();
            found = true;
        }
    }

    if (!found) cout << "Tidak ditemukan buyer/seller dengan ID tersebut.\n";
}

void removeUserById() {
    int id = readInt("Masukkan ID user yang ingin dihapus: ");

    // hapus seller jika ada
    for (auto it = sellers.begin(); it != sellers.end(); ++it) {
        if ((*it)->getSellerId() == id) {
            delete *it;
            sellers.erase(it);
            cout << "Seller dengan ID " << id << " telah dihapus.\n";
            return;
        }
    }

    // hapus buyer jika ada
    for (auto it = buyers.begin(); it != buyers.end(); ++it) {
        if ((*it)->getId() == id) {
            delete *it;
            buyers.erase(it);
            cout << "Buyer dengan ID " << id << " telah dihapus.\n";
            return;
        }
    }

    // hapus akun bank jika ada
    for (auto it = customers.begin(); it != customers.end(); ++it) {
        if ((*it)->getId() == id) {
            delete *it;
            customers.erase(it);
            cout << "Bank account ID " << id << " juga dihapus.\n";
            return;
        }
    }

    cout << "User dengan ID " << id << " tidak ditemukan.\n";
}

void viewTransactions() {
    cout << "\n=== Daftar Transaksi ===\n";
    if (transactions.empty()) {
        cout << "(Belum ada transaksi)\n";
        return;
    }
    for (const auto &t : transactions) {
        t.print();
    }
}

int main() {
    cout << "Program mulai...\n";
    int choice;
    do {
        cout << "\n=== Main Menu ===\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. View All Buyers\n";
        cout << "4. View All Sellers\n";
        cout << "5. Search Buyer/Seller by ID\n";
        cout << "6. Remove Buyer/Seller\n";
        cout << "7. View All Transactions\n";
        cout << "8. Exit\n";

        choice = readInt("Pilihan: ");

        if (choice == 1) {
            registerUser();
        } else if (choice == 2) {
            Buyer* b = loginUser();
            if (b) buyerMenu(b);
        } else if (choice == 3) {
            viewAllBuyers();
        } else if (choice == 4) {
            viewAllSellers();
        } else if (choice == 5) {
            seekUserById();
        } else if (choice == 6) {
            removeUserById();
        } else if (choice == 7) {
            viewTransactions();
        } else if (choice == 8) {
            cout << "Exiting program...\n";
        } else {
            cout << "Pilihan tidak dikenal.\n";
        }
    } while (choice != 8);

    for (auto p : buyers) delete p;
    for (auto p : sellers) delete p;
    for (auto p : customers) delete p;

    return 0;
}
