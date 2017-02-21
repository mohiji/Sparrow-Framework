//
//  SPUtils.m
//  Sparrow
//
//  Created by Daniel Sperl on 04.01.11.
//  Copyright 2011-2015 Gamua. All rights reserved.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the Simplified BSD License.
//

#import "SparrowClass.h"
#import "SPMacros.h"
#import "SPNSExtensions.h"
#import "SPUtils.h"

#import <sys/stat.h>

static NSBundle *defaultBundle = nil;

@implementation SPUtils

+ (void)initialize
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^
    {
        defaultBundle = [NSBundle mainBundle];
    });
}

- (instancetype)init
{
    SP_STATIC_CLASS_INITIALIZER();
    return nil;
}

#pragma mark Math Utils

+ (NSInteger)nextPowerOfTwo:(NSInteger)number
{    
    int result = 1; 
    while (result < number) result *= 2;
    return result;    
}

+ (BOOL)isPowerOfTwo:(NSInteger)number
{
    return ((number != 0) && !(number & (number - 1)));
}

+ (int)randomIntBetweenMin:(int)minValue andMax:(int)maxValue
{
    return (int)(minValue + SPRandomFloat() * (maxValue - minValue));
}

+ (NSInteger)randomIntegerBetweenMin:(NSInteger)minValue andMax:(NSInteger)maxValue
{
    return (NSInteger)(minValue + SPRandomFloat() * (maxValue - minValue));
}

+ (float)randomFloatBetweenMin:(float)minValue andMax:(float)maxValue
{
    return (float)(minValue + SPRandomFloat() * (maxValue - minValue));
}

+ (float)randomFloat
{
    return SPRandomFloat();
}

#pragma mark File Utils

+ (NSBundle *)defaultBundle
{
    return defaultBundle;
}

+ (void)setDefaultBundle:(NSBundle *)bundle
{
    defaultBundle = bundle;
}

+ (BOOL)fileExistsAtPath:(NSString *)path
{
    if (!path)
        return NO;
    else if (!path.isAbsolutePath)
        path = [defaultBundle pathForResource:path];
    
    struct stat buffer;   
    return stat([path UTF8String], &buffer) == 0;
}

#if TARGET_OS_IPHONE
+ (nullable NSString *)absolutePathToFile:(NSString *)path withScaleFactor:(float)factor
                                    idiom:(UIUserInterfaceIdiom)idiom bundle:(NSBundle *)bundle
{
    // iOS image resource naming conventions:
    // SD: <ImageName><device_modifier>.<filename_extension>
    // HD: <ImageName>@2x<device_modifier>.<filename_extension>
    
    if (factor < 1.0f) factor = 1.0f;
    
    NSString *originalPath = path;
    NSString *pathWithScale = [path stringByAppendingScaleSuffixToFilename:factor];
    NSString *idiomSuffix = (idiom == UIUserInterfaceIdiomPad) ? @"~ipad" : @"~iphone";
    NSString *pathWithIdiom = [pathWithScale stringByAppendingSuffixToFilename:idiomSuffix];
    
    BOOL isAbsolute = [path isAbsolutePath];
    NSString *absolutePath = isAbsolute ? pathWithScale : [bundle pathForResource:pathWithScale];
    NSString *absolutePathWithIdiom = isAbsolute ? pathWithIdiom : [bundle pathForResource:pathWithIdiom];
    
    if ([SPUtils fileExistsAtPath:absolutePathWithIdiom])
        return absolutePathWithIdiom;
    else if ([SPUtils fileExistsAtPath:absolutePath])
        return absolutePath;
    else if (factor >= 2.0f)
        return [SPUtils absolutePathToFile:originalPath withScaleFactor:factor-1.0f idiom:idiom];
    else
        return nil;
};

+ (NSString *)absolutePathToFile:(NSString *)path withScaleFactor:(float)factor
                           idiom:(UIUserInterfaceIdiom)idiom
{
    return [self absolutePathToFile:path withScaleFactor:factor idiom:idiom bundle:defaultBundle];
}

+ (NSString *)absolutePathToFile:(NSString *)path withScaleFactor:(float)factor
{
    UIUserInterfaceIdiom currentIdiom = [[UIDevice currentDevice] userInterfaceIdiom];
    return [SPUtils absolutePathToFile:path withScaleFactor:factor idiom:currentIdiom];
}

+ (NSString *)absolutePathToFile:(NSString *)path
{
    return [SPUtils absolutePathToFile:path withScaleFactor:Sparrow.contentScaleFactor];
}
#endif

@end
