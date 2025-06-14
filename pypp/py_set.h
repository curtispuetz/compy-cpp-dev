#include <initializer_list>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unordered_set>

template <typename T> class PySet {
  private:
    std::unordered_set<T> data;

  public:
    // Constructors
    PySet() = default;
    PySet(std::initializer_list<T> init) : data(init) {}

    // Add an element
    void add(const T &value) { data.insert(value); }

    // Remove an element (raises if not found)
    void remove(const T &value) {
        auto it = data.find(value);
        if (it == data.end()) {
            throw std::runtime_error("KeyError: element not found in set");
        }
        data.erase(it);
    }

    // Discard an element (no error if not found)
    void discard(const T &value) { data.erase(value); }

    // Check membership
    bool contains(const T &value) const {
        return data.find(value) != data.end();
    }

    // Set operations
    PySet<T> union_(const PySet<T> &other) const {
        PySet<T> result = *this;
        result.data.insert(other.data.begin(), other.data.end());
        return result;
    }

    PySet<T> intersection(const PySet<T> &other) const {
        PySet<T> result;
        for (const auto &item : data) {
            if (other.contains(item)) {
                result.add(item);
            }
        }
        return result;
    }

    PySet<T> difference(const PySet<T> &other) const {
        PySet<T> result;
        for (const auto &item : data) {
            if (!other.contains(item)) {
                result.add(item);
            }
        }
        return result;
    }

    PySet<T> symmetric_difference(const PySet<T> &other) const {
        PySet<T> result;
        for (const auto &item : data) {
            if (!other.contains(item)) {
                result.add(item);
            }
        }
        for (const auto &item : other.data) {
            if (!contains(item)) {
                result.add(item);
            }
        }
        return result;
    }

    // Update: add elements from another container
    template <typename Iterable> void update(const Iterable &other) {
        for (const auto &elem : other) {
            data.insert(elem);
        }
    }

    // Difference update: remove elements found in another container
    template <typename Iterable> void difference_update(const Iterable &other) {
        for (const auto &elem : other) {
            data.erase(elem);
        }
    }

    // Intersection update: keep only elements also in another container
    template <typename Iterable>
    void intersection_update(const Iterable &other) {
        std::unordered_set<T> other_set(other.begin(), other.end());
        for (auto it = data.begin(); it != data.end();) {
            if (other_set.find(*it) == other_set.end()) {
                it = data.erase(it);
            } else {
                ++it;
            }
        }
    }

    // Symmetric difference update: elements in either set but not both
    template <typename Iterable>
    void symmetric_difference_update(const Iterable &other) {
        std::unordered_set<T> other_set(other.begin(), other.end());
        for (const auto &elem : other_set) {
            if (data.find(elem) != data.end()) {
                data.erase(elem);
            } else {
                data.insert(elem);
            }
        }
    }

    // Comparison operators (issubset, issuperset, equality)
    bool issubset(const PySet<T> &other) const {
        for (const auto &item : data) {
            if (!other.contains(item)) {
                return false;
            }
        }
        return true;
    }

    bool issuperset(const PySet<T> &other) const {
        return other.issubset(*this);
    }

    bool operator==(const PySet<T> &other) const { return data == other.data; }

    bool operator!=(const PySet<T> &other) const { return data != other.data; }

    // Size
    int len() const { return data.size(); }

    // Clear
    void clear() { data.clear(); }

    // Iterator support
    auto begin() { return data.begin(); }
    auto end() { return data.end(); }
    auto begin() const { return data.begin(); }
    auto end() const { return data.end(); }

    // For debugging
    void print(std::ostream &os) const {
        if (data.size() == 0) {
            os << "set()";
            return;
        }
        os << "{";
        int i = 0;
        for (const auto &item : data) {
            os << item;
            if (i != data.size() - 1)
                os << ", ";
            i++;
        }
        os << "}";
    }

    void print() const {
        print(std::cout);
        std::cout << std::endl;
    }

    template <typename U>
    friend std::ostream &operator<<(std::ostream &os, const PySet<U> &other);
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const PySet<T> &other) {
    other.print(os);
    return os;
}