// Book.cpp
#include "Book.h"

string Book::pack() const {
    string data = to_string(id) + "|" + title + "|" + authors + "|" +
                       to_string(year) + "|" + categories;
    int size = data.length() + sizeof(int);

    string buffer;
    buffer.resize(sizeof(int));
    memcpy(&buffer[0], &size, sizeof(int));
    buffer += data;

    return buffer;
}

bool Book::unpack(const string& buffer) {
    if (buffer.size() < sizeof(int)) return false;

    int size;
    memcpy(&size, buffer.data(), sizeof(int));

    string data = buffer.substr(sizeof(int));
    stringstream ss(data);
    string idStr, yearStr;

    getline(ss, idStr, '|');
    getline(ss, title, '|');
    getline(ss, authors, '|');
    getline(ss, yearStr, '|');
    getline(ss, categories);

    try {
        id = stoi(idStr);
        year = stoi(yearStr);
    } catch (const exception& e) {
        return false;
    }

    return true;
}
