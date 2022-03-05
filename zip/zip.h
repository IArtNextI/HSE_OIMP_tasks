#pragma once

#include <forward_list>
#include <string>

using Value = std::string;
using Iterator = std::forward_list<std::string>::const_iterator;
using ZippedPair = std::pair<const Value&, const Value&>;

class Zipped {
private:
    Iterator first_begin_;
    Iterator second_begin_;
    Iterator first_end_;
    Iterator second_end_;

public:
    class ZippedIterator {
    private:
        Iterator first_;
        Iterator second_;

    public:
        ZippedIterator(Iterator first, Iterator second);
        ZippedPair operator*();
        ZippedIterator& operator++();
        bool operator!=(const ZippedIterator& other) const;
    };

    explicit Zipped(Iterator first_begin, Iterator second_begin, Iterator first_end, Iterator second_end);

    ZippedIterator begin();
    ZippedIterator end();
};

Zipped Zip(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end);
