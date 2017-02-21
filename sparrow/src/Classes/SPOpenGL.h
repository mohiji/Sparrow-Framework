//
//  SPOpenGL.h
//  Sparrow
//
//  Created by Robert Carone on 10/8/13.
//  Copyright 2011-2015 Gamua. All rights reserved.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the Simplified BSD License.
//

#import <Sparrow/SparrowBase.h>
#import <Sparrow/SPMacros.h>

#if TARGET_OS_IPHONE
    #import <OpenGLES/ES2/glext.h>
    #import <OpenGLES/ES3/gl.h>
    #import <OpenGLES/ES3/glext.h>
#else
    #import <OpenGL/gl3.h>
#endif

// -----------------------------------------------------------
// EXPERIMENTAL FEATURE: Activate the OpenGL state cache here!
// -----------------------------------------------------------

#define SP_ENABLE_GL_STATE_CACHE 0

/// Sparrow's OpenGL state cache reference type.
typedef struct SGLStateCache *SGLStateCacheRef;

/// Allocates a new state cache.
SP_EXTERN SGLStateCacheRef sglStateCacheCreate(void);

/// Returns a copy of a state cache.
SP_EXTERN SGLStateCacheRef sglStateCacheCopy(SGLStateCacheRef stateCache);

/// Deallocates a state cache.
SP_EXTERN void sglStateCacheRelease(SGLStateCacheRef stateCache);

/// Resets the state cache's values to an null state.
SP_EXTERN void sglStateCacheReset(SGLStateCacheRef stateCache);

/// Returns the current global state cache.
SP_EXTERN SGLStateCacheRef sglStateCacheGetCurrent(void);

/// Sets the current global state cache, if NULL will use a default state cache.
SP_EXTERN void sglStateCacheSetCurrent(SGLStateCacheRef stateCache);

/// Returns a string representing an OpenGL error code.
SP_EXTERN const char* sglGetErrorString(uint error);

/// extension remappings

#if GL_OES_vertex_array_object
    #undef GL_VERTEX_ARRAY_BINDING

    #define GL_VERTEX_ARRAY_BINDING     GL_VERTEX_ARRAY_BINDING_OES
    #define glBindVertexArray           glBindVertexArrayOES
    #define glGenVertexArrays           glGenVertexArraysOES
    #define glDeleteVertexArrays        glDeleteVertexArraysOES
    #define glIsVertexArray             glIsVertexArrayOES
#endif

/// debug utils

#define SP_FORCE_DEBUG_MARKERS 0

#if TARGET_OS_IPHONE && (DEBUG || SP_FORCE_DEBUG_MARKERS)
    #define SPPushDebugMarker(s) \
        glPushGroupMarkerEXT(0, s)

    #define SPPopDebugMarker() \
        glPopGroupMarkerEXT()

    #define SPExecuteWithDebugMarker(s) \
        for (BOOL SP_CONCAT(_SP_DEBUG_MRK, __LINE__) = _SPStartDebugMarker(s); \
             SP_CONCAT(_SP_DEBUG_MRK, __LINE__); \
             SP_CONCAT(_SP_DEBUG_MRK, __LINE__) = _SPEndDebugMarker())

    SP_INLINE BOOL _SPStartDebugMarker(const char *s) { glPushGroupMarkerEXT(0, s); return YES; }
    SP_INLINE BOOL _SPEndDebugMarker() { glPopGroupMarkerEXT(); return NO; }

#else // !DEBUG
    #define SPPushDebugMarker(s)
    #define SPPopDebugMarker()
    #define SPExecuteWithDebugMarker(s)
#endif

/// state cache remappings

#if SP_ENABLE_GL_STATE_CACHE
    #undef  glBindVertexArray
    #undef  glDeleteVertexArrays

    #define glActiveTexture             sglActiveTexture
    #define glBindBuffer                sglBindBuffer
    #define glBindFramebuffer           sglBindFramebuffer
    #define glBindRenderbuffer          sglBindRenderbuffer
    #define glBindTexture               sglBindTexture
    #define glBindVertexArray           sglBindVertexArray
    #define glBlendFunc                 sglBlendFunc
    #define glDeleteBuffers             sglDeleteBuffers
    #define glDeleteFramebuffers        sglDeleteFramebuffers
    #define glDeleteProgram             sglDeleteProgram
    #define glDeleteRenderbuffers       sglDeleteRenderbuffers
    #define glDeleteTextures            sglDeleteTextures
    #define glDeleteVertexArrays        sglDeleteVertexArrays
    #define glDisable                   sglDisable
    #define glEnable                    sglEnable
    #define glGetIntegerv               sglGetIntegerv
    #define glScissor                   sglScissor
    #define glUseProgram                sglUseProgram
    #define glViewport                  sglViewport

    SP_EXTERN void                      sglActiveTexture(GLenum texture);
    SP_EXTERN void                      sglBindBuffer(GLenum target, GLuint buffer);
    SP_EXTERN void                      sglBindFramebuffer(GLenum target, GLuint framebuffer);
    SP_EXTERN void                      sglBindRenderbuffer(GLenum target, GLuint renderbuffer);
    SP_EXTERN void                      sglBindTexture(GLenum target, GLuint texture);
    SP_EXTERN void                      sglBindVertexArray(GLuint array);
    SP_EXTERN void                      sglBlendFunc(GLenum sfactor, GLenum dfactor);
    SP_EXTERN void                      sglDeleteBuffers(GLsizei n, const GLuint* buffers);
    SP_EXTERN void                      sglDeleteFramebuffers(GLsizei n, const GLuint* framebuffers);
    SP_EXTERN void                      sglDeleteProgram(GLuint program);
    SP_EXTERN void                      sglDeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers);
    SP_EXTERN void                      sglDeleteTextures(GLsizei n, const GLuint* textures);
    SP_EXTERN void                      sglDeleteVertexArrays(GLsizei n, const GLuint* arrays);
    SP_EXTERN void                      sglDisable(GLenum cap);
    SP_EXTERN void                      sglEnable(GLenum cap);
    SP_EXTERN void                      sglGetIntegerv(GLenum pname, GLint* params);
    SP_EXTERN void                      sglScissor(GLint x, GLint y, GLsizei width, GLsizei height);
    SP_EXTERN void                      sglUseProgram(GLuint program);
    SP_EXTERN void                      sglViewport(GLint x, GLint y, GLsizei width, GLsizei height);
#endif
