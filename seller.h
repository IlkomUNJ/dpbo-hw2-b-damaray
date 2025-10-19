#ifndef SELLER_H
#define SELLER_H

#include "buyer.h"
#include "item.h"
#include <vector>
#include <string>
#include <iostream>

class seller : public Buyer {
private:
    int sellerId;
    std::string sellerName;
    std::vector<Item> items;

public:
    seller() = default;

    seller(Buyer buyer, int sellerId, const std::string& sellerName)
        : Buyer(buyer.getId(), buyer.getName(), buyer.getAccount()),
          sellerId(sellerId), sellerName(sellerName) {}

    virtual ~seller() = default;

    void addNewItem(int newId, const std::string& newName, int newQuantity, double newPrice) {
        Item newItem(newId, newName, newQuantity, newPrice);
        items.push_back(newItem);
        std::cout << "Total item sekarang: " << items.size() << std::endl;
    }

    void updateItem(int itemId, const std::string& newName, int newQuantity, double newPrice) {
        for (auto& item : items) {
            if (item.getId() == itemId) {
                item.alterItemById(itemId, newName, newQuantity, newPrice);
            }
        }
    }

    void makeItemVisibleToCustomer(int itemId) {
        for (auto& item : items) {
            if (item.getId() == itemId) {
                item.setDisplay(true);
                break;
            }
        }
    }

    void displayInventory() const {
        std::cout << "\n=== Inventory Seller: " << sellerName << " (ID seller: " << sellerId << ") ===\n";
        if (items.empty()) {
            std::cout << "(Belum ada barang di inventory)\n";
        } else {
            for (const auto& item : items) {
                std::cout << "ID: " << item.getId()
                          << " | Nama: " << item.getName()
                          << " | Jumlah: " << item.getQuantity()
                          << " | Harga: " << item.getPrice() << std::endl;
            }
        }
    }

    int getSellerId() const { return sellerId; }
    std::string getSellerName() const { return sellerName; }
    size_t getItemCount() const { return items.size(); }
    vector<Item>& getItems() { return items; }
};

#endif // SELLER_H
