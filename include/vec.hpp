#ifndef VEC3_HPP
#define VEC3_HPP

struct Vec3 {
    float x, y, z;

    constexpr Vec3(float tx, float ty, float tz = 0.0F) : x(tx), y(ty), z(tz) {}

    constexpr Vec3 operator+(const Vec3 &other) const {
        return Vec3{x + other.x, y + other.y, z + other.z};
    }

    constexpr Vec3 operator-() const {
        return Vec3{-x, -y, -z};
    }

    constexpr Vec3 operator-(const Vec3 &other) const {
        return Vec3{x - other.x, y - other.y, z - other.z};
    }

    constexpr Vec3 operator*(float scalar) const {
        return Vec3{x * scalar, y * scalar, z * scalar};
    }

    constexpr Vec3 operator/(float scalar) const {
        return Vec3{x / scalar, y / scalar, z / scalar};
    }

    // dot product
    constexpr float operator*(const Vec3 &other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // cross product
    constexpr Vec3 operator^(const Vec3 &other) const {
        return Vec3{y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x};
    }
};

#endif // VEC3_HPP
