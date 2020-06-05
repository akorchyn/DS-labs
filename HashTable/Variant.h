#pragma once

#include <string>

// Variant 3
struct Address
{
    std::string name;
    std::string city;
    int zip;

    static Address randomize()
    {
        Address result;
        result.name = randomize_str();
        result.city = randomize_str();
        result.zip = rand() % 1000;
        return result;
    }

    bool operator<(const Address& b) const
    {
        return std::tie(name, city, zip) < std::tie(b.name, b.city, b.zip);
    }
private:
    static std::string randomize_str()
    {
        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

        size_t len = 20;
        std::string str(20, 'f');
        for (size_t i = 0; i < len; ++i)
        {
            str[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
        }
        return str;
    }
};


// http://www.cse.yorku.ca/~oz/hash.html djb2
unsigned long hash(const std::string& str) {
    unsigned long hash = 5381;

    for (char c : str)
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}
// Knut multiplicative method
unsigned long hash(unsigned long i)
{  
    return (i * 2654435761) % 4294967296;
}

unsigned long hash(const Address& i)
{
    return hash(i.name) + hash(i.city) + hash(i.zip);
}