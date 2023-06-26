#include "base.h"
#include "hash_store.h"

#include <iostream>

int main() {
    BaseStore* store = new HashStore("hash_store.txt");

    store->Load();
    store->Set("store", "hash");
    std::cout << store->Get("store") << std::endl;

    store->Set("hello", "world");
    std::cout << store->Get("hello") << std::endl;

    store->Save();

    delete store;

    return 0;
}
