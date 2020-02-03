#ifndef Vector4f_H
#define Vector4f_H

#include <cmath>

// Vector4f performs vector operations with 4-dimensions
// The purpose of this class is primarily for 3D graphics
// applications.
struct Vector4f{
    // Note: x,y,z,w are a convention
    // x,y,z,w could be position, but also any 4-component value.
    float x,y,z,w;

    // Default conostrutcor
    // 'why default?' https://stackoverflow.com/questions/20828907/the-new-keyword-default-in-c11
    Vector4f() = default;

    // The "Real" constructor we want to use.
    // This initializes the values x,y,z (my note: I assume this also means w)
    Vector4f(float a, float b, float c, float d){
        x = a;
        y = b;
        z = c;
        w = d;
    }

    // Copy constructor for a Vector4f
    Vector4f(const Vector4f& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        w = v.w;
    }

    // Index operator, allowing us to access the individual
    // x,y,z,w components of our vector.
    float& operator[](int i){
        // TODO: Discuss with your partner why this works.
        //       There is no code to change here.
      return ((&x)[i]);
    }

    // Index operator, allowing us to access the individual
    // x,y,z,w components of our vector.
    const float& operator[](int i) const{
        // TODO: Discuss with your partner why this works.
        //       There is no code to change here.
        return ((&x)[i]);
    }

    // Multiplication Operator
    // Multiply vector by a uniform-scalar.
    Vector4f& operator *=(float s){
        this->x *= s;
        this->y *= s;
        this->z *= s;
        this->w *= s;
        return (*this);
    }

    // Division Operator
    Vector4f& operator /=(float s){
        this->x /= s;
        this->y /= s;
        this->z /= s;
        this->w /= s;
        return (*this);
    }

    // Addition operator
    Vector4f& operator +=(const Vector4f& v){
        this->x += v.x;
        this->y += v.y;
        this->z += v.z;
        this->w += v.w;
      return (*this);
    }

    // Subtraction operator
    Vector4f& operator -=(const Vector4f& v){
        this->x -= v.x;
        this->y -= v.y;
        this->z -= v.z;
        this->w -= v.w;
      return (*this);
    }

};

// Compute the dot product of a Vector4f
inline float Dot(const Vector4f& a, const Vector4f& b){
    float result = (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
    return result;
}

// Multiplication of a vector by a scalar values
inline Vector4f operator *(const Vector4f& v, float s){
    float new_x = v.x * s;
    float new_y = v.y * s;
    float new_z = v.z * s;
    float new_w = v.w * s;

    Vector4f vec(new_x, new_y, new_z, new_w);
    return vec;
}

// Division of a vector by a scalar value.
inline Vector4f operator /(const Vector4f& v, float s){
    // TODO:
    float new_x = v.x / s;
    float new_y = v.y / s;
    float new_z = v.z / s;
    float new_w = v.w / s;

    Vector4f vec(new_x, new_y, new_z, new_w);
    return vec;
}

// Negation of a vector
// Use Case: Sometimes it is handy to apply a force in an opposite direction
inline Vector4f operator -(const Vector4f& v){
    Vector4f vec(-v.x, -v.y, -v.z, -v.w);
    return vec;
}

// Return the magnitude of a vector
inline float Magnitude(const Vector4f& v){
    float result = sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2) + pow(v.w, 2));
    return result;
}

// Add two vectors together
inline Vector4f operator +(const Vector4f& a, const Vector4f& b){
    float new_x = a.x + b.x;
    float new_y = a.y + b.y;
    float new_z = a.z + b.z;
    float new_w = a.w + b.w;

    Vector4f vec(new_x, new_y, new_z, new_w);
    return vec;
}

// Subtract two vectors
inline Vector4f operator -(const Vector4f& a, const Vector4f& b){
    float new_x = a.x - b.x;
    float new_y = a.y - b.y;
    float new_z = a.z - b.z;
    float new_w = a.w - b.w;

    Vector4f vec(new_x, new_y, new_z, new_w);
    return vec;
}

// Vector Projection
// Note: This is the vector projection of 'a' onto 'b'
inline Vector4f Project(const Vector4f& a, const Vector4f& b){
    float mag = Magnitude(b);
    float projection = Dot(a, b);
    Vector4f vec = b * (projection / pow(mag, 2));
    return vec;
}

// Set a vectors magnitude to 1
// Note: This is NOT generating a normal vector
inline Vector4f Normalize(const Vector4f& v){
    // To normalize the vector, you should just have to divide it by its magnitude.
    float mag = Magnitude(v);
    Vector4f vec(v);
    vec /= mag;
    return vec;
}

// a x b (read: 'a crossed b')
// Produces a new vector perpendicular to a and b.
// (So long as a and b are not parallel which returns zero vector)
// Note: For a Vector4f, we can only compute a cross porduct to 
//       to vectors in 3-dimensions. Simply ignore w, and set to (0,0,0,1)
//       for this vector.
inline Vector4f CrossProduct(const Vector4f& a, const Vector4f& b){
    // TODO:
    // Following the calculation using the determinant of the i, j, k matrix.
    float new_x = a.y * b.z - a.z * b.y;
    float new_y = -(a.x * b.z - a.z * b.x);
    float new_z = a.x * b.y - b.x * a.y;
    float w = 1.0f;
    Vector4f vec(new_x, new_y, new_z, w);
    return vec;
}


#endif
