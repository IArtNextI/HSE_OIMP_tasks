#include "zip.h"

Zipped::ZippedIterator Zipped::begin() {
    return Zipped::ZippedIterator(first_begin_, second_begin_);
}

Zipped::ZippedIterator Zipped::end() {
    return Zipped::ZippedIterator(first_end_, second_end_);
}

Zipped::Zipped(Iterator first_begin, Iterator second_begin, Iterator first_end, Iterator second_end) {
    first_begin_ = first_begin;
    first_end_ = first_end;
    second_begin_ = second_begin;
    second_end_ = second_end;
}

Zipped::ZippedIterator::ZippedIterator(Iterator first, Iterator second) : first_(first), second_(second) {
}

ZippedPair Zipped::ZippedIterator::operator*() {
    return ZippedPair(*first_, *second_);
}

Zipped::ZippedIterator& Zipped::ZippedIterator::operator++() {
    ++first_;
    ++second_;
    return *this;
}

bool Zipped::ZippedIterator::operator!=(const Zipped::ZippedIterator& other) const {
    return first_ != other.first_ && second_ != other.second_;
}

Zipped Zip(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end) {
    Zipped zipped(a_begin, b_begin, a_end, b_end);
    return zipped;
}
