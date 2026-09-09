#pragma once
#include <cstddef>
#include <utility>
#include <initializer_list>

template <typename T>
class SimpleVector {
private:
    T* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;

    void reallocate(size_t new_cap) {
        T* new_data = new T[new_cap];
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move(data_[i]);
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_cap;
    }

public:
    SimpleVector() = default;

    explicit SimpleVector(size_t initial_cap) {
        if (initial_cap > 0) reallocate(initial_cap);
    }

    SimpleVector(std::initializer_list<T> list) {
        reallocate(list.size());
        for (const auto& item : list) {
            push_back(item);
        }
    }

    ~SimpleVector() {
        delete[] data_;
    }

    SimpleVector(const SimpleVector& other) : size_(other.size_), capacity_(other.capacity_) {
        if (capacity_ > 0) {
            data_ = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
    }

    SimpleVector& operator=(const SimpleVector& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = capacity_ > 0 ? new T[capacity_] : nullptr;
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
        return *this;
    }

    SimpleVector(SimpleVector&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    SimpleVector& operator=(SimpleVector&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    void push_back(const T& val) {
        if (size_ == capacity_) reallocate(capacity_ == 0 ? 4 : capacity_ * 2);
        data_[size_++] = val;
    }

    void push_back(T&& val) {
        if (size_ == capacity_) reallocate(capacity_ == 0 ? 4 : capacity_ * 2);
        data_[size_++] = std::move(val);
    }

    void pop_back() {
        if (size_ > 0) --size_;
    }

    void clear() { size_ = 0; }
    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    T& operator[](size_t index) { return data_[index]; }
    const T& operator[](size_t index) const { return data_[index]; }

    T* begin() { return data_; }
    const T* begin() const { return data_; }
    T* end() { return data_ + size_; }
    const T* end() const { return data_ + size_; }
};