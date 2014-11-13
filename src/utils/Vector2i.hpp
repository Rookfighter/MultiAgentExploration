#ifndef MAE_VECTOR2I_HPP
#define MAE_VECTOR2I_HPP

#include <string>

namespace mae
{

    class Vector2i
    {
    public:
        int x;
        int y;

        Vector2i(): x(0), y(0) { }
        Vector2i(int p_x, int p_y): x(p_x), y(p_y) { }
        ~Vector2i() { }

        void set(const int p_x, const int p_y);
        int lengthSQ() const;
        int length() const;
        Vector2i perpendicular() const;

        Vector2i& operator+=(Vector2i const& p_vec);
        Vector2i& operator-=(Vector2i const& p_vec);
        Vector2i& operator*=(const int p_factor);
        Vector2i& operator/=(const int p_divisor);

        std::string str() const;
    };

    const Vector2i operator+(Vector2i const& p_vec1, Vector2i const& p_vec2);
    const Vector2i operator-(Vector2i const& p_vec1, Vector2i const& p_vec2);
    const Vector2i operator*(Vector2i const& p_vec, const int p_factor);
    const Vector2i operator*(const int p_factor, Vector2i const& p_vec);
    const Vector2i operator/(Vector2i const& p_vec, const int p_divisor);
    bool operator==(Vector2i const& p_vec1, Vector2i const& p_vec2);
    bool operator!=(Vector2i const& p_vec1, Vector2i const& p_vec2);

}

#endif
