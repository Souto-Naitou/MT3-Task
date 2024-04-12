#include "vector3calc.h"

#include <math.h>

Vector3 Add(const Vector3& _v1, const Vector3& _v2)
{
    return Vector3(
        _v1.x + _v2.x,
        _v1.y + _v2.y,
        _v1.z + _v2.z
    );
}

Vector3 Subtract(const Vector3& _v1, const Vector3& _v2)
{
    return Vector3(
        _v1.x - _v2.x,
        _v1.y - _v2.y,
        _v1.z - _v2.z
    );
}

Vector3 Multiply(float _scalar, const Vector3& _v)
{
    return Vector3(
        _scalar * _v.x,
        _scalar * _v.y,
        _scalar * _v.z
    );
}

float Dot(const Vector3& _v1, const Vector3& _v2)
{
    return float(
        _v1.x * _v2.x +
        _v1.y * _v2.y +
        _v1.z * _v2.z
        );
}

float Length(const Vector3& _v)
{
    return sqrtf(
        _v.x * _v.x +
        _v.y * _v.y +
        _v.z * _v.z
        );
}

Vector3 Normalize(const Vector3& _v)
{
    float length = Length(_v);
    return Vector3(
        _v.x / length,
        _v.y / length,
        _v.z / length
        );
}
