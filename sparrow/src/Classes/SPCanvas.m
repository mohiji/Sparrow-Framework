//
//  SPCanvas.m
//  Sparrow
//
//  Created by Robert Carone on 8/4/15.
//  Copyright 2011-2015 Gamua. All rights reserved.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the Simplified BSD License.
//

#import "SparrowClass.h"
#import "SPCanvas.h"
#import "SPIndexData.h"
#import "SPMatrix.h"
#import "SPMatrix3D.h"
#import "SPOpenGL.h"
#import "SPPoint.h"
#import "SPPolygon.h"
#import "SPProgram.h"
#import "SPRenderSupport.h"
#import "SPVertexData.h"

#define PROGRAM_NAME @"Shape"

@implementation SPCanvas
{
    BOOL _syncRequired;
    SP_GENERIC(NSMutableArray, SPPolygon*) *_polygons;
    SPProgram *_program;
    
    SPVertexData *_vertexData;
    uint _vertexBufferName;
    SPIndexData *_indexData;
    uint _indexBufferName;
    
    uint _fillColor;
    float _fillAlpha;
}

#pragma mark Initialization

- (instancetype)init
{
    if (self = [super init])
    {
        _polygons = [[NSMutableArray alloc] init];
        _vertexData = [[SPVertexData alloc] init];
        _indexData = [[SPIndexData alloc] init];
        _syncRequired = NO;
        
        _fillColor = SPColorWhite;
        _fillAlpha = 1.0f;
        
        [self registerPrograms];
    }
    return self;
}

- (void)dealloc
{
    [self destroyBuffers];
    
    [_polygons release];
    [_program release];
    [_vertexData release];
    [_indexData release];
    [super dealloc];
}

#pragma mark Methods

- (void)drawCircleWithX:(float)x y:(float)y radius:(float)radius
{
    [self appendPolygon:[SPPolygon circleWithX:x y:y radius:radius]];
}

- (void)drawEllipseWithX:(float)x y:(float)y radiusX:(float)radiusX radiusY:(float)radiusY
{
    [self appendPolygon:[SPPolygon ellipseWithX:x y:y radiusX:radiusX radiusY:radiusY]];
}

- (void)drawRectangleWithX:(float)x y:(float)y width:(float)width height:(float)height
{
    [self appendPolygon:[SPPolygon rectangleWithX:x y:y width:width height:height]];
}

- (void)drawPolygon:(SPPolygon *)polygon
{
    [self appendPolygon:polygon];
}

- (void)beginFill:(uint)color
{
    [self beginFill:color alpha:1.0f];
}

- (void)beginFill:(uint)color alpha:(float)alpha
{
    _fillColor = color;
    _fillAlpha = alpha;
}

- (void)endFill
{
    _fillColor = SPColorWhite;
    _fillAlpha = 1.0f;
}

- (void)clear
{
    _vertexData.numVertices = 0;
    _indexData.numIndices = 0;
    [_polygons removeAllObjects];
    [self destroyBuffers];
}

#pragma mark SPDisplayObject

- (void)render:(SPRenderSupport *)support
{
    if (_indexData.numIndices == 0)
        return;
    
    SPExecuteWithDebugMarker("Canvas")
    {
        if (_syncRequired)
            [self syncBuffers];
        
        [support finishQuadBatch];
        [support addDrawCalls:1];
        [support applyBlendModeForPremultipliedAlpha:NO];
        
        int uMvpMatrix = [_program uniformByName:@"uMvpMatrix"];
        int uAlpha     = [_program uniformByName:@"uAlpha"];
        int aPosition  = [_program attributeByName:@"aPosition"];
        int aColor     = [_program attributeByName:@"aColor"];
        
        glUseProgram(_program.name);
        glUniformMatrix4fv(uMvpMatrix, 1, 0, support.mvpMatrix3D.rawData);
        glUniform1f(uAlpha, support.alpha * self.alpha);
        
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferName);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferName);
        
        glEnableVertexAttribArray(aPosition);
        glEnableVertexAttribArray(aColor);
        
        glVertexAttribPointer(aPosition, 2, GL_FLOAT, GL_FALSE,
                              sizeof(SPVertex), (void *)offsetof(SPVertex, position));
        
        glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE,
                              sizeof(SPVertex), (void *)offsetof(SPVertex, color));
        
        glDrawElements(GL_TRIANGLES, (int)_indexData.numIndices, GL_UNSIGNED_SHORT, 0);
    }
}

- (SPRectangle *)boundsInSpace:(SPDisplayObject *)targetSpace
{
    SPMatrix *transformationMatrix = targetSpace == self ? nil : [self transformationMatrixToSpace:targetSpace];
    return [_vertexData boundsAfterTransformation:transformationMatrix];
}

- (SPDisplayObject *)hitTestPoint:(SPPoint *)localPoint forTouch:(BOOL)forTouch
{
    if (forTouch && (!self.visible || !self.touchable))
        return nil;
    
    for (SPPolygon *polygon in _polygons)
        if ([polygon containsPoint:localPoint])
            return self;
    
    return nil;
}

#pragma mark NSCopying

- (instancetype)copyWithZone:(NSZone *)zone
{
    SPCanvas *canvas = [super copyWithZone:zone];
    
    SP_RELEASE_AND_COPY(canvas->_vertexData, _vertexData);
    SP_RELEASE_AND_COPY(canvas->_indexData, _indexData);
    
    [canvas->_polygons release];
    canvas->_polygons = [[NSMutableArray alloc] initWithArray:_polygons copyItems:YES];
    
    canvas->_fillAlpha = _fillAlpha;
    canvas->_fillColor = _fillColor;
    canvas->_syncRequired = YES;
    
    return canvas;
}

#pragma mark Private

- (void)appendPolygon:(SPPolygon *)polygon
{
    NSInteger oldNumVertices = _vertexData.numVertices;
    NSInteger oldNumIndices = _indexData.numIndices;
    
    [polygon triangulate:_indexData];
    [polygon copyToVertexData:_vertexData atIndex:oldNumVertices];
    
    NSInteger newNumIndices = _indexData.numIndices;
    [_indexData offsetIndicesAtIndex:oldNumIndices numIndices:newNumIndices-oldNumIndices offset:oldNumVertices];
    
    [self applyFillColorAtIndex:oldNumVertices numVertices:polygon.numVertices];
    
    [_polygons addObject:polygon];
    _syncRequired = YES;
}

- (void)registerPrograms
{
    _program = [[Sparrow.currentController programByName:PROGRAM_NAME] retain];
    if (_program) return;
    
    _program = [[SPProgram alloc] initWithVertexShader:[self vertexShader] fragmentShader:[self fragmentShader]];
    [Sparrow.currentController registerProgram:_program name:PROGRAM_NAME];
}

- (NSString *)vertexShader
{
    return
    @"attribute vec4 aPosition; \n"
    @"attribute vec4 aColor; \n"
    @"uniform mat4 uMvpMatrix; \n"
    @"uniform float uAlpha; \n"
    @"varying lowp vec4 vColor; \n"
    @"void main() { \n"
    @"  gl_Position = uMvpMatrix * aPosition; \n"
    @"  vColor = aColor * uAlpha; \n"
    @"} \n";
}

- (NSString *)fragmentShader
{
    return
    @"varying lowp vec4 vColor;"
    @"void main() { \n"
    @"  gl_FragColor = vColor; \n"
    @"} \n";
}

- (void)applyFillColorAtIndex:(NSInteger)vertexIndex numVertices:(NSInteger)numVertices
{
    NSInteger endIndex = vertexIndex + numVertices;
    for (NSInteger i=vertexIndex; i<endIndex; ++i)
        [_vertexData setColor:_fillColor alpha:_fillAlpha atIndex:i];
}

- (void)syncBuffers
{
    [self destroyBuffers];
    
    NSInteger numVertices = _vertexData.numVertices;
    NSInteger numIndices  = _indexData.numIndices;
    
    glGenBuffers(1, &_vertexBufferName);
    glGenBuffers(1, &_indexBufferName);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferName);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(SPVertex), _vertexData.vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferName);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(ushort), _indexData.indices, GL_STATIC_DRAW);
    
    _syncRequired = NO;
}

- (void)destroyBuffers
{
    if (_vertexBufferName)
    {
        glDeleteBuffers(1, &_vertexBufferName);
        _vertexBufferName = 0;
    }
    
    if (_indexBufferName)
    {
        glDeleteBuffers(1, &_indexBufferName);
        _indexBufferName = 0;
    }
}

@end
