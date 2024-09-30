#ifndef COLOR_HPP
#define COLOR_HPP

#include "vec.hpp"

struct Color {
    float r, g, b;

    constexpr Color(float tr, float tg, float tb) : r(tr), g(tg), b(tb) {}

    constexpr operator Vec3() const {
        return Vec3{r, g, b};
    }
};

constexpr Color RED{1.0F, 0.0F, 0.0F};
constexpr Color GREEN{0.0F, 1.0F, 0.0F};
constexpr Color BLUE{0.0F, 0.0F, 1.0F};

constexpr Color WHITE{1.0F, 1.0F, 1.0F};
constexpr Color BLACK{0.0F, 0.0F, 0.0F};

#endif // COLOR_HPP
