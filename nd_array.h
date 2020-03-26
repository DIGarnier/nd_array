#pragma once

#include <array>


template <typename T, int... N>
struct nd_array : public std::array<T, (N * ...)>  {

    std::array<T, sizeof...(N) + 1> dims{1, N...};
    decltype(std::make_index_sequence<sizeof...(N)>{}) seq = std::make_index_sequence<sizeof...(N)>{};

    template <typename... Idxs>
    constexpr const T& at(Idxs... indices) const {
        range_check(indices...);
        return nt_element(indices...);
    }

    template <typename... Idxs>
    constexpr T& at(Idxs... indices) {
        range_check(indices...);
        return nt_element(indices...);
    }

private:

    
    using super = std::array<T, (N * ...)>;

    template <typename... Idxs>
    constexpr const T& nt_element(Idxs... indices) const {
        return super::operator[](calculate_index(seq, indices...));
    }

    template <typename... Idxs>
    constexpr T& nt_element(Idxs... indices) {
        return super::operator[](calculate_index(seq, indices...));
    }

    template <typename... Idxs, std::size_t... Is>
    constexpr void range_check(Idxs... idx) const {
        if (not_in_range(seq, idx...))
            throw std::out_of_range("Indices out of range");
    }

    template <typename... Idxs, std::size_t... Is>
    constexpr std::size_t calculate_index(std::index_sequence<Is...>,
                                     Idxs... idx) const {
        return ((dims[std::max((int)(Is - 1), 0)] * dims[Is] * idx) + ...);
    }

    template <typename... Idxs, std::size_t... Is>
    constexpr bool not_in_range(std::index_sequence<Is...>,
                                Idxs... idx) const {
        return ((idx >= dims[Is + 1]) || ...);
    }

};
