#ifndef MAE_VECTOR2F_HPP
#define MAE_VECTOR2F_HPP

#include <string>

namespace mae
{

    class Vector2f
    {
    public:
        double x;
        double y;

        Vector2f(): x(0), y(0) { }
        Vector2f(double p_x, double p_y): x(p_x), y(p_y) { }
        ~Vector2f() { }

        void set(const double p_x, const double p_y);
        double lengthSQ() const;
        double length() const;
        Vector2f perpendicular() const;

        Vector2f& operator+=(Vector2f const& p_vec);
        Vector2f& operator-=(Vector2f const& p_vec);
        Vector2f& operator*=(const double p_factor);
        Vector2f& operator/=(const double p_divisor);

        std::string str() const;
    };

    const Vector2f operator+(Vector2f const& p_vec1, Vector2f const& p_vec2);
    const Vector2f operator-(Vector2f const& p_vec1, Vector2f const& p_vec2);
    const Vector2f operator*(Vector2f const& p_vec, const double p_factor);
    const Vector2f operator*(const double p_factor, Vector2f const& p_vec);
    const Vector2f operator/(Vector2f const& p_vec, const double p_divisor);
    bool operator==(Vector2f const& p_vec1, Vector2f const& p_vec2);
    bool operator!=(Vector2f const& p_vec1, Vector2f const& p_vec2);

}

#endif
