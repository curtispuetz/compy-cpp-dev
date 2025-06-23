#pragma once

#include "exceptions/stdexcept.h"
#include "py_list.h"
#include <vector>

// Helper to check if a type is a PyList
template <typename> struct is_pylist : std::false_type {};
template <typename U> struct is_pylist<PyList<U>> : std::true_type {};

// Recursive helper to deduce the shape of a nested PyList.
// It also validates that the list is not ragged.
template <typename ListType>
static void deduce_shape_recursive(const ListType &list, PyList<int> &shape) {
    // Use if-constexpr to distinguish between a list of lists and a list of
    // elements at compile time.
    if constexpr (is_pylist<typename ListType::value_type>::value) {
        shape.append(list.len());
        if (list.len() > 0) {
            const auto &first_sublist = list[0];
            for (int i = 1; i < list.len(); ++i) {
                if (list[i].len() != first_sublist.len()) {
                    throw PyppValueError(
                        "Inconsistent list lengths found when initializing "
                        "numpy array; array is ragged.");
                }
            }
            deduce_shape_recursive(first_sublist, shape);
        }
    } else {
        shape.append(list.len());
    }
}

// Recursive helper to flatten the nested PyList into a single std::vector.
template <typename T, typename ListType>
static void flatten_recursive(const ListType &list, std::vector<T> &data) {
    if constexpr (is_pylist<typename ListType::value_type>::value) {
        // This list contains other lists, so recurse into each one.
        for (const auto &sublist : list) {
            flatten_recursive(sublist, data);
        }
    } else {
        // This list contains the actual data elements. Copy them to the
        // flat vector.
        for (const T &item : list) {
            data.push_back(item);
        }
    }
}