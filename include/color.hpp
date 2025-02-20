#ifndef COLOR_HPP
#define COLOR_HPP

#include <cmath>

namespace utils::color {

struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

struct HSV {
    float h;
    float s;
    float v;
};

struct float4 {
    float x;
    float y;
    float z;
    float w;
};

constexpr float _epsilon = 1e-5F;

constexpr Color BLACK   = {0, 0, 0, 255};
constexpr Color WHITE   = {255, 255, 255, 255};
constexpr Color RED     = {255, 0, 0, 255};
constexpr Color GREEN   = {0, 255, 0, 255};
constexpr Color BLUE    = {0, 0, 255, 255};
constexpr Color YELLOW  = {255, 255, 0, 255};
constexpr Color MAGENTA = {255, 0, 255, 255};
constexpr Color CYAN    = {0, 255, 255, 255};

constexpr bool operator==(const Color& l, const Color& r) {
    return l.r == r.r && l.g == r.g && l.b == r.b && l.a == r.a;
}

constexpr bool operator==(const HSV& l, const HSV& r) {
    return std::abs(l.h - r.h) < _epsilon &&
           std::abs(l.s - r.s) < _epsilon &&
           std::abs(l.v - r.v) < _epsilon;
}

constexpr bool operator==(const float4& l, const float4& r) {
    return std::abs(l.x - r.x) < _epsilon &&
           std::abs(l.y - r.y) < _epsilon &&
           std::abs(l.z - r.z) < _epsilon &&
           std::abs(l.w - r.w) < _epsilon;
}

// I think compilers implicitly generate these from operator== now
constexpr bool operator!=(const Color& l, const Color& r) {
    return !(l == r);
}

constexpr bool operator!=(const HSV& l, const HSV& r) {
    return !(l == r);
}

constexpr bool operator!=(const float4& l, const float4& r) {
    return !(l == r);
}

constexpr unsigned int to_hex(const Color& color) {
    unsigned int hex = 0;
    hex = static_cast<unsigned char>(color.r) << 24 | static_cast<unsigned char>(color.g) << 16 |
          static_cast<unsigned char>(color.b) << 8  | static_cast<unsigned char>(color.a);
    return hex;
}

constexpr Color from_hex(const unsigned int hex) {
    Color color{};
    color.r = static_cast<unsigned char>((hex >> 24) & 0xFF);
    color.g = static_cast<unsigned char>((hex >> 16) & 0xFF);
    color.b = static_cast<unsigned char>((hex >> 8)  & 0xFF);
    color.a = static_cast<unsigned char>( hex        & 0xFF);
    return color;
}

constexpr float4 normalize_color(const Color& color) {
    float4 vec4{};
    vec4.x = static_cast<float>(color.r) / 255.0F;
    vec4.y = static_cast<float>(color.g) / 255.0F;
    vec4.z = static_cast<float>(color.b) / 255.0F;
    vec4.w = static_cast<float>(color.a) / 255.0F;
    return vec4;
}

// naming
constexpr Color to_color(const float4& vec4) {
    Color color{};
    color.r = static_cast<unsigned char>(vec4.x * 255.0F);
    color.g = static_cast<unsigned char>(vec4.y * 255.0F);
    color.b = static_cast<unsigned char>(vec4.z * 255.0F);
    color.a = static_cast<unsigned char>(vec4.w * 255.0F);
    return color;
}

constexpr float _min(const float a, const float b) {
    return a < b ? a : b;
}

constexpr float _max(const float a, const float b) {
    return a > b ? a : b;
}

constexpr HSV rgb_to_hsv(const Color& rgba) {
    HSV hsv{};
    auto [r, g, b, _] = normalize_color(rgba);

    const float max = _max(r, _max(g, b));
    const float min = _min(r, _min(g, b));
    const float chroma = max - min;

    hsv.v = max;

    if (chroma == 0.0F) {
        hsv.h = 0.0F;
    } else if (std::abs(max - r) < _epsilon) {
        hsv.h = 60.0F * std::fmodf((g - b) / chroma, 6.0F);
    } else if (std::abs(max - g) < _epsilon) {
        hsv.h = 60.0F * ((b - r) / chroma + 2.0F);
    } else if (std::abs(max - b) < _epsilon) {
        hsv.h = 60.0F * ((r - g) / chroma + 4.0F);
    }

    if (hsv.h < 0.0F) {
        hsv.h += 360.0F;
    }

    if (std::abs(max) < _epsilon) {
        hsv.s = 0.0F;
    } else {
        hsv.s = chroma / max;
    }

    return hsv;
}

constexpr Color hsv_to_rgb(const HSV& hsv) {
    // f(n) = v - v * s * max(0, min(k, 4 - k, 1))
    // where k = (n + h / 60) % 6
    // and rgb = f(5), f(3), f(1)

    const float kr = std::fmodf(5.0F + hsv.h / 60.0F, 6.0F);
    const float fr = hsv.v - hsv.v * hsv.s * _max(0.0F, _min(kr, _min(4.0F - kr, 1.0F)));

    const float kg = std::fmodf(3.0F + hsv.h / 60.0F, 6.0F);
    const float fg = hsv.v - hsv.v * hsv.s * _max(0.0F, _min(kg, _min(4.0F - kg, 1.0F)));

    const float kb = std::fmodf(1.0F + hsv.h / 60.0F, 6.0F);
    const float fb = hsv.v - hsv.v * hsv.s * _max(0.0F, _min(kb, _min(4.0F - kb, 1.0F)));

    return to_color({.x = fr, .y = fg, .z = fb, .w = 1.0F});
}

constexpr Color rgb_to_grayscale(const Color& color) {
    const auto [r, g, b, a] = normalize_color(color);
    const float gray = 0.299F * r + 0.587F * g + 0.114F * b;
    return to_color({.x = gray, .y = gray, .z = gray, .w = a});
}

} // namespace utils::color

#endif // COLOR_HPP
