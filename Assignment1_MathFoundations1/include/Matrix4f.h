// High level design note
// Our matrix should match the behavior of the glm library.
#ifndef MATRIX4F_H
#define MATRIX4F_H

#include <cmath>
#include <iostream>

// We need to Vector4f header in order to multiply a matrix
// by a vector.
#include "Vector4f.h"

// Matrix 4f represents 4x4 matrices in Math
struct Matrix4f{
private:
    float n[4][4];  // Store each value of the matrix

public:
    Matrix4f() = default;

    // Column major order!
    // Matrix constructor with 9 scalar values.
    Matrix4f( float n00, float n01, float n02, float n03,
              float n10, float n11, float n12, float n13,
              float n20, float n21, float n22, float n23,
              float n30, float n31, float n32, float n33){

        n[0][0] = n00; n[0][1] = n10; n[0][2] = n20; n[0][3] = n30;
        n[1][0] = n01; n[1][1] = n11; n[1][2] = n21; n[1][3] = n31;
        n[2][0] = n02; n[2][1] = n12; n[2][2] = n22; n[2][3] = n32;
        n[3][0] = n03; n[3][1] = n13; n[3][2] = n23; n[3][3] = n33;
    }

    // Matrix constructor from four vectors.
    // Note: 'd' will almost always be 0,0,0,1
    Matrix4f(const Vector4f& a, const Vector4f& b, const Vector4f& c, const Vector4f& d){
      n[0][0] = a.x; n[0][1] = b.x; n[0][2] = c.x; n[0][3] = d.x;
      n[1][0] = a.y; n[1][1] = b.y; n[1][2] = c.y; n[1][3] = d.y;
      n[2][0] = a.z; n[2][1] = b.z; n[2][2] = c.z; n[2][3] = d.z;
      n[3][0] = a.w; n[3][1] = b.w; n[3][2] = c.w; n[3][3] = d.w;
    }

    // Makes the matrix an identity matrix
    void identity(){
        n[0][0] = 1; n[0][1] = 0; n[0][2] = 0; n[0][3] = 0;
        n[1][0] = 0; n[1][1] = 1; n[1][2] = 0; n[1][3] = 0;
        n[2][0] = 0; n[2][1] = 0; n[2][2] = 1; n[2][3] = 0;
        n[3][0] = 0; n[3][1] = 0; n[3][2] = 0; n[3][3] = 1;
    }

    // Index operator with two dimensions
    // Example: M(1,1) returns row 1 and column 1 of matrix M.
    // i is the row, j is the column.
    float& operator ()(int i, int j){
      return (n[i][j]);
    }

    // Index operator with two dimensions
    // Example: M(1,1) returns row 1 and column 1 of matrix M.
    // i is the row, j is the column.
    const float& operator ()(int i, int j) const{
      return (n[i][j]);
    }

    // Return a single  vector from the matrix (row or columnn major? hmm).
    Vector4f& operator [](int j){
      return (*reinterpret_cast<Vector4f *>(n[j]));
    }

    // Return a single  vector from the matrix (row or columnn major? hmm).
    const Vector4f& operator [](int j) const{
      return (*reinterpret_cast<const Vector4f *>(n[j]));
    }

    // Make a matrix rotate about various axis
    Matrix4f MakeRotationX(float t){
        // TODO:
        Matrix4f x_rotation = Matrix4f(1,0,0,0,
                                       0,cos(t),sin(t),0,
                                       0,-sin(t),cos(t),0,
                                       0,0,0,1);
        
        Matrix4f result = *this * x_rotation;
        return result; // You will need to modify this.
                            // When you test, test against glm_gtx_transform
    }
    Matrix4f MakeRotationY(float t){
        // TODO:
        Matrix4f y_rotation = 
            Matrix4f(
                cos(t), 0, -sin(t), 0,
                0, 1, 0, 0,
                sin(t), 0, cos(t), 0,
                0, 0, 0, 1);
        
        Matrix4f result = *this * y_rotation;
        return result; // You will need to modify this.
                            // When you test, test against glm_gtx_transform
    }
    Matrix4f MakeRotationZ(float t){
        // TODO:
        Matrix4f z_rotation = 
            Matrix4f(
                cos(t), sin(t), 0, 0,
                -sin(t), cos(t), 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1);
        Matrix4f result = *this * z_rotation;
        return result; // You will need to modify this.
                            // When you test, test against glm_gtx_transform
    }

    // Multiplies the first row (00, 01, 02, 03) by x, second by y, third by z.
    Matrix4f MakeScale(float sx,float sy, float sz){
        // TODO:
        n[0][0] *= sx; n[0][1] *= sx; n[0][2] *= sx; 
        n[1][0] *= sy; n[1][1] *= sy; n[1][2] *= sy; 
        n[2][0] *= sz; n[2][1] *= sz; n[2][2] *= sz;

        return(*this); // You will need to modify this.
                            // When you test, test against glm_gtx_transform
    }

    void print_me()
    {
        std::cout << "Matrix:" << std::endl;
        std::cout << n[0][0] << " " << n[0][1] << " " << n[0][2] << " " << n[0][3] << std::endl;
        std::cout << n[1][0] << " " << n[1][1] << " " << n[1][2] << " " << n[1][3] << std::endl;
        std::cout << n[2][0] << " " << n[2][1] << " " << n[2][2] << " " << n[2][3] << std::endl;
        std::cout << n[3][0] << " " << n[3][1] << " " << n[3][2] << " " << n[3][3] << std::endl;
    }

    friend Matrix4f operator*(const Matrix4f& left, const Matrix4f& right);
};

// Matrix Multiplication
Matrix4f operator*(const Matrix4f& A, const Matrix4f& B){
    float new_v1_x = (A(0, 0) * B(0, 0)) + (A(0, 1) * B(1, 0)) + (A(0, 2) * B(2, 0)) + (A(0, 3) * B(3, 0));
    float new_v1_y = (A(0, 0) * B(0, 1)) + (A(0, 1) * B(1, 1)) + (A(0, 2) * B(2, 1)) + (A(0, 3) * B(3, 1));
    float new_v1_z = (A(0, 0) * B(0, 2)) + (A(0, 1) * B(1, 2)) + (A(0, 2) * B(2, 2)) + (A(0, 3) * B(3, 2));
    float new_v1_w = (A(0, 0) * B(0, 3)) + (A(0, 1) * B(1, 3)) + (A(0, 2) * B(2, 3)) + (A(0, 3) * B(3, 3));

    float new_v2_x = (A(1, 0) * B(0, 0)) + (A(1, 1) * B(1, 0)) + (A(1, 2) * B(2, 0)) + (A(1, 3) * B(3, 0));
    float new_v2_y = (A(1, 0) * B(0, 1)) + (A(1, 1) * B(1, 1)) + (A(1, 2) * B(2, 1)) + (A(1, 3) * B(3, 1));
    float new_v2_z = (A(1, 0) * B(0, 2)) + (A(1, 1) * B(1, 2)) + (A(1, 2) * B(2, 2)) + (A(1, 3) * B(3, 2));
    float new_v2_w = (A(1, 0) * B(0, 3)) + (A(1, 1) * B(1, 3)) + (A(1, 2) * B(2, 3)) + (A(1, 3) * B(3, 3));

    float new_v3_x = (A(2, 0) * B(0, 0)) + (A(2, 1) * B(1, 0)) + (A(2, 2) * B(2, 0)) + (A(2, 3) * B(3, 0));
    float new_v3_y = (A(2, 0) * B(0, 1)) + (A(2, 1) * B(1, 1)) + (A(2, 2) * B(2, 1)) + (A(2, 3) * B(3, 1));
    float new_v3_z = (A(2, 0) * B(0, 2)) + (A(2, 1) * B(1, 2)) + (A(2, 2) * B(2, 2)) + (A(2, 3) * B(3, 2));
    float new_v3_w = (A(2, 0) * B(0, 3)) + (A(2, 1) * B(1, 3)) + (A(2, 2) * B(2, 3)) + (A(2, 3) * B(3, 3));

    float new_v4_x = (A(3, 0) * B(0, 0)) + (A(3, 1) * B(1, 0)) + (A(3, 2) * B(2, 0)) + (A(3, 3) * B(3, 0));
    float new_v4_y = (A(3, 0) * B(0, 1)) + (A(3, 1) * B(1, 1)) + (A(3, 2) * B(2, 1)) + (A(3, 3) * B(3, 1));
    float new_v4_z = (A(3, 0) * B(0, 2)) + (A(3, 1) * B(1, 2)) + (A(3, 2) * B(2, 2)) + (A(3, 3) * B(3, 2));
    float new_v4_w = (A(3, 0) * B(0, 3)) + (A(3, 1) * B(1, 3)) + (A(3, 2) * B(2, 3)) + (A(3, 3) * B(3, 3));

    Matrix4f mat4(
        new_v1_x, new_v1_y, new_v1_z, new_v1_w,
        new_v2_x, new_v2_y, new_v2_z, new_v2_w,
        new_v3_x, new_v3_y, new_v3_z, new_v3_w,
        new_v4_x, new_v4_y, new_v4_z, new_v4_w);

    return mat4;
}

// Matrix multiply by a vector

Vector4f operator *(const Matrix4f& M, const Vector4f& v){
    float new_x = (M(0, 0) * v.x) + (M(0, 1) * v.y) + (M(0, 2) * v.z) + (M(0, 3) * v.w);
    float new_y = (M(1, 0) * v.x) + (M(1, 1) * v.y) + (M(1, 2) * v.z) + (M(1, 3) * v.w);
    float new_z = (M(2, 0) * v.x) + (M(2, 1) * v.y) + (M(2, 2) * v.z) + (M(2, 3) * v.w);
    float new_w = (M(3, 0) * v.x) + (M(3, 1) * v.y) + (M(3, 2) * v.z) + (M(3, 3) * v.w);

    Vector4f vec(new_x, new_y, new_z, new_w);

    return vec;
}


#endif
