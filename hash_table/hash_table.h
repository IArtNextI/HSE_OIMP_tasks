#pragma once

#include <vector>
#include <string>
#include <unordered_map>

template <class K, class V, class Hash = std::hash<K>, class KeyEqual = std::equal_to<K>>
class HashTable {
private:
    static constexpr int primes[22]{11,      23,      59,       109,      271,      547,      1063,   2683,
                                    5441,    10429,   26017,    51349,    109441,   276181,   560969, 1000099,
                                    2834939, 5132629, 10646771, 26949317, 56081101, 114328583};
    std::vector<std::pair<K, V>> data_;
    std::vector<bool> free_;
    size_t ptr_;
    size_t size_;

    long double LoadFactor() {
        return static_cast<long double>(size_) / data_.size();
    }

    std::pair<size_t, bool> FindFirstEmpty(K key, const std::vector<std::pair<K, V>>& data,
                                           const std::vector<bool>& is_free) const {
        size_t start_pos = Hash()(key) % data.size();
        size_t ptr = start_pos;
        do {
            if (is_free[ptr]) {
                return std::make_pair(ptr, true);
            }
            ++ptr;
            if (ptr == data.size()) {
                ptr = 0;
            }
        } while (ptr != start_pos);
        return std::make_pair(data.size(), false);
    }

    std::pair<size_t, bool> FindFirstEqual(K key, const std::vector<std::pair<K, V>>& data,
                                           const std::vector<bool>& is_free) const {
        size_t start_pos = Hash()(key) % data.size();
        size_t ptr = start_pos;
        do {
            if (!is_free[ptr] && KeyEqual()(data[ptr].first, key)) {
                return std::make_pair(ptr, true);
            }
            ++ptr;
            if (ptr == data.size()) {
                ptr = 0;
            }
        } while (ptr != start_pos);
        return std::make_pair(data.size(), false);
    }

    std::pair<size_t, bool> FindFirstEqualOrEmpty(K key, const std::vector<std::pair<K, V>>& data,
                                                  const std::vector<bool>& is_free) const {
        size_t start_pos = Hash()(key) % data.size();
        size_t ptr = start_pos;
        do {
            if (is_free[ptr]) {
                return std::make_pair(ptr, true);
            }
            if (KeyEqual()(data[ptr].first, key)) {
                return std::make_pair(ptr, true);
            }
            ++ptr;
            if (ptr == data.size()) {
                ptr = 0;
            }
        } while (ptr != start_pos);
        return std::make_pair(data.size(), false);
    }

    std::pair<size_t, bool> FindNextEmpty(size_t start, const std::vector<bool>& free) const {
        size_t ptr = start;
        do {
            if (free[ptr]) {
                return std::make_pair(ptr, true);
            }
            ++ptr;
            if (ptr == free.size()) {
                ptr = 0;
            }
        } while (start != ptr);
        return std::make_pair(free.size(), false);
    }

    std::pair<size_t, bool> FindNextNotEmpty(size_t start, const std::vector<bool>& free) const {
        size_t ptr = start;
        do {
            if (!free[ptr]) {
                return std::make_pair(ptr, true);
            }
            ++ptr;
            if (ptr == free.size()) {
                ptr = 0;
            }
        } while (start != ptr);
        return std::make_pair(free.size(), false);
    }

    void ReHash() {
        std::vector<std::pair<K, V>> new_data_;
        std::vector<bool> new_free_;
        size_t new_size;
        if (ptr_ < 22) {
            new_size = primes[++ptr_];
        } else {
            new_size = data_.size() * 2;
        }
        new_data_.resize(new_size);
        new_free_.resize(new_size, true);
        for (size_t i = 0; i < data_.size(); ++i) {
            if (free_[i]) {
                continue;
            }
            auto& [key, value] = data_[i];
            auto index = FindFirstEmpty(key, new_data_, new_free_);
            new_data_[index.first] = data_[i];
            new_free_[index.first] = false;
        }
        data_ = new_data_;
        free_ = new_free_;
    }

    void CheckLoadFactor() {
        if (LoadFactor() >= 0.75l) {
            ReHash();
        }
    }

public:
    HashTable() {
        ptr_ = 0;
        size_ = 0;
        data_.resize(primes[0]);
        free_.assign(primes[0], true);
    }

    HashTable(HashTable& other) = default;
    HashTable(HashTable&& other) = default;

    template <class Iterator>
    HashTable(Iterator a, Iterator b) {
        for (auto it = a; it != b; ++it) {
            insert(*it);
        }
    }

    HashTable& operator=(HashTable& other) {
        data_ = other.data_;
        free_ = other.free_;
        size_ = other.size_;
        ptr_ = other.ptr_;
    }

    HashTable& operator=(HashTable&& other) {
        data_.swap(other.data_);
        free_.swap(other.free_);
        size_ = other.size_;
        ptr_ = other.ptr_;
    }

    class HashTableIterator {
    private:
        HashTable& table_;
        size_t index_;

    public:
        const std::pair<K, V>& operator*() const {
            return table_.data_[index_];
        }

        std::pair<K, V>& operator*() {
            return table_.data_[index_];
        }

        HashTableIterator(HashTable& table, size_t index) : table_(table), index_(index) {
        }

        HashTableIterator& operator++() {
            ++index_;
            while (index_ < table_.data_.size() && table_.free_[index_]) {
                ++index_;
            }
            return *this;
        }

        std::pair<K, V>* operator->() const {
            return &table_.data_[index_];
        }

        bool operator!=(const HashTableIterator& other) const {
            return &table_ != &other.table_ || index_ != other.index_;
        }
    };

    class HashTableConstIterator {
    private:
        const HashTable& table_;
        size_t index_;

    public:
        const std::pair<K, V>& operator*() const {
            return table_.data_[index_];
        }

        HashTableConstIterator(const HashTable& table, size_t index) : table_(table), index_(index) {
        }

        HashTableConstIterator& operator++() {
            ++index_;
            while (index_ < table_.data_.size() && table_.free_[index_]) {
                ++index_;
            }
            return *this;
        }

        bool operator!=(const HashTableConstIterator& other) const {
            return &table_ != &other.table_ || index_ != other.index_;
        }
    };

    V& operator[](K key) {
        CheckLoadFactor();
        auto index = FindFirstEqualOrEmpty(key, data_, free_);
        if (free_[index.first]) {
            ++size_;
        }
        free_[index.first] = false;
        data_[index.first].first = key;
        return data_[index.first].second;
    }

    const V& at(K key) const {
        auto index = FindFirstEqual(key, data_, free_);
        return data_[index.first].second;
    }

    auto find(K key) {
        auto index = FindFirstEqual(key, data_, free_);
        if (index.second) {
            return HashTableIterator(*this, index.first);
        } else {
            return HashTableIterator(*this, data_.size());
        }
    }

    size_t size() const {
        return size_;
    }

    void clear() {
        free_.assign(free_.size(), true);
        size_ = 0;
    }

    bool empty() const {
        return size_ == 0;
    }

    HashTableIterator begin() {
        auto it = FindNextNotEmpty(0, free_);
        return HashTableIterator(*this, it.first);
    }

    HashTableConstIterator begin() const {
        auto it = FindNextNotEmpty(0, free_);
        return HashTableConstIterator(*this, it.first);
    }

    HashTableIterator end() {
        return HashTableIterator(*this, free_.size());
    }

    HashTableConstIterator end() const {
        return HashTableConstIterator(*this, free_.size());
    }

    std::pair<HashTableIterator, bool> emplace(K key, V value) {
        auto it = FindFirstEqualOrEmpty(key, data_, free_);
        if (!free_[it.first]) {
            return std::make_pair(end(), false);
        } else {
            data_[it.first] = std::make_pair(key, value);
            free_[it.first] = false;
            return std::make_pair(HashTableIterator(*this, it.first), true);
        }
    }

    std::pair<HashTableIterator, bool> insert(std::pair<K, V> item) {
        return emplace(item.first, item.second);
    }
};
