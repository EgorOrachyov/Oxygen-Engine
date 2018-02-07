/*
 * Project: Oxygen Game Engine
 * Created by Egor Orachyov on 29.01.2018.
 * Copyright
 *
 * xBaseGeometry provides simple and common
 * structures for working with 3d objects
 * in rendering module as well as in audio
 * and physics modules
 *
 * xVertex, xEdge and xPolygon releases an
 * index-based technique of the dating the
 * information about dotes, lines and polygons
 * int the renderer (info about that will be
 * stored int the dynamic arrays)
 */

#ifndef OXYGEN_XBASEGEOMETRY_H
#define OXYGEN_XBASEGEOMETRY_H

#include "xEngine.h"

// ----------------------------------------------------------------------
// Base structures for 3d data
// ----------------------------------------------------------------------

struct xVector2
{
public:
    float x, y;

    // ----------------------------------------------------------------------
    // Creates the vector from x y components
    // ----------------------------------------------------------------------
    xVector2(float x = 0.0, float y = 0.0) {
        this->x = x;
        this->y = y;
    }

    void Set(float x, float y) {
        this->x = x;
        this->y = y;
    }

    void Set(xVector2 * value) {
        this->x = value->x;
        this->y = value->y;
    }

    float Module() {
        return sqrt(x * x + y * y);
    }

    void Normalize() {
        float m = Module();
        if (!m) {
            x /= m;
            y /= m;
        }
    }

    void Sum(xVector2 * value) {
        x += value->x;
        y += value->y;
    }

    void Diff(xVector2 * value) {
        x -= value->x;
        y -= value->y;
    }

    void MultScalar(float value) {
        x *= value;
        y *= value;
    }
};

struct xVector3
{
public:
    float x, y, z;

    // ----------------------------------------------------------------------
    // Creates the vector from x y z components
    // ----------------------------------------------------------------------
    xVector3(float x = 0.0, float y = 0.0, float z = 0.0) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void Set(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void Set(xVector3 * value) {
        this->x = value->x;
        this->y = value->y;
        this->z = value->z;
    }

    float Module() {
        return sqrt(x * x + y * y + z * z);
    }

    void Normalize() {
        float m = Module();
        if (!m) {
            x /= m;
            y /= m;
            z /= m;
        }
    }

    void Sum(xVector3 * value) {
        x += value->x;
        y += value->y;
        z += value->z;
    }

    void Diff(xVector3 * value) {
        x -= value->x;
        y -= value->y;
        z -= value->z;
    }

    void MultScalar(float value) {
        x *= value;
        y *= value;
        z *= value;
    }
};

struct xVector4
{
public:
    float x, y, z, w;

    // ----------------------------------------------------------------------
    // Creates the vector from x y z w components
    // ----------------------------------------------------------------------
    xVector4(float x = 0.0, float y = 0.0, float z = 0.0, float w = 0.0) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    void Set(float x, float y, float z, float w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    void Set(xVector4 * value) {
        this->x = value->x;
        this->y = value->y;
        this->z = value->z;
        this->w = value->w;
    }

    float Module() {
        return sqrt(x * x + y * y + z * z + w * w);
    }

    void Normalize() {
        float m = Module();
        if (!m) {
            x /= m;
            y /= m;
            z /= m;
            w /= m;
        }
    }

    void Sum(xVector4 * value) {
        x += value->x;
        y += value->y;
        z += value->z;
        w += value->w;
    }

    void Diff(xVector4 * value) {
        x -= value->x;
        y -= value->y;
        z -= value->z;
        w -= value->w;
    }

    void MultScalar(float value) {
        x *= value;
        y *= value;
        z *= value;
        w *= value;
    }
};

// ----------------------------------------------------------------------
// Rendering structures
// ----------------------------------------------------------------------

struct xPoint2
{
public:
    float x, y;

    xPoint2(float x, float y) {
        this->x = x;
        this->y = y;
    }
};

struct xPoint3
{
public:
    float x, y, z;

    xPoint3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

struct xEdge
{
public:
    long v1, v2;
};

// ----------------------------------------------------------------------
// Face structure with ids of vertexes, texture coordinates, normals
// (It should be used with objects and theirs own arrays of
// xVertex3 and xVertex2 for data)
// ----------------------------------------------------------------------
struct xFace
{
public:
    long vertex[3];
    long texture[3];
    long normal[3];
};

// ----------------------------------------------------------------------
// Comfortable converting properties structures
// ----------------------------------------------------------------------

struct xArray2
{
    float values[2];

    xArray2(float x = 0.0, float y = 0.0) {
        values[0] = x;
        values[1] = y;
    }

    xArray2(xVector2 * vec) {
        values[0] = vec->x;
        values[1] = vec->y;
    }

    void VectorToArray(xVector2 * vec) {
        values[0] = vec->x;
        values[1] = vec->y;
    }
};

struct xArray3
{
    float values[3];

    xArray3(float x = 0.0, float y = 0.0, float z = 0.0) {
        values[0] = x;
        values[1] = y;
        values[2] = z;
    }

    xArray3(xVector3 * vec) {
        values[0] = vec->x;
        values[1] = vec->y;
        values[2] = vec->z;
    }

    void VectorToArray(xVector3 * vec) {
        values[0] = vec->x;
        values[1] = vec->y;
        values[2] = vec->z;
    }
};

struct xArray4
{
    float values[4];

    xArray4(float x = 0.0, float y = 0.0, float z = 0.0, float w = 0.0) {
        values[0] = x;
        values[1] = y;
        values[2] = z;
        values[3] = w;
    }

    xArray4(xVector4 * vec) {
        values[0] = vec->x;
        values[1] = vec->y;
        values[2] = vec->z;
        values[3] = vec->w;
    }

    void VectorToArray(xVector4 * vec) {
        values[0] = vec->x;
        values[1] = vec->y;
        values[2] = vec->z;
        values[3] = vec->w;
    }
};

#endif //OXYGEN_XBASEGEOMETRY_H
