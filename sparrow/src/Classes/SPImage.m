//
//  SPImage.m
//  Sparrow
//
//  Created by Daniel Sperl on 19.06.09.
//  Copyright 2011-2015 Gamua. All rights reserved.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the Simplified BSD License.
//

#import "SparrowClass.h"
#import "SPGLTexture.h"
#import "SPImage.h"
#import "SPMacros.h"
#import "SPPoint.h"
#import "SPRectangle.h"
#import "SPRenderSupport.h"
#import "SPSubTexture.h"
#import "SPTexture.h"
#import "SPVertexData.h"

@implementation SPImage
{
    SPVertexData *_vertexDataCache;
    BOOL _vertexDataCacheInvalid;
}

@synthesize texture = _texture;

#pragma mark Initialization

- (instancetype)initWithTexture:(SPTexture *)texture
{
    if (!texture) [NSException raise:SPExceptionInvalidOperation format:@"texture cannot be nil!"];
    
    SPRectangle *frame = texture.frame;    
    float width  = frame ? frame.width  : texture.width;
    float height = frame ? frame.height : texture.height;
    BOOL pma = texture.premultipliedAlpha;
    
    if ((self = [super initWithWidth:width height:height color:SPColorWhite premultipliedAlpha:pma]))
    {
        _vertexData.vertices[1].texCoords.x = 1.0f;
        _vertexData.vertices[2].texCoords.y = 1.0f;
        _vertexData.vertices[3].texCoords.x = 1.0f;
        _vertexData.vertices[3].texCoords.y = 1.0f;
        
        _texture = [texture retain];
        _vertexDataCache = [[SPVertexData alloc] initWithSize:4 premultipliedAlpha:pma];
        _vertexDataCacheInvalid = YES;
    }
    return self;
}

- (instancetype)initWithContentsOfFile:(NSString *)path generateMipmaps:(BOOL)mipmaps
{
    return [self initWithTexture:[SPTexture textureWithContentsOfFile:path generateMipmaps:mipmaps]];
}

- (instancetype)initWithContentsOfFile:(NSString *)path
{
    return [self initWithContentsOfFile:path generateMipmaps:NO];
}

- (instancetype)initWithWidth:(float)width height:(float)height
{
    return [self initWithTexture:[SPTexture textureWithWidth:width height:height draw:NULL]];
}

- (instancetype)init
{
    return [self initWithTexture:[SPTexture emptyTexture]];
}

- (void)dealloc
{
    [_texture release];
    [_vertexDataCache release];
    [super dealloc];
}

+ (instancetype)imageWithTexture:(SPTexture *)texture
{
    return [[[self alloc] initWithTexture:texture] autorelease];
}

+ (instancetype)imageWithContentsOfFile:(NSString *)path
{
    return [[[self alloc] initWithContentsOfFile:path] autorelease];
}

#pragma mark Methods

- (void)setTexCoords:(SPPoint *)coords ofVertex:(NSInteger)vertexID
{
    [_vertexData setTexCoords:coords atIndex:vertexID];
    [self vertexDataDidChange];
}

- (void)setTexCoordsWithX:(float)x y:(float)y ofVertex:(NSInteger)vertexID
{
    [_vertexData setTexCoordsWithX:x y:y atIndex:vertexID];
    [self vertexDataDidChange];
}

- (SPPoint *)texCoordsOfVertex:(NSInteger)vertexID
{
    return [_vertexData texCoordsAtIndex:vertexID];
}

- (void)readjustSize
{
    SPRectangle *frame = _texture.frame;    
    float width  = frame ? frame.width  : _texture.width;
    float height = frame ? frame.height : _texture.height;

    _vertexData.vertices[1].position.x = width;
    _vertexData.vertices[2].position.y = height;
    _vertexData.vertices[3].position.x = width;
    _vertexData.vertices[3].position.y = height;
    
    [self vertexDataDidChange];
}

#pragma mark NSCopying

- (instancetype)copyWithZone:(NSZone *)zone
{
    SPImage *image = [super copyWithZone:zone];
    
    image.texture = self.texture;
    [image readjustSize];
    
    return image;
}

#pragma mark SPQuad

- (void)vertexDataDidChange
{
    _vertexDataCacheInvalid = YES;
}

- (void)copyTransformedVertexDataTo:(SPVertexData *)targetData atIndex:(NSInteger)targetIndex
                             matrix:(nullable SPMatrix *)matrix
{
    if (_vertexDataCacheInvalid)
    {
        _vertexDataCacheInvalid = NO;
        [_vertexData copyToVertexData:_vertexDataCache];
        [_texture adjustVertexData:_vertexDataCache atIndex:0 numVertices:4];
    }
    
    [_vertexDataCache copyTransformedToVertexData:targetData atIndex:targetIndex matrix:matrix fromIndex:0 numVertices:4];
}

- (void)setTexture:(SPTexture *)value
{
    if (value == nil)
    {
        [NSException raise:SPExceptionInvalidOperation format:@"texture cannot be nil!"];
    }
    else if (value != _texture)
    {
        SP_RELEASE_AND_RETAIN(_texture, value);
        [_vertexData setPremultipliedAlpha:_texture.premultipliedAlpha updateVertices:YES];
        [_vertexDataCache setPremultipliedAlpha:_texture.premultipliedAlpha updateVertices:NO];
        [self vertexDataDidChange];
    }
}

@end
