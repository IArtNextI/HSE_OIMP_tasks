#pragma once

#include <exception>
#include <iostream>
#include <string>
#include <sstream>
#include <tuple>

class CsvParserError : public std::exception {
};

template<class... TArgs>
class Tuple;

template<>
class Tuple<> {
public:
    Tuple<>() {
    }
    friend std::ostream& operator<<(std::ostream& out, const Tuple<>& a) {
        return out;
    }
};

template<class T>
class Tuple<T> {
private:
    T value_;
public:
    Tuple<T>(T value) : value_(value) {
    }

    Tuple<T>(T value, Tuple<> tail) : value_(value) {
    }

    friend std::ostream& operator<<(std::ostream& out, const Tuple<T>& a) {
        out << a.value_;
        return out;
    }
};

template<class THead, class... TTail>
class Tuple<THead, TTail...> : public Tuple<TTail...> {
private:
    THead value_;

public:
    Tuple<THead, TTail...>(THead value, TTail... values) : Tuple<TTail...>(values...), value_(value) {
    }

    Tuple<THead, TTail...>(THead head, Tuple<TTail...> tail) : Tuple<TTail...>(tail), value_(head){
    }

    friend std::ostream& operator<<(std::ostream& out, const Tuple<THead, TTail...>& a) {
        out << a.value_ << ", ";
        out << static_cast<const Tuple<TTail...>&>(a);
        return out;
    }
};

template<class... Args>
class TupleParser;

template<>
class TupleParser<> {
public:
    static Tuple<> go(std::string& str, size_t index_, char sep_) {
        return Tuple<>();
    }
};

template<class... B>
class TupleParser<std::string, B...> {
public:
    static Tuple<std::string, B...> go(std::string& str, size_t index_, char sep_) {
        std::string current;
        bool in1 = false;
        bool in2 = false;
        size_t i = index_;
        for (; i < str.size() && str[i] != '\n'; ++i) {
            if (str[i] == '\"' && str[i + 1] == '\"'){
                in2 ^= true;
                current += '\"';
                ++i;
            } else if (str[i] == '\"' && str[i + 1] != '\"'){
                in1 ^= true;
            } else {
                if (in1 || in2 || str[i] != sep_) {
                    current += str[i];
                } else {
                    break;
                }
            }
        }
        index_ = i + 1;
        return Tuple<std::string, B...>(current, TupleParser<B...>::go(str, index_, sep_));
    }
};


template<class A, class... B>
class TupleParser<A, B...> {
public:
    static Tuple<A, B...> go(std::string& str, size_t index_, char sep_) {
        std::string current;
        bool in1 = false;
        bool in2 = false;
        size_t i = index_;
        for (; i < str.size() && str[i] != '\n'; ++i) {
            if (str[i] == '\"' && str[i + 1] == '\"'){
                in2 ^= true;
                current += '\"';
                ++i;
            } else if (str[i] == '\"' && str[i + 1] != '\"'){
                in1 ^= true;
            } else {
                if (in1 || in2 || str[i] != sep_) {
                    current += str[i];
                } else {
                    break;
                }
            }
        }
        index_ = i + 1;
        std::stringstream s;
        A res{};
        if (!current.empty()) {
            s << current;
            s >> res;
            if (s.fail()) {
                throw CsvParserError();
            }
        }
        return Tuple<A, B...>(res, TupleParser<B...>::go(str, index_, sep_));
    }
};

template <class... Args>
class CsvParser {
private:
    std::istream &in_;
    char sep_;
    size_t pos_;
    size_t end_;

public:
    CsvParser(std::istream &in, char sep, size_t ignore = 0) : in_(in), sep_(sep) {
        in_.seekg(0, std::ios::end);
        end_ = in_.tellg();
        in_.seekg(0);
        size_t cnt = 0;
        pos_ = 0;
        while (cnt < ignore) {
            char cur;
            in_.read(&cur, 1);
            if (cur == '\n') {
                ++cnt;
            }
            ++pos_;
        }
    }

    class CsvParserIterator {
    private:
        CsvParser<Args...>& parser_;
        size_t index_;

    public:
        CsvParserIterator(CsvParser<Args...>& parser, size_t index) : parser_(parser), index_(index) {
        }

        Tuple<Args...> operator*() {
            size_t index_cur_ = index_;
            parser_.in_.seekg(index_);
            std::string current;
            char c;
            while (index_cur_ < parser_.end_) {
                parser_.in_.read(&c, 1);
                current += c;
                ++index_cur_;
                if (c == '\n') {
                    break;
                }
            }
            if (index_cur_ == parser_.end_) {
                current.push_back('\n');
            }
            return TupleParser<Args...>::go(current, 0, parser_.sep_);
        }

        CsvParserIterator& operator++() {
            parser_.in_.seekg(index_);
            while (index_ < parser_.end_) {
                char c;
                parser_.in_.read(&c, 1);
                ++index_;
                if (c == '\n') {
                    break;
                }
            }
            return *this;
        }

        bool operator!=(const CsvParserIterator& other) const {
            return &parser_ != &other.parser_ || index_ != other.index_;
        }
    };

    CsvParserIterator begin() {
        return CsvParserIterator(*this, this->pos_);
    }

    CsvParserIterator end() {
        return CsvParserIterator(*this, this->end_);
    }
};
