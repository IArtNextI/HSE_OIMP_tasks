#pragma once

#include <iterator>

template<class Iterator1, class Iterator2>
class Zipped {
private:
    Iterator1 first_begin_;
    Iterator2 second_begin_;
    Iterator1 first_end_;
    Iterator2 second_end_;

public:
    class ZippedIterator {
    private:
        Iterator1 first_;
        Iterator2 second_;

    public:
        ZippedIterator(Iterator1 first, Iterator2 second) : first_(first), second_(second) {};
        std::pair<decltype(*first_), decltype(*second_)> operator*() {
            return std::pair<decltype(*first_), decltype(*second_)>(*first_, *second_);
        }
        ZippedIterator& operator++() {
            ++first_;
            ++second_;
            return *this;
        }
        bool operator!=(const ZippedIterator& other) const {
            return first_ != other.first_ && second_ != other.second_;
        }
    };

    explicit Zipped(Iterator1 first_begin, Iterator2 second_begin, Iterator1 first_end, Iterator2 second_end)
        : first_begin_(first_begin), second_begin_(second_begin), first_end_(first_end), second_end_(second_end) {};

    ZippedIterator begin() {
        return ZippedIterator(first_begin_, second_begin_);
    }

    ZippedIterator end() {
        return ZippedIterator(first_end_, second_end_);
    }
};

template <typename Sequence1, typename Sequence2>
auto Zip(const Sequence1& sequence1, const Sequence2& sequence2) {
    auto a_begin = std::begin(sequence1);
    auto b_begin = std::begin(sequence2);
    auto a_end = std::end(sequence1);
    auto b_end = std::end(sequence2);
    return Zipped(a_begin, b_begin, a_end, b_end);
}
