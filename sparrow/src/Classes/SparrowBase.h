//
//  SparrowBase.h
//  Sparrow
//
//  Created by Robert Carone on 8/13/15.
//  Copyright 2011-2015 Gamua. All rights reserved.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the Simplified BSD License.
//

#import <Availability.h>
#import <TargetConditionals.h>

#import <CoreGraphics/CGGeometry.h>
#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>
#import <GLKit/GLKMath.h>

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#else
#import <AppKit/AppKit.h>
#endif

// defines

#ifndef SP_DEPRECATED
    #define SP_DEPRECATED __attribute__((deprecated))
#endif

#ifndef SP_INLINE
    #define SP_INLINE static __inline__
#endif

#ifndef SP_EXTERN
    #ifdef __cplusplus
        #define SP_EXTERN extern "C" __attribute__((visibility ("default")))
    #else
        #define SP_EXTERN extern __attribute__((visibility ("default")))
    #endif
#endif

// from https://gist.github.com/smileyborg/d513754bc1cf41678054#file-xcode7macros-h-L6

#if !__has_feature(nullability)
    #define NS_ASSUME_NONNULL_BEGIN
    #define NS_ASSUME_NONNULL_END
    #define nullable
    #define __nullable
#endif

#if __has_feature(objc_generics)
    #define SP_GENERIC(class, ...)      class<__VA_ARGS__>
    #define SP_GENERIC_TYPE(type)       type
#else
    #define SP_GENERIC(class, ...)      class
    #define SP_GENERIC_TYPE(type)       id
#endif


// enums

enum { SPNotFound = -1 };

/// Horizontal alignment.
typedef NS_ENUM(NSInteger, SPHAlign)
{
    SPHAlignLeft,
    SPHAlignCenter,
    SPHAlignRight
};

/// Vertical alignment.
typedef NS_ENUM(NSInteger, SPVAlign)
{
    SPVAlignTop,
    SPVAlignCenter,
    SPVAlignBottom
};

// exceptions

SP_EXTERN NSString *const SPExceptionAbstractClass;
SP_EXTERN NSString *const SPExceptionAbstractMethod;
SP_EXTERN NSString *const SPExceptionNotRelated;
SP_EXTERN NSString *const SPExceptionIndexOutOfBounds;
SP_EXTERN NSString *const SPExceptionInvalidOperation;
SP_EXTERN NSString *const SPExceptionFileNotFound;
SP_EXTERN NSString *const SPExceptionFileInvalid;
SP_EXTERN NSString *const SPExceptionDataInvalid;
SP_EXTERN NSString *const SPExceptionOperationFailed;
