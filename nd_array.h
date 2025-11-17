#pragma once

#include <array>
#include <concepts>
#include <cstddef>
#include <stdexcept>
#include <compare>

// C++20 Concepts for better type safety and error messages
template<typename T>
concept Integral = std::integral<T>;

template<typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

// Compile-time calculation of steps/strides for multidimensional indexing
template<std::size_t... N>
consteval auto make_steps() noexcept {
    constexpr auto dims = std::array{N...};
    std::array<std::size_t, sizeof...(N)> steps{};

    if constexpr (sizeof...(N) > 0) {
        steps[0] = 1;
        for (std::size_t i = 0; i < dims.size() - 1; ++i) {
            steps[i + 1] = dims[i] * steps[i];
        }
    }

    return steps;
}

template <Arithmetic T, std::size_t... N>
    requires (sizeof...(N) > 0) && ((N > 0) && ...)
struct nd_array : public std::array<T, (N * ...)> {
private:
    using super = std::array<T, (N * ...)>;

    // Static constexpr steps - computed once at compile-time, no per-instance overhead
    static constexpr auto steps = make_steps<N...>();
    static constexpr auto dimensions = std::array{N...};

public:
    // Type aliases for better API
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = typename super::iterator;
    using const_iterator = typename super::const_iterator;
    using reverse_iterator = typename super::reverse_iterator;
    using const_reverse_iterator = typename super::const_reverse_iterator;

    // Expose iterator interface from base class for range-based operations
    using super::begin;
    using super::end;
    using super::cbegin;
    using super::cend;
    using super::rbegin;
    using super::rend;
    using super::crbegin;
    using super::crend;

    // Checked access with range validation
    template <Integral... Idxs>
        requires (sizeof...(Idxs) == sizeof...(N))
    [[nodiscard]] constexpr const_reference at(Idxs... indices) const {
        range_check(indices...);
        return nt_element(indices...);
    }

    template <Integral... Idxs>
        requires (sizeof...(Idxs) == sizeof...(N))
    [[nodiscard]] constexpr reference at(Idxs... indices) {
        range_check(indices...);
        return nt_element(indices...);
    }

    // Unchecked access for performance-critical code
    template <Integral... Idxs>
        requires (sizeof...(Idxs) == sizeof...(N))
    [[nodiscard]] [[gnu::always_inline]] constexpr const_reference unsafe_at(Idxs... indices) const noexcept {
        return nt_element(indices...);
    }

    template <Integral... Idxs>
        requires (sizeof...(Idxs) == sizeof...(N))
    [[nodiscard]] [[gnu::always_inline]] constexpr reference unsafe_at(Idxs... indices) noexcept {
        return nt_element(indices...);
    }

    // C++23 multidimensional subscript operator
#if __cpp_multidimensional_subscript >= 202110L
    template <Integral... Idxs>
        requires (sizeof...(Idxs) == sizeof...(N))
    [[nodiscard]] constexpr const_reference operator[](Idxs... indices) const noexcept {
        return nt_element(indices...);
    }

    template <Integral... Idxs>
        requires (sizeof...(Idxs) == sizeof...(N))
    [[nodiscard]] constexpr reference operator[](Idxs... indices) noexcept {
        return nt_element(indices...);
    }
#endif

    // Utility functions
    [[nodiscard]] static constexpr std::size_t dimension_count() noexcept {
        return sizeof...(N);
    }

    [[nodiscard]] static constexpr std::size_t dimension_size(std::size_t dim) noexcept {
        return dim < sizeof...(N) ? dimensions[dim] : 0;
    }

    [[nodiscard]] static constexpr auto get_dimensions() noexcept {
        return dimensions;
    }

    // C++20 spaceship operator for comparisons
    [[nodiscard]] constexpr auto operator<=>(const nd_array&) const noexcept = default;

private:
    // Core element access - hot path function
    template <Integral... Idxs>
    [[nodiscard]] [[gnu::always_inline]] constexpr const_reference nt_element(Idxs... indices) const noexcept {
        return super::operator[](calculate_index(std::index_sequence_for<Idxs...>{}, indices...));
    }

    template <Integral... Idxs>
    [[nodiscard]] [[gnu::always_inline]] constexpr reference nt_element(Idxs... indices) noexcept {
        return super::operator[](calculate_index(std::index_sequence_for<Idxs...>{}, indices...));
    }

    // Compile-time index calculation using fold expressions
    template <Integral... Idxs, std::size_t... Is>
    [[nodiscard]] [[gnu::always_inline]] static constexpr std::size_t calculate_index(
        std::index_sequence<Is...>, Idxs... idx) noexcept {
        return ((steps[Is] * static_cast<std::size_t>(idx)) + ...);
    }

    // Range validation
    template <Integral... Idxs>
    static constexpr void range_check(Idxs... idx) {
        if (not_in_range(idx...)) {
            throw std::out_of_range("nd_array: indices out of range");
        }
    }

    template <Integral... Idxs>
    [[nodiscard]] static constexpr bool not_in_range(Idxs... idx) noexcept {
        return ((static_cast<std::size_t>(idx) >= N) || ...);
    }
};

// C++17 Deduction guides for CTAD (Class Template Argument Deduction)
template<typename T, typename... U>
nd_array(T, U...) -> nd_array<T, sizeof...(U) + 1>;
