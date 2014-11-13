#include <cmath>
#include <sstream>
#include "utils/Vector2f.hpp"

namespace mae
{

    void Vector2f::set(const double p_x, const double p_y)
    {
        x = p_x;
        y = p_y;
    }

    double Vector2f::lengthSQ() const
    {
        return x * x + y * y;
    }

    double Vector2f::length() const
    {
        return sqrt(lengthSQ());
    }

    Vector2f Vector2f::perpendicular() const
    {
        return Vector2f(-y, x);
    }

    Vector2f& Vector2f::operator+=(Vector2f const& p_vec)
    {
        x += p_vec.x;
        y += p_vec.y;

        return *this;
    }

    Vector2f& Vector2f::operator-=(Vector2f const& p_vec)
    {
        x -= p_vec.x;
        y -= p_vec.y;

        return *this;
    }

    Vector2f& Vector2f::operator*=(const double p_factor)
    {
        x *= p_factor;
        y *= p_factor;

        return *this;
    }

    Vector2f& Vector2f::operator/=(const double p_divisor)
    {
        x /= p_divisor;
        y /= p_divisor;

        return *this;
    }

    std::string Vector2f::str() const
    {
        std::stringstream ss;
        ss.precision(2);
        ss << "(" << x << ";" << y << ")";
        return ss.str();
    }

    const Vector2f operator+(Vector2f const& p_vec1, Vector2f const& p_vec2)
    {
        Vector2f result(p_vec1);
        result += p_vec2;

        return result;
    }

    const Vector2f operator-(Vector2f const& p_vec1, Vector2f const& p_vec2)
    {
        Vector2f result(p_vec1);
        result -= p_vec2;

        return result;
    }

    const Vector2f operator*(Vector2f const& p_vec, const double p_factor)
    {
        Vector2f result(p_vec);
        result *= p_factor;

        return result;
    }

    const Vector2f operator*(const double p_factor, Vector2f const& p_vec)
    {
        return p_vec * p_factor;
    }

    const Vector2f operator/(Vector2f const& p_vec, const double p_divisor)
    {
        Vector2f result(p_vec);
        result /= p_divisor;

        return result;
    }

    bool operator==(Vector2f const& p_vec1, Vector2f const& p_vec2)
    {
        return p_vec1.x == p_vec2.x && p_vec1.y == p_vec2.y;
    }

    bool operator!=(Vector2f const& p_vec1, Vector2f const& p_vec2)
    {
        return !(p_vec1 == p_vec2);
    }
}
