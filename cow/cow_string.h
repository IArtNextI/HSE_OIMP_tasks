#pragma once

#include <cstring>
#include <string>

class CowString {
private:
    class MyString {
    public:
        char* data_;
        size_t size_;
        size_t* links_;

        MyString(const char* data);
        MyString(const MyString& other);
        MyString(MyString&& other);

        ~MyString();

        MyString& operator=(const MyString& other);
        MyString& operator=(MyString&& other);
    };

    class MyChar {
    public:
        char& c;
        CowString& str;
        size_t index;

        MyChar(char& c, CowString& str, size_t index);

        char operator=(char x);
        bool operator==(char x);
    };

    MyString data_;

public:
    class CowStringIterator {
    private:
        CowString& string_;
        size_t index_;

    public:
        char operator*() const;
        MyChar operator*();
        CowStringIterator(CowString& string_ref, size_t index);
        CowStringIterator& operator++();
        bool operator!=(const CowStringIterator& other) const;
    };

    class CowStringConstIterator {
    private:
        const CowString& string_;
        size_t index_;

    public:
        char operator*() const;
        CowStringConstIterator(const CowString& string_ref, size_t index);
        CowStringConstIterator& operator++();
        bool operator!=(const CowStringConstIterator& other) const;
    };

    CowString(std::string_view data);
    CowString(const CowString& data);
    CowString(CowString&& data);

    CowString& operator=(const CowString& other);
    CowString& operator=(CowString&& other);

    const char* GetData() const;

    const char& operator[](size_t index) const;
    CowString::MyChar operator[](size_t index);

    const char At(size_t index) const;

    CowString operator+=(std::string_view other);

    CowStringIterator begin();
    CowStringConstIterator begin() const;
    CowStringIterator end();
    CowStringConstIterator end() const;

    bool operator==(std::string_view other) const;
    bool operator!=(std::string_view other) const;
    friend bool operator==(std::string_view self, const CowString& other);
    friend bool operator!=(std::string_view self, const CowString& other);

    friend bool operator==(char x, MyChar other);
};
