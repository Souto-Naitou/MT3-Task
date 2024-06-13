#include "Object3d.h"
#define NOMINMAX
#define _USE_MATH_DEFINES
#include <cmath>
#include <Matrix4x4/calc/matrix4calc.h>
#include <Matrix4x4/output/matrix4out.h>
#include <Vector3/calc/vector3calc.h>
#include <Novice.h>
#include <algorithm>
#include <assert.h>
#include <type_traits>

enum class LineType
{
    Line,
    Ray,
    Segment,
};

Vector3 Perpendicular(const Vector3& _vector)
{
    if (_vector.x != 0.0f || _vector.y != 0.0f)
    {
        return { -_vector.y, _vector.x, 0.0f };
    }
    return { 0.0f, -_vector.z, _vector.y };
}

Plane MakePlane(const Triangle& _triangle)
{
    Plane result{};
    
    Vector3 v1, v2;
    v1 = Subtract(_triangle.vertices[1], _triangle.vertices[0]);
    v2 = Subtract(_triangle.vertices[2], _triangle.vertices[1]);
    result.normal = Cross(v1, v2);
    result.normal = Normalize(result.normal);
    result.distance = Dot(_triangle.vertices[0], result.normal);
    
    return result;
}

void Clamp(AABB& _aabb)
{
    _aabb.min.x = (std::min)(_aabb.min.x, _aabb.max.x);
    _aabb.max.x = (std::max)(_aabb.min.x, _aabb.max.x);
    _aabb.min.y = (std::min)(_aabb.min.y, _aabb.max.y);
    _aabb.max.y = (std::max)(_aabb.min.y, _aabb.max.y);
    _aabb.min.z = (std::min)(_aabb.min.z, _aabb.max.z);
    _aabb.max.z = (std::max)(_aabb.min.z, _aabb.max.z);
    return;
}

void ScreenPrint(const Vector3& _pos, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, const char* fmt)
{
    Vector3 screen = Transform(Transform(_pos, _viewProjectionMatrix), _viewportMatrix);
    int count = int(strlen(fmt)) - 1;
    screen.x -= 5.0f * count;
    screen.y -= 5.0f * count;
    screen.z -= 5.0f * count;
    Novice::ScreenPrintf(int(screen.x), int(screen.y), fmt);
    return;
}

void DrawLine(const Vector3& _origin, const Vector3& _diff, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color)
{
    Vector3 start = Transform(Transform(_origin, _viewProjectionMatrix), _viewportMatrix);
    Vector3 end = Transform(Transform(Add(_origin, _diff), _viewProjectionMatrix), _viewportMatrix);

    Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), _color);
    return;
}

void DrawSphere(const Sphere& _sphere, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color)
{
    const float pi = float(M_PI);
    const uint32_t kSubdivision = 16;
    const float kLonEvery = pi * 2 / kSubdivision;
    const float kLatEvery = pi / kSubdivision;
    const Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, _sphere.center);
    const Matrix4x4 wvpMatrix = Multiply(worldMatrix, _viewProjectionMatrix);
    // 緯度の方向に分割 -pi / 2 ~ pi / 2
    for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex)
    {
        float lat = -pi / 2.0f + kLatEvery * latIndex; // 現在の緯度
        // 経度方向に分割 0 ~ 2pi
        for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex)
        {
            float lon = lonIndex * kLonEvery; // 現在の経度
            // world座標系でa,b,cを求める
            Vector3 a =
            {
                _sphere.radius * std::cosf(lat) * std::cosf(lon),
                _sphere.radius * std::sinf(lat),
                _sphere.radius * std::cosf(lat) * std::sin(lon)
            };
            Vector3 b =
            {
                _sphere.radius* std::cosf(lat+kLatEvery)* std::cosf(lon),
                _sphere.radius* std::sinf(lat+kLatEvery),
                _sphere.radius* std::cosf(lat+kLatEvery)* std::sinf(lon)
            };
            Vector3 c =
            {
                _sphere.radius* std::cosf(lat)* std::cosf(lon + kLonEvery),
                _sphere.radius* std::sinf(lat),
                _sphere.radius* std::cosf(lat)* std::sinf(lon + kLonEvery)
            };
            
            Vector3 ndcA = Transform(a, wvpMatrix);
            Vector3 ndcB = Transform(b, wvpMatrix);
            Vector3 ndcC = Transform(c, wvpMatrix);

            Vector3 screenA = Transform(ndcA, _viewportMatrix);
            Vector3 screenB = Transform(ndcB, _viewportMatrix);
            Vector3 screenC = Transform(ndcC, _viewportMatrix);

            // ab,acで線を引く
            Novice::DrawLine(
                int(screenA.x), int(screenA.y),
                int(screenB.x), int(screenB.y),
                _color
            );
            Novice::DrawLine(
                int(screenA.x), int(screenA.y),
                int(screenC.x), int(screenC.y),
                _color
            );
        }
    }
}

void DrawPlane(const Plane& _plane, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color)
{
    Vector3 center = Multiply(_plane.distance, _plane.normal);
    Vector3 perpendiculars[4];
    perpendiculars[0] = Normalize(Perpendicular(_plane.normal));
    perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z };
    perpendiculars[2] = Cross(_plane.normal, perpendiculars[0]);
    perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z };

    Vector3 points[4];
    for (int32_t index = 0; index < 4; ++index)
    {
        Vector3 extend = Multiply(2.0f, perpendiculars[index]);
        Vector3 point = Add(center, extend);
        points[index] = Transform(Transform(point, _viewProjectionMatrix), _viewportMatrix);
    }

    Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[2].x), int(points[2].y), _color);
    Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[3].x), int(points[3].y), _color);
    Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[1].x), int(points[1].y), _color);
    Novice::DrawLine(int(points[3].x), int(points[3].y), int(points[0].x), int(points[0].y), _color);
    return;
}

void DrawTriangle(const Triangle& _triangle, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color)
{
    DrawLine(_triangle.vertices[0], Subtract(_triangle.vertices[1], _triangle.vertices[0]), _viewProjectionMatrix, _viewportMatrix, _color);
    DrawLine(_triangle.vertices[1], Subtract(_triangle.vertices[2], _triangle.vertices[1]), _viewProjectionMatrix, _viewportMatrix, _color);
    DrawLine(_triangle.vertices[2], Subtract(_triangle.vertices[0], _triangle.vertices[2]), _viewProjectionMatrix, _viewportMatrix, _color);
    return;
}

void DrawAABB(const AABB& _aabb, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color)
{
    Vector3 vertices[8]
    {
        {_aabb.min.x, _aabb.min.y, _aabb.min.z}, // 0
        {_aabb.min.x, _aabb.max.y, _aabb.min.z}, // 1
        {_aabb.max.x, _aabb.max.y, _aabb.min.z}, // 2
        {_aabb.max.x, _aabb.min.y, _aabb.min.z}, // 3
        {_aabb.min.x, _aabb.min.y, _aabb.max.z}, // 4
        {_aabb.min.x, _aabb.max.y, _aabb.max.z}, // 5
        {_aabb.max.x, _aabb.max.y, _aabb.max.z}, // 6
        {_aabb.max.x, _aabb.min.y, _aabb.max.z}, // 7
    };

    DrawLine(vertices[0], Subtract(vertices[1], vertices[0]), _viewProjectionMatrix, _viewportMatrix, _color); // 0 -> 1
    DrawLine(vertices[1], Subtract(vertices[2], vertices[1]), _viewProjectionMatrix, _viewportMatrix, _color); // 1 -> 2
    DrawLine(vertices[2], Subtract(vertices[3], vertices[2]), _viewProjectionMatrix, _viewportMatrix, _color); // 2 -> 3
    DrawLine(vertices[3], Subtract(vertices[0], vertices[3]), _viewProjectionMatrix, _viewportMatrix, _color); // 3 -> 0
    DrawLine(vertices[0], Subtract(vertices[4], vertices[0]), _viewProjectionMatrix, _viewportMatrix, _color); // 0 -> 4
    DrawLine(vertices[4], Subtract(vertices[5], vertices[4]), _viewProjectionMatrix, _viewportMatrix, _color); // 4 -> 5
    DrawLine(vertices[5], Subtract(vertices[6], vertices[5]), _viewProjectionMatrix, _viewportMatrix, _color); // 5 -> 6 (max)
    DrawLine(vertices[6], Subtract(vertices[7], vertices[6]), _viewProjectionMatrix, _viewportMatrix, _color); // 6 -> 7
    DrawLine(vertices[7], Subtract(vertices[4], vertices[7]), _viewProjectionMatrix, _viewportMatrix, _color); // 7 -> 4
    DrawLine(vertices[1], Subtract(vertices[5], vertices[1]), _viewProjectionMatrix, _viewportMatrix, _color); // 1 -> 5
    DrawLine(vertices[2], Subtract(vertices[6], vertices[2]), _viewProjectionMatrix, _viewportMatrix, _color); // 2 -> 6
    DrawLine(vertices[3], Subtract(vertices[7], vertices[3]), _viewProjectionMatrix, _viewportMatrix, _color); // 3 -> 7

    return;
}

void DrawOBB(const OBB& _obb, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color)
{
    Matrix4x4 rotateMatrix{};
    for (int i = 0; i < 3; i++)
        rotateMatrix.m[0][i] = _obb.orientations[i].x;
    for (int i = 0; i < 3; i++)
        rotateMatrix.m[1][i] = _obb.orientations[i].y;
    for (int i = 0; i < 3; i++)
        rotateMatrix.m[2][i] = _obb.orientations[i].z;
    rotateMatrix.m[3][3] = 1.0f;

    Vector3 vertices[8]
    {
        {-_obb.size.x, -_obb.size.y, -_obb.size.z}, // 0
        {-_obb.size.x, _obb.size.y, -_obb.size.z},  // 1
        {_obb.size.x, _obb.size.y, -_obb.size.z},   // 2
        {_obb.size.x, -_obb.size.y, -_obb.size.z},  // 3
        {-_obb.size.x, -_obb.size.y, _obb.size.z},  // 4
        {-_obb.size.x, _obb.size.y, _obb.size.z},   // 5
        {_obb.size.x, _obb.size.y, _obb.size.z},    // 6
        {_obb.size.x, -_obb.size.y, _obb.size.z},   // 7
    };

    for (int i = 0; i < 8; i++)
    {
        vertices[i] = Add(vertices[i], _obb.center);
        vertices[i] = Transform(vertices[i], rotateMatrix);
    }

    DrawLine(vertices[0], Subtract(vertices[1], vertices[0]), _viewProjectionMatrix, _viewportMatrix, _color); // 0 -> 1
    DrawLine(vertices[1], Subtract(vertices[2], vertices[1]), _viewProjectionMatrix, _viewportMatrix, _color); // 1 -> 2
    DrawLine(vertices[2], Subtract(vertices[3], vertices[2]), _viewProjectionMatrix, _viewportMatrix, _color); // 2 -> 3
    DrawLine(vertices[3], Subtract(vertices[0], vertices[3]), _viewProjectionMatrix, _viewportMatrix, _color); // 3 -> 0
    DrawLine(vertices[0], Subtract(vertices[4], vertices[0]), _viewProjectionMatrix, _viewportMatrix, _color); // 0 -> 4
    DrawLine(vertices[4], Subtract(vertices[5], vertices[4]), _viewProjectionMatrix, _viewportMatrix, _color); // 4 -> 5
    DrawLine(vertices[5], Subtract(vertices[6], vertices[5]), _viewProjectionMatrix, _viewportMatrix, _color); // 5 -> 6 (max)
    DrawLine(vertices[6], Subtract(vertices[7], vertices[6]), _viewProjectionMatrix, _viewportMatrix, _color); // 6 -> 7
    DrawLine(vertices[7], Subtract(vertices[4], vertices[7]), _viewProjectionMatrix, _viewportMatrix, _color); // 7 -> 4
    DrawLine(vertices[1], Subtract(vertices[5], vertices[1]), _viewProjectionMatrix, _viewportMatrix, _color); // 1 -> 5
    DrawLine(vertices[2], Subtract(vertices[6], vertices[2]), _viewProjectionMatrix, _viewportMatrix, _color); // 2 -> 6
    DrawLine(vertices[3], Subtract(vertices[7], vertices[3]), _viewProjectionMatrix, _viewportMatrix, _color); // 3 -> 7

    return;
}

bool IsCollision(const Sphere& _s1, const Sphere& _s2)
{
    float distance = Length(Subtract(_s1.center, _s2.center));
    if (distance <= _s1.radius + _s2.radius)
    {
        return true;
    }
    return false;
}

bool IsCollision(const Sphere& _sphere, const Plane& _plane)
{
    float k = Dot(_plane.normal, _sphere.center) - _plane.distance;
    k = sqrtf(powf(k, 2));
    return k <= _sphere.radius;
}

bool IsCollision(const Segment& _segment, const Plane& _plane)
{
    float dot = Dot(_plane.normal, _segment.diff);

    // 垂直＝平行 であるため 衝突していない
    if (dot == 0.0f)
    {
        return false;
    }

    float t = (_plane.distance - Dot(_segment.origin, _plane.normal)) / dot;

    return t > 0.0f && t <= 1.0f;
}

bool IsCollision(const Line& _line, const Plane& _plane)
{
    float dot = Dot(_plane.normal, _line.diff);

    return dot != 0.0f;
}

bool IsCollision(const Ray& _ray, const Plane& _plane)
{
    float dot = Dot(_plane.normal, _ray.diff);

    if (dot == 0.0f)
    {
        return false;
    }

    float t = (_plane.distance - Dot(_ray.origin, _plane.normal)) / dot;
    
    return t > 0.0f;
}

bool IsCollision(const Segment& _segment, const Triangle& _triangle)
{
    Plane plane = MakePlane(_triangle);
    float dot = Dot(plane.normal, _segment.diff);

    if (dot == 0.0f)
    {
        return false;
    }

    float t = (plane.distance - Dot(_segment.origin, plane.normal)) / dot;

    if (t > 1.0f)
    {
        return false;
    }

    Vector3 p = Add(_segment.origin, Multiply(t, _segment.diff));

    Vector3 v0p = Subtract(p, _triangle.vertices[0]);
    Vector3 v1p = Subtract(p, _triangle.vertices[1]);
    Vector3 v2p = Subtract(p, _triangle.vertices[2]);

    Vector3 v01 = Subtract(_triangle.vertices[1], _triangle.vertices[0]);
    Vector3 v12 = Subtract(_triangle.vertices[2], _triangle.vertices[1]);
    Vector3 v20 = Subtract(_triangle.vertices[0], _triangle.vertices[2]);

    Vector3 cross01 = Cross(v01, v1p);
    Vector3 cross12 = Cross(v12, v2p);
    Vector3 cross20 = Cross(v20, v0p);

    if (Dot(cross01, plane.normal) >= 0.0f &&
        Dot(cross12, plane.normal) >= 0.0f &&
        Dot(cross20, plane.normal) >= 0.0f) {
        return true;
    }

    return false;
}

bool IsCollision(const Segment& _segment, const Triangle& _triangle, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix)
{
    Plane plane = MakePlane(_triangle);
    DrawPlane(plane, _viewProjectionMatrix, _viewportMatrix, 0x00ff00ff);
    float dot = Dot(plane.normal, _segment.diff);

    if (dot == 0.0f)
    {
        return false;
    }

    float t = (plane.distance - Dot(_segment.origin, plane.normal)) / dot;
    
    if (t > 1.0f)
    {
        return false;
    }

    Vector3 p = Add(_segment.origin, Multiply(t, _segment.diff));

    Sphere sphere{};
    sphere.center = p;
    sphere.radius = 0.01f;

    DrawSphere(sphere, _viewProjectionMatrix, _viewportMatrix, GREEN);

    Vector3 v0p = Subtract(p, _triangle.vertices[0]);
    Vector3 v1p = Subtract(p, _triangle.vertices[1]);
    Vector3 v2p = Subtract(p, _triangle.vertices[2]);

    Vector3 v01 = Subtract(_triangle.vertices[1], _triangle.vertices[0]);
    Vector3 v12 = Subtract(_triangle.vertices[2], _triangle.vertices[1]);
    Vector3 v20 = Subtract(_triangle.vertices[0], _triangle.vertices[2]);

    Vector3 cross01 = Cross(v01, v1p);
    Vector3 cross12 = Cross(v12, v2p);
    Vector3 cross20 = Cross(v20, v0p);

    if (Dot(cross01, plane.normal) >= 0.0f &&
        Dot(cross12, plane.normal) >= 0.0f &&
        Dot(cross20, plane.normal) >= 0.0f) {
        return true;
    }
    
    return false;
}

bool IsCollision(const AABB& _aabb1, const AABB& _aabb2)
{
    if (_aabb1.max.x >= _aabb2.min.x && _aabb1.min.x <= _aabb2.max.x &&
        _aabb1.max.y >= _aabb2.min.y && _aabb1.min.y <= _aabb2.max.y &&
        _aabb1.max.z >= _aabb2.min.z && _aabb1.min.z <= _aabb2.max.z)
    {
        return true;
    }

    return false;
}

bool IsCollision(const AABB& _aabb, const Sphere& _sphere)
{
    Vector3 _closestPoint{
        std::clamp(_sphere.center.x, _aabb.min.x, _aabb.max.x),
        std::clamp(_sphere.center.y, _aabb.min.y, _aabb.max.y),
        std::clamp(_sphere.center.z, _aabb.min.z, _aabb.max.z)
    };
    float distance = Length(_closestPoint - _sphere.center);
    if (distance <= _sphere.radius)
    {
        return true;
    }
    return false;
}

Vector3 CalcParametricValue(const AABB& _aabb, const Vector3& _origin, const Vector3& _diff)
{
    float txmin = (_aabb.min.x - _origin.x) / _diff.x;
    float txmax = (_aabb.max.x - _origin.x) / _diff.x;
    float tymin = (_aabb.min.y - _origin.y) / _diff.y;
    float tymax = (_aabb.max.y - _origin.y) / _diff.y;
    float tzmin = (_aabb.min.z - _origin.z) / _diff.z;
    float tzmax = (_aabb.max.z - _origin.z) / _diff.z;

    float tNearX = std::min(txmin, txmax);
    float tNearY = std::min(tymin, tymax);
    float tNearZ = std::min(tzmin, tzmax);

    float tFarX = std::max(txmin, txmax);
    float tFarY = std::max(tymin, tymax);
    float tFarZ = std::max(tzmin, tzmax);

    float tmin = std::max(std::max(tNearX, tNearY), tNearZ);
    float tmax = std::min(std::min(tFarX, tFarY), tFarZ);

    return Vector3{ .x = tmin, .y = tmax, .z = 0.0f};
}

bool IsCollision(const AABB& _aabb, const Segment& _segment)
{
    // すべて0だったらアサーション
    for (int i = 0; i < 3; i++)
    {
        if (*(&_segment.diff.x + i) != 0)
        {
            break;
        }
        if (i == 2)
        {
            assert(false);
        }
    }

    Vector3 t = CalcParametricValue(_aabb, _segment.origin, _segment.diff);
    float tmin = t.x;
    float tmax = t.y;

    if (tmin > 0.0f && tmin <= 1.0f && tmin <= tmax) return true;
    return false;
}

bool IsCollision(const AABB& _aabb, const Line& _line)
{
    // すべて0だったらアサーション
    for (int i = 0; i < 3; i++)
    {
        if (*(&_line.diff.x + i) != 0)
        {
            break;
        }
        if (i == 2)
        {
            assert(false);
        }
    }

    Vector3 t = CalcParametricValue(_aabb, _line.origin, _line.diff);
    float tmin = t.x;
    float tmax = t.y;

    if (tmin <= tmax) return true;
    return false;
}

bool IsCollision(const AABB& _aabb, const Ray& _ray)
{
    // すべて0だったらアサーション
    for (int i = 0; i < 3; i++)
    {
        if (*(&_ray.diff.x + i) != 0)
        {
            break;
        }
        if (i == 2)
        {
            assert(false);
        }
    }

    Vector3 t = CalcParametricValue(_aabb, _ray.origin, _ray.diff);
    float tmin = t.x;
    float tmax = t.y;
    
    if (tmin <= tmax && tmin > 0.0f) return true;

    return false;
}

bool IsCollision(const OBB& _obb, const Sphere& _sphere)
{
    Matrix4x4 obbWorldMatrix{};
    for (int i = 0; i < 3; i++)
        obbWorldMatrix.m[0][i] = _obb.orientations[i].x;
    for (int i = 0; i < 3; i++)
        obbWorldMatrix.m[1][i] = _obb.orientations[i].y;
    for (int i = 0; i < 3; i++)
        obbWorldMatrix.m[2][i] = _obb.orientations[i].z;
    for (int i = 0; i < 3; i++)
        obbWorldMatrix.m[3][i] = *(&_obb.center.x + i);
    obbWorldMatrix.m[3][3] = 1.0f;

    Matrix4x4 obbWorldMatrixInverse = Inverse(obbWorldMatrix);

    Vector3 centerInOBBLocalSpace = Transform(_sphere.center, obbWorldMatrixInverse);

    AABB aabbOBBLocal{ .min = -_obb.size, .max = _obb.size };
    Sphere sphereOBBLocal{ centerInOBBLocalSpace, _sphere.radius };
    
    return IsCollision(aabbOBBLocal, sphereOBBLocal);
}

bool IsCollision(const OBB& _obb, const Sphere& _sphere, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix)
{
    Matrix4x4 obbWorldMatrix{};
    for (int i = 0; i < 3; i++)
        obbWorldMatrix.m[0][i] = _obb.orientations[i].x;
    for (int i = 0; i < 3; i++)
        obbWorldMatrix.m[1][i] = _obb.orientations[i].y;
    for (int i = 0; i < 3; i++)
        obbWorldMatrix.m[2][i] = _obb.orientations[i].z;
    for (int i = 0; i < 3; i++)
        obbWorldMatrix.m[3][i] = *(&_obb.center.x + i);
    obbWorldMatrix.m[3][3] = 1.0f;

    Matrix4x4 obbWorldMatrixInverse = Inverse(obbWorldMatrix);

    Vector3 centerInOBBLocalSpace = Transform(_sphere.center, obbWorldMatrixInverse);

    AABB aabbOBBLocal{ .min = -_obb.size, .max = _obb.size };
    Sphere sphereOBBLocal{ centerInOBBLocalSpace, _sphere.radius };

    DrawAABB(aabbOBBLocal, _viewProjectionMatrix, _viewportMatrix, BLUE);
    DrawSphere(sphereOBBLocal, _viewProjectionMatrix, _viewportMatrix, BLUE);
    
    return IsCollision(aabbOBBLocal, sphereOBBLocal);
}
