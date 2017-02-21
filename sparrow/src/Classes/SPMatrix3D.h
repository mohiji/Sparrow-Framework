//
//  SPMatrix3D.h
//  Sparrow
//
//  Created by Robert Carone on 7/31/15.
//  Copyright 2011-2015 Gamua. All rights reserved.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the Simplified BSD License.
//

#import <Sparrow/SparrowBase.h>
#import <Sparrow/SPPoolObject.h>
#import <simd/simd.h>

NS_ASSUME_NONNULL_BEGIN

@class SPMatrix;
@class SPPoint3D;

/** ------------------------------------------------------------------------------------------------
 
 The SPMatrix3D class represents a transformation matrix that determines the position and orientation
 of a three-dimensional (3D) display object. The matrix can perform transformation functions 
 including translation (repositioning along the x, y, and z axes), rotation, and scaling (resizing).
 
------------------------------------------------------------------------------------------------- */

@interface SPMatrix3D : SPPoolObject <NSCopying>
{
  @protected
    matrix_float4x4 _m;
}

/// --------------------
/// @name Initialization
/// --------------------

/// Initializes a matrix with a simd float4x4 struct. _Designated Initializer_.
- (instancetype)initWithMatrix4x4:(matrix_float4x4)matrix;

/// Initializes a matrix with a GLKMatrix4 struct.
- (instancetype)initWithGLKMatrix4:(GLKMatrix4)matrix;

/// Initializes a matrix with the specified float array.
- (instancetype)initWithValues:(float [16])values;

/// Initializes an identity matrix.
- (instancetype)init;

/// Factory method.
+ (instancetype)matrix3DWithIdentity;

/// Factory method.
+ (instancetype)matrix3DWithMatrix4x4:(matrix_float4x4)matrix;

/// Factory method.
+ (instancetype)matrix3DWithRotation:(float)rotation x:(float)x y:(float)y z:(float)z;

/// Factory method.
+ (instancetype)matrix3DWithRotationX:(float)angle;

/// Factory method.
+ (instancetype)matrix3DWithRotationY:(float)angle;

/// Factory method.
+ (instancetype)matrix3DWithRotationZ:(float)angle;

/// Factory method.
+ (instancetype)matrix3DWithScaleX:(float)sx y:(float)sy z:(float)sz;

/// Factory method.
+ (instancetype)matrix3DWithTranslationX:(float)tx y:(float)ty z:(float)tz;

/// -------------
/// @name Methods
/// -------------

/// Appends the matrix by multiplying another Matrix3D object by the current Matrix3D object.
- (void)appendMatrix:(SPMatrix3D *)lhs;

/// Appends an incremental rotation to a Matrix3D object.
- (void)appendRotation:(float)angle axis:(SPPoint3D *)axis;

/// Appends an incremental scale change along the x, y, and z axes to a Matrix3D object.
- (void)appendScaleX:(float)sx y:(float)sy z:(float)sz;

/// Appends an incremental translation, a repositioning along the x, y, and z axes, to a Matrix3D object.
- (void)appendTranslationX:(float)tx y:(float)ty z:(float)tz;

/// Prepends a matrix by multiplying the current Matrix3D object by another Matrix3D object.
- (void)prependMatrix:(SPMatrix3D *)rhs;

/// Prepends an incremental rotation to a Matrix3D object.
- (void)prependRotation:(float)angle axis:(SPPoint3D *)axis;

/// Prepends an incremental scale change along the x, y, and z axes to a Matrix3D object.
- (void)prependScaleX:(float)sx y:(float)sy z:(float)sz;

/// Prepends an incremental translation, a repositioning along the x, y, and z axes, to a Matrix3D object.
- (void)prependTranslationX:(float)tx y:(float)ty z:(float)tz;

/// Converts the current matrix to an identity or unit matrix.
- (void)identity;

/// Inverts the current matrix.
- (BOOL)invert;

/// Rotates the display object so that it faces a specified position.
- (void)pointAt:(SPPoint3D *)pos at:(SPPoint3D *)at up:(SPPoint3D *)up;

/// Converts the current Matrix3D object to a matrix where the rows and columns are swapped.
- (void)transpose;

/// Copies all of the matrix data from the source Matrix3D object into the calling Matrix3D object.
- (void)copyFromMatrix:(SPMatrix3D *)matrix;

/// Compares two matrices.
- (BOOL)isEqualToMatrix:(SPMatrix3D *)matrix;

/// Converts a 3D matrix to a 2D matrix. Beware that this will work only for a 3D matrix
/// describing a pure 2D transformation.
- (SPMatrix *)convertTo2D;

/// Returns a GLKit matrix that is equivalent to this instance.
- (GLKMatrix4)convertToGLKMatrix;

/// Returns a SIMD matrix that is equivalent to this instance.
- (matrix_float4x4)convertToMatrix4x4;

/// Uses the transformation matrix to transform a Point3D object from one space coordinate to another.
- (SPPoint3D *)transformPoint3D:(SPPoint3D *)vector;

/// Uses the transformation matrix to transform a coordinate from one space coordinate to another.
- (SPPoint3D *)transformPoint3DWithX:(float)x y:(float)y z:(float)z;

/// ----------------
/// @name Properties
/// ----------------

/// An array of 16 floats, where every four elements is a column of a 4x4 matrix.
@property (nonatomic, assign) float *rawData;

/// A Number that determines whether a matrix is invertible.
@property (nonatomic, readonly) float determinant;

@end


/** Sparrow compatibility replacements for simd. */

#if __IPHONE_OS_VERSION_MIN_ALLOWED < 71000
extern const matrix_float4x4 matrix_identity_float4x4;
#endif

#if TARGETOS_IPHONE && __IPHONE_OS_VERSION_MAX_ALLOWED < 90000
static _Bool __SIMD_ATTRIBUTES__ matrix_almost_equal_elements(matrix_float4x4 __x, matrix_float4x4 __y, float __tol)
{
    return vector_all((__tg_fabs(__x.columns[0] - __y.columns[0]) <= __tol) &
                      (__tg_fabs(__x.columns[1] - __y.columns[1]) <= __tol) &
                      (__tg_fabs(__x.columns[2] - __y.columns[2]) <= __tol) &
                      (__tg_fabs(__x.columns[3] - __y.columns[3]) <= __tol));
}
#endif

NS_ASSUME_NONNULL_END
