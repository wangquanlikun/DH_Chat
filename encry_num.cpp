#include "encry_num.h"

Encry_num::Encry_num() {
    reset_num();
}

void Encry_num::reset_num() {
    this->p = 2;
    this->g = 2;
    this->private_key = 1;
    this->public_key = 0;
    this->shared_key = 0;
}

unsigned long long Encry_num::mod_operation(unsigned long long A, unsigned long long n, unsigned long long p) const {
    unsigned long long result = 1;
    A = A % p;

    while (n > 0) {
        if (n % 2 == 1) {
            result = (result * A) % p;
        }
        n = n >> 1;
        A = (A * A) % p;
    }

    return result;
}

bool Encry_num::is_qualified_p(const unsigned long long temp_p) const {
    if (temp_p <= 1) {
        return false;
    }
    if (temp_p <= 3) {
        return true;
    }
    if (temp_p % 2 == 0 || temp_p % 3 == 0) {
        return false;
    }

    for (unsigned long long i = 5; i * i <= temp_p; i += 6) {
        if (temp_p % i == 0 || temp_p % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

bool Encry_num::set_p(unsigned long long temp_p) {
    if(is_qualified_p(temp_p)){
        this->p = temp_p;
        return true;
    }
    else
        return false;
}

void Encry_num::set_public_key() {
    this->public_key = mod_operation(this->g, this->private_key, this->p);
}

void Encry_num::set_share_key(unsigned long long others_pubkey){
    this->shared_key = mod_operation(others_pubkey, this->private_key, this->p);
}
