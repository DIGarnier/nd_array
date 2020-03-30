#pragma once

#include <array>

template<uint32_t... N>
constexpr auto make_coeffs();

template <typename T, uint32_t... N>
struct nd_array : public std::array<T, (N * ...)>  {

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

    template <typename... Idxs>
    constexpr const T& unsafe_at(Idxs... indices) const {
        return nt_element(indices...);
    }

    template <typename... Idxs>
    constexpr T& unsafe_at(Idxs... indices) {
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
    constexpr std::size_t calculate_index(std::index_sequence<Is...>,
                                          Idxs... idx) const {
        return ((coeffs[Is] * idx) + ...);
    }

    template <typename... Idxs>
    constexpr void range_check(Idxs... idx) const {
        static_assert(sizeof...(N) == sizeof...(idx), "Mismatch between number of indices provided and nd_array dimensions");
        if (not_in_range(idx...))
            throw std::out_of_range("Indices out of range");
    }

    template <typename... Idxs>
    constexpr bool not_in_range(Idxs... idx) const {
        return ((idx >= N) || ...);
    }


    std::array<uint32_t, sizeof...(N)> const coeffs = make_coeffs<N...>();
    decltype(std::make_index_sequence<sizeof...(N)>{}) const seq{};
};


template<uint32_t... N>
constexpr auto make_coeffs() {
    constexpr auto dims = std::array{N...};
    std::array<uint32_t, sizeof...(N)> coeffs{};
    coeffs[0] = 1;

    auto coeff = 1;
    for(auto i = 0; i < dims.size() - 1; ++i) {
        coeff *= dims[i];
        coeffs[i + 1] = coeff;   
    }
    return coeffs;
}