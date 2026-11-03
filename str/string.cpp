#include "string_class.h"

#include <cstring>
#include <ostream>

String::String() : String("") {}

String::String(const char *text) : data_(nullptr), size_(0), capacity_(1) {
    if (!text) {
        text = "";
    }
    size_ = std::strlen(text);
    if (size_ > capacity_) {
        capacity_ = size_;
    }
    data_ = new char[capacity_ + 1];
    std::memcpy(data_, text, size_ + 1);
}

String::String(const String &other) : String(other.c_str()) {}

String::String(String &&other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

String &String::operator=(String other) noexcept {
    swap(other);
    return *this;
}

String::~String() {
    delete[] data_;
}

void String::swap(String &other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

std::size_t String::size() const {
    return size_;
}

std::size_t String::capacity() const {
    return capacity_;
}

bool String::empty() const {
    return size_ == 0;
}

const char *String::c_str() const {
    return data_ ? data_ : "";
}

void String::push_back(char ch) {
    char text[] = {ch, '\0'};
    append(text);
}

void String::append(const char *text) {
    if (!text || *text == '\0') {
        return;
    }

    std::size_t len = std::strlen(text);
    const std::size_t max_capacity = npos - 1;
    if (len > max_capacity - size_) {
        throw std::length_error("String is too long");
    }

    std::size_t new_size = size_ + len;
    if (new_size > capacity_) {
        std::size_t new_capacity = capacity_ ? capacity_ : 1;
        while (new_capacity < new_size) {
            if (new_capacity > max_capacity / 2) {
                new_capacity = new_size;
                break;
            }
            new_capacity *= 2;
        }

        char *bigger = new char[new_capacity + 1];
        std::memcpy(bigger, c_str(), size_);
        std::memcpy(bigger + size_, text, len + 1);
        delete[] data_;
        data_ = bigger;
        capacity_ = new_capacity;
    } else {
        std::memmove(data_ + size_, text, len + 1);
    }
    size_ = new_size;
}

void String::append(const String &other) {
    append(other.c_str());
}

std::size_t String::find(const char *text) const {
    if (!text) {
        return npos;
    }
    const char *start = c_str();
    const char *found = std::strstr(start, text);
    return found ? static_cast<std::size_t>(found - start) : npos;
}

void String::print(std::ostream &out) const {
    out << c_str();
}

static bool is_separator(char ch) {
    unsigned char byte = static_cast<unsigned char>(ch);
    return std::isspace(byte) || std::ispunct(byte);
}

void String::word_histogram(String *words, std::size_t *counts, std::size_t &out_size) const {
    out_size = 0;
    for (std::size_t i = 0; i < size_;) {
        if (is_separator(data_[i])) {
            ++i;
            continue;
        }

        String word;
        while (i < size_ && !is_separator(data_[i])) {
            word.push_back(data_[i++]);
        }

        std::size_t pos = 0;
        while (pos < out_size && std::strcmp(words[pos].c_str(), word.c_str()) != 0) {
            ++pos;
        }
        if (pos == out_size) {
            words[pos] = std::move(word);
            counts[pos] = 0;
            ++out_size;
        }
        ++counts[pos];
    }
}
