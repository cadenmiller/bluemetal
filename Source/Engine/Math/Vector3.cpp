#include "Vector3.h"

//===========================
// blVector3i
//===========================

blVector3i::blVector3i() noexcept
    : x(0), y(0), z(0)
{
}

blVector3i::blVector3i(int x, int y, int z) noexcept
    : x(x), y(y), z(z)
{
}

blVector3i::blVector3i(int xyz) noexcept
    : x(xyz), y(xyz), z(xyz)
{
}

blVector3i::blVector3i(const blVector3i& rhs) noexcept
    : x(rhs.x), y(rhs.y), z(rhs.z)
{
}

blVector3i& blVector3i::operator=(const blVector3i& rhs) noexcept
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    return *this;
}

blVector3i& blVector3i::operator+(const blVector3i& rhs) noexcept
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

blVector3i& blVector3i::operator-(const blVector3i& rhs) noexcept
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

blVector3i& blVector3i::operator*(const blVector3i& rhs) noexcept
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
}

blVector3i& blVector3i::operator/(const blVector3i& rhs) noexcept
{
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    return *this;
}

int blVector3i::magnitude() const noexcept
{
    return (int)std::sqrt(x*x + y*y + z*z);
}

int blVector3i::distance(const blVector3i& other) const noexcept
{
    const int xm = (other.x - x);
    const int ym = (other.y - y);
    const int zm = (other.z - z);
    return (int)std::sqrt(xm*xm + ym*ym + zm*zm);
}

//===========================
// blVector3f
//===========================

blVector3f::blVector3f() noexcept
    : x(0), y(0), z(0)
{
}

blVector3f::blVector3f(float x, float y, float z) noexcept
    : x(x), y(y), z(z)
{
}

blVector3f::blVector3f(float xyz) noexcept
    : x(xyz), y(xyz), z(xyz)
{
}

blVector3f& blVector3f::operator=(const blVector3f& rhs) noexcept
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    return *this;
}

blVector3f& blVector3f::operator+(const blVector3f& rhs) noexcept
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

blVector3f& blVector3f::operator-(const blVector3f& rhs) noexcept
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

blVector3f& blVector3f::operator*(const blVector3f& rhs) noexcept
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
}

blVector3f& blVector3f::operator/(const blVector3f& rhs) noexcept
{
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    return *this;
}

float blVector3f::magnitude() const noexcept
{
    return std::sqrt(x*x + y*y + z*z);
}

float blVector3f::distance(const blVector3f& other) const noexcept
{
    const float xm = (other.x - x);
    const float ym = (other.y - y);
    const float zm = (other.z - z); 
    return std::sqrt(xm*xm + ym*ym + zm*zm);
}

//===========================
// blExtent3D
//===========================

blExtent3D::blExtent3D(unsigned int width, unsigned int height, unsigned int depth)
    : width(width), height(height), depth(depth)
{
}

blExtent3D::blExtent3D(unsigned int widthHeightDepth)
    : width(widthHeightDepth), height(widthHeightDepth), depth(widthHeightDepth)
{
}

blExtent3D::blExtent3D(VkExtent3D extent)
    : width(extent.width), height(extent.height), depth(extent.depth)
{   
}

blExtent3D::operator VkExtent3D()
{
    return VkExtent3D(width, height, depth);
}