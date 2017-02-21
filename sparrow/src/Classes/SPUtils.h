//
//  SPUtils.h
//  Sparrow
//
//  Created by Daniel Sperl on 04.01.11.
//  Copyright 2011-2015 Gamua. All rights reserved.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the Simplified BSD License.
//

#import <Sparrow/SparrowBase.h>

NS_ASSUME_NONNULL_BEGIN

/// The SPUtils class contains utility methods for different purposes.

@interface SPUtils : NSObject 

/// ----------------
/// @name Math Utils
/// ----------------

/// Finds the next power of two equal to or above the specified number.
+ (NSInteger)nextPowerOfTwo:(NSInteger)number;

/// Checks if a number is a power of two.
+ (BOOL)isPowerOfTwo:(NSInteger)number;

/// Returns a random int between `minValue` (inclusive) and `maxValue` (exclusive).
+ (int)randomIntBetweenMin:(int)minValue andMax:(int)maxValue;

/// Returns a random NSInteger between `minValue` (inclusive) and `maxValue` (exclusive).
+ (NSInteger)randomIntegerBetweenMin:(NSInteger)minValue andMax:(NSInteger)maxValue;

/// Returns a random float number between `minValue` (inclusive) and `maxValue` (exclusive).
+ (float)randomFloatBetweenMin:(float)minValue andMax:(float)maxValue;

/// Returns a random float number between 0.0 and 1.0
+ (float)randomFloat;

/// ----------------
/// @name File Utils
/// ----------------

/// Returns the bundle used for file utility methods. Default is '[NSBundle mainBundle]'
+ (NSBundle *)defaultBundle;

/// Change the default bundle for use with file utility methods. For example changing the bundle
/// to a unit testing bundle.
+ (void)setDefaultBundle:(NSBundle *)bundle;

/// Returns a Boolean value that indicates whether a file or directory exists at a specified path.
/// If you pass a relative path, the resource folder of the application bundle will be searched.
+ (BOOL)fileExistsAtPath:(NSString *)path;

#if TARGET_OS_IPHONE
/// Finds the full path for a file, favoring those with the given scale factor and
/// device idiom. Relative paths are searched in the application bundle. If no suitable file can
/// be found, the method returns nil. Use the bundle parameter to specify a specific bundle.
+ (nullable NSString *)absolutePathToFile:(NSString *)path withScaleFactor:(float)factor
                                    idiom:(UIUserInterfaceIdiom)idiom bundle:(NSBundle *)bundle;

/// Finds the full path for a file, favoring those with the given scale factor and
/// device idiom. Relative paths are searched in the application bundle. If no suitable file can
/// be found, the method returns nil. Will use the 'defaultBundle'.
+ (nullable NSString *)absolutePathToFile:(NSString *)path withScaleFactor:(float)factor
                                    idiom:(UIUserInterfaceIdiom)idiom;
#endif

/// Finds the full path for a file, favoring those with the given scale factor and the current
/// device idiom. Relative paths are searched in the application bundle. If no suitable file can
/// be found, the method returns nil. Will use the 'defaultBundle'.
+ (nullable NSString *)absolutePathToFile:(NSString *)path withScaleFactor:(float)factor;

/// Finds the full path for a file, favoring those with the current content scale factor and
/// device idiom. Relative paths are searched in the application bundle. If no suitable file can
/// be found, the method returns nil. Will use the 'defaultBundle'.
+ (nullable NSString *)absolutePathToFile:(NSString *)path;

@end

NS_ASSUME_NONNULL_END
