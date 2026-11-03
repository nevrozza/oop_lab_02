#pragma once

#include <cstddef>
#include <iosfwd>

class String {
public:
    static constexpr std::size_t npos = static_cast<std::size_t>(-1);

    String();
    String(const char *text);
    String(const String &other);
    String(String &&other) noexcept;
    String &operator=(String other) noexcept;
    ~String();

    std::size_t size() const;
    std::size_t capacity() const;
    bool empty() const;
    const char *c_str() const;

    void push_back(char ch);
    void append(const char *text);
    void append(const String &other);
    std::size_t find(const char *text) const;
    void print(std::ostream &out) const;

    void word_histogram(String *words, std::size_t *counts, std::size_t &out_size) const;

private:
    void swap(String &other) noexcept;

    char *data_;
    std::size_t size_;
    std::size_t capacity_;
};
