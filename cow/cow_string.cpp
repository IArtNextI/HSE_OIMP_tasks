#include "cow_string.h"

#include <tuple>

CowString::MyString::MyString(const char *data) {
    size_ = strlen(data);
    data_ = new char[size_];
    links_ = new size_t(1);
    for (size_t i = 0; i < size_; ++i) {
        data_[i] = data[i];
    }
}

CowString::MyString::MyString(const CowString::MyString &other) {
    size_ = other.size_;
    data_ = other.data_;
    links_ = other.links_;
    ++(*links_);
}

CowString::MyString::~MyString() {
    --(*links_);
    if (*links_ == 0) {
        delete[] data_;
        delete links_;
    }
}

CowString::MyString::MyString(CowString::MyString &&other) {
    links_ = other.links_;
    data_ = other.data_;
    size_ = other.size_;
    other.links_ = nullptr;
    other.data_ = nullptr;
    other.size_ = 0;
}

CowString::MyString &CowString::MyString::operator=(const CowString::MyString &other) {
    if (*links_ == 1) {
        delete[] data_;
        delete links_;
    }
    data_ = other.data_;
    links_ = other.links_;
    size_ = other.size_;
    ++(*links_);
    return *this;
}

CowString::MyString &CowString::MyString::operator=(CowString::MyString &&other) {
    if (*links_ == 1) {
        delete[] data_;
        delete links_;
    }
    data_ = other.data_;
    links_ = other.links_;
    size_ = other.size_;
    other.links_ = nullptr;
    other.data_ = nullptr;
    other.size_ = 0;
    return *this;
}

CowString::CowString(std::string_view data) : data_(std::string(data).c_str()) {
}

CowString::CowString(const CowString &data) : data_(data.data_) {
}

CowString::CowString(CowString &&data) : data_(std::move(data.data_)) {
}

CowString &CowString::operator=(const CowString &other) {
    data_ = other.data_;
    return *this;
}

CowString &CowString::operator=(CowString &&other) {
    data_ = std::move(other.data_);
    return *this;
}

const char *CowString::GetData() const {
    return data_.data_;
}

const char &CowString::operator[](size_t index) const {
    return data_.data_[index];
}

CowString::MyChar CowString::operator[](size_t index) {
    return CowString::MyChar(data_.data_[index], *this, index);
}

const char CowString::At(size_t index) const {
    return data_.data_[index];
}

CowString CowString::operator+=(std::string_view other) {
    if (*data_.links_ == 1) {
        char *new_data = new char[data_.size_ + other.size()];
        std::copy(data_.data_, data_.data_ + data_.size_, new_data);
        for (size_t i = 0; i < other.size(); ++i) {
            new_data[data_.size_ + i] = other[i];
        }
        delete[] data_.data_;
        data_.data_ = new_data;
        data_.size_ = data_.size_ + other.size();
    } else {
        char *new_data = new char[data_.size_ + static_cast<size_t>(other.size())];
        std::copy(data_.data_, data_.data_ + data_.size_, new_data);
        for (size_t i = 0; i < other.size(); ++i) {
            new_data[data_.size_ + i] = other[i];
        }
        --(*data_.links_);
        data_.links_ = new size_t(1);
        data_.data_ = new_data;
        data_.size_ = data_.size_ + other.size();
    }
    return *this;
}

CowString::CowStringIterator CowString::begin() {
    return CowString::CowStringIterator(*this, 0);
}

CowString::CowStringConstIterator CowString::begin() const {
    return CowString::CowStringConstIterator(*this, 0);
}

CowString::CowStringIterator CowString::end() {
    return CowString::CowStringIterator(*this, data_.size_);
}

CowString::CowStringConstIterator CowString::end() const {
    return CowString::CowStringConstIterator(*this, data_.size_);
}

bool CowString::operator==(std::string_view other) const {
    if (other.size() != data_.size_) {
        return false;
    }
    for (size_t i = 0; i < data_.size_; ++i) {
        if (data_.data_[i] != other[i]) {
            return false;
        }
    }
    return true;
}

bool CowString::operator!=(std::string_view other) const {
    if (other.size() != data_.size_) {
        return true;
    }
    for (size_t i = 0; i < data_.size_; ++i) {
        if (data_.data_[i] != other[i]) {
            return true;
        }
    }
    return false;
}

bool operator==(std::string_view self, const CowString &other) {
    if (self.size() != other.data_.size_) {
        return false;
    }
    for (size_t i = 0; i < other.data_.size_; ++i) {
        if (other.data_.data_[i] != self[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(std::string_view self, const CowString &other) {
    if (self.size() != other.data_.size_) {
        return true;
    }
    for (size_t i = 0; i < other.data_.size_; ++i) {
        if (other.data_.data_[i] != self[i]) {
            return true;
        }
    }
    return false;
}

CowString::MyChar::MyChar(char &c, CowString &str, size_t index) : c(c), str(str), index(index) {
}

char CowString::MyChar::operator=(char x) {
    if (c == x) {
        return c;
    }
    if (*str.data_.links_ == 1) {
        return c = x;
    } else {
        char *new_data = new char[str.data_.size_];
        std::copy(str.data_.data_, str.data_.data_ + str.data_.size_, new_data);
        new_data[index] = x;
        --(*str.data_.links_);
        str.data_.links_ = new size_t(1);
        str.data_.data_ = new_data;
        return x;
    }
}

bool CowString::MyChar::operator==(char x) {
    return c == x;
}

bool operator==(char x, CowString::MyChar other) {
    return x == other.c;
}

CowString::CowStringIterator::CowStringIterator(CowString &string_ref, size_t index)
    : string_(string_ref), index_(index) {
}

CowString::CowStringIterator &CowString::CowStringIterator::operator++() {
    ++index_;
    return *this;
}

bool CowString::CowStringIterator::operator!=(const CowString::CowStringIterator &other) const {
    return &string_ != &other.string_ || index_ != other.index_;
}

CowString::MyChar CowString::CowStringIterator::operator*() {
    return CowString::MyChar(string_.data_.data_[index_], string_, index_);
}

char CowString::CowStringIterator::operator*() const {
    return string_.data_.data_[index_];
}

CowString::CowStringConstIterator::CowStringConstIterator(const CowString &string_ref, size_t index)
    : string_(string_ref), index_(index) {
}

char CowString::CowStringConstIterator::operator*() const {
    return string_.data_.data_[index_];
}

CowString::CowStringConstIterator &CowString::CowStringConstIterator::operator++() {
    ++index_;
    return *this;
}

bool CowString::CowStringConstIterator::operator!=(const CowString::CowStringConstIterator &other) const {
    return &string_ != &other.string_ || index_ != other.index_;
}
