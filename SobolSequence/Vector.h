#pragma once

#include <math.h>

#define Assert(expr) \
    ((expr) ? (void)0 : \
        printf("Assertion \"%s\" failed in %s, line %d"))

// Geometry Declarations
class Vector {
public:
    // Vector Public Methods
    Vector() { x = y = z = 0.f; }
    Vector(float xx, float yy, float zz)
        : x(xx), y(yy), z(zz) {
        Assert(!HasNaNs());
    }
	bool HasNaNs() { return this ->isnan(x) || isnan((float)y) || isnan((float)z); }
#ifndef NDEBUG
    // The default versions of these are fine for release builds; for debug
    // we define them so that we can add the Assert checks.
    Vector(const Vector &v) {
        x = v.x; y = v.y; z = v.z;
    }
    
    Vector &operator=(const Vector &v) {
        x = v.x; y = v.y; z = v.z;
        return *this;
    }
#endif // !NDEBUG
    Vector operator+(const Vector &v) const {
        return Vector(x + v.x, y + v.y, z + v.z);
    }
    
    Vector& operator+=(const Vector &v) {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }
    Vector operator-(const Vector &v) const {
        return Vector(x - v.x, y - v.y, z - v.z);
    }
    
    Vector& operator-=(const Vector &v) {
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }
    Vector operator*(float f) const { return Vector(f*x, f*y, f*z); }
    
    Vector &operator*=(float f) {
        x *= f; y *= f; z *= f;
        return *this;
    }
    Vector operator/(float f) const {
        float inv = 1.f / f;
        return Vector(x * inv, y * inv, z * inv);
    }
    
    Vector &operator/=(float f) {
        float inv = 1.f / f;
        x *= inv; y *= inv; z *= inv;
        return *this;
    }
    Vector operator-() const { return Vector(-x, -y, -z); }
    float operator[](int i) const {
        return (&x)[i];
    }
    
    float &operator[](int i) {
        return (&x)[i];
    }
    float LengthSquared() const { return x*x + y*y + z*z; }
    float Length() const { return sqrt(LengthSquared()); }
	inline void Normalize() { float oneOverLength = Length(); oneOverLength = 1.0 / oneOverLength; x *= oneOverLength; y *= oneOverLength; z *= oneOverLength; }

    bool operator==(const Vector &v) const {
        return x == v.x && y == v.y && z == v.z;
    }
    bool operator!=(const Vector &v) const {
        return x != v.x || y != v.y || z != v.z;
    }

    // Vector Public Data
    float x, y, z;
private:
	inline bool isnan(float x) { return false; }
	inline bool isnan(double x) { return false; }
};

inline Vector Cross(const Vector &v1, const Vector &v2) {
	double v1x = v1.x, v1y = v1.y, v1z = v1.z;
	double v2x = v2.x, v2y = v2.y, v2z = v2.z;
	return Vector((v1y * v2z) - (v1z * v2y),
		(v1z * v2x) - (v1x * v2z),
		(v1x * v2y) - (v1y * v2x));
}

inline float Dot(const Vector &v1, const Vector &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

