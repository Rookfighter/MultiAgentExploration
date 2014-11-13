#include <cmath>
#include <sstream>
#include "utils/Vector2i.hpp"

namespace mae
{

    void Vector2i::set(const int p_x, const int p_y)
    {
        x = p_x;
        y = p_y;
    }

    int Vector2i::lengthSQ() const
    {
        return x * x + y * y;
    }

    int Vector2i::length() const
    {
        return sqrt(lengthSQ());
    }

    Vector2i Vector2i::perpendicular() const
    {
        return Vector2i(-y, x);
    }

    Vector2i& Vector2i::operator+=(Vector2i const& p_vec)
    {
        x += p_vec.x;
        y += p_vec.y;

        return *this;
    }

    Vector2i& Vector2i::operator-=(Vector2i const& p_vec)
    {
        x -= p_vec.x;
        y -= p_vec.y;

        return *this;
    }

    Vector2i& Vector2i::operator*=(const int p_factor)
    {
        x *= p_factor;
        y *= p_factor;

        return *this;
    }

    Vector2i& Vector2i::operator/=(const int p_divisor)
    {
        x /= p_divisor;
        y /= p_divisor;

        return *this;
    }

    std::string Vector2i::str() const
    {
        std::stringstream ss;
        ss.precision(2);
        ss << "(" << x << ";" << y << ")";
        return ss.str();
    }

    const Vector2i operator+(Vector2i const& p_vec1, Vector2i const& p_vec2)
    {
        Vector2i result(p_vec1);
        result += p_vec2;

        return result;
    }

    const Vector2i operator-(Vector2i const& p_vec1, Vector2i const& p_vec2)
    {
        Vector2i result(p_vec1);
        result -= p_vec2;

        return result;
    }

    const Vector2i operator*(Vector2i const& p_vec, const int p_factor)
    {
        Vector2i result(p_vec);
        result *= p_factor;

        return result;
    }

    const Vector2i operator*(const int p_factor, Vector2i const& p_vec)
    {
        return p_vec * p_factor;
    }

    const Vector2i operator/(Vector2i const& p_vec, const int p_divisor)
    {
        Vector2i result(p_vec);
        result /= p_divisor;

        return result;
    }

    bool operator==(Vector2i const& p_vec1, Vector2i const& p_vec2)
    {
        return p_vec1.x == p_vec2.x && p_vec1.y == p_vec2.y;
    }

    bool operator!=(Vector2i const& p_vec1, Vector2i const& p_vec2)
    {
        return !(p_vec1 == p_vec2);
    }
}
