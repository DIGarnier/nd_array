#pragma once

#include <array>


template <typename T, int... N>
struct nd_array {

std::array<T, (N * ...)> array = {};
    std::array<T, sizeof...(N) + 1> dims{1, N...};
    decltype(std::make_index_sequence<sizeof...(N)>{}) seq = std::make_index_sequence<sizeof...(N)>{};

    template <typename... Idxs>
    constexpr T const& at(Idxs... indice) const {
        range_check(indice...);
        return array[calculate_index(seq, indice...)];
    }

    template <typename... Idxs>
    T& at(Idxs... indice) {
        range_check(indice...);
        return array[calculate_index(seq, indice...)];
    }

private:
    template <typename... Idxs, std::size_t... Is>
    constexpr std::size_t calculate_index(std::index_sequence<Is...>,
                                     Idxs... idx) const {
        return ((dims[std::max((T)(Is - 1), 0)] * dims[Is] * idx) + ...);
    }

    template <typename... Idxs, std::size_t... Is>
    constexpr void range_check(Idxs... idx) const {
        if (not_in_range(seq, idx...))
            throw std::out_of_range("Indices out of range");
    }

    template <typename... Idxs, std::size_t... Is>
    constexpr bool not_in_range(std::index_sequence<Is...>,
                                Idxs... idx) const {
        return ((idx >= dims[Is + 1]) || ...);
    }

};
