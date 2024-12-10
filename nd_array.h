#pragma once

#include <array>
#include <cstdint>
#include <stdexcept>

template<uint32_t... N>
constexpr auto make_steps() noexcept;

template <typename T, uint32_t... N>
struct nd_array : public std::array<T, (N * ...)>  {
    std::array<uint32_t, sizeof...(N)> const steps = make_steps<N...>();
    decltype(std::make_index_sequence<sizeof...(N)>{}) const seq{};

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
    constexpr const T& unsafe_at(Idxs... indices) const noexcept {
        return nt_element(indices...);
    }

    template <typename... Idxs>
    constexpr T& unsafe_at(Idxs... indices) noexcept {
        return nt_element(indices...);
    }

private:

    using super = std::array<T, (N * ...)>;

    template <typename... Idxs>
    constexpr const T& nt_element(Idxs... indices) const noexcept {
        return super::operator[](calculate_index(seq, indices...));
    }

    template <typename... Idxs>
    constexpr T& nt_element(Idxs... indices) noexcept {
        return super::operator[](calculate_index(seq, indices...));
    }

    template <typename... Idxs, std::size_t... Is>
    constexpr std::size_t calculate_index(std::index_sequence<Is...>,
                                          Idxs... idx) const noexcept {
        return ((steps[Is] * idx) + ...);
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
};


template<uint32_t... N>
constexpr auto make_steps() noexcept {
    constexpr auto dims = std::array{N...};
    std::array<uint32_t, sizeof...(N)> steps{};
    steps[0] = 1;
    for(auto i = 0; i < dims.size() - 1; ++i) {
        steps[i + 1] = dims[i] * steps[i];   
    }
    return steps;
}

