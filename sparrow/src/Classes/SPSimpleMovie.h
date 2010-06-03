//
//  SPSimpleMovie.h
//  Sparrow
//
//  Created by Daniel Sperl on 01.05.10.
//  Copyright 2010 Incognitek. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SPSprite.h"
#import "SPTexture.h"
#import "SPAnimatable.h"
#import "SPImage.h"
#import "SPSoundChannel.h"

@interface SPSimpleMovie : SPImage <SPAnimatable>
{
    NSMutableArray *mFrames;
    NSMutableArray *mSounds;
    NSMutableArray *mFrameDurations;
    
    double mDefaultDuration;
    double mTotalDuration;
    double mElapsedTime;
    BOOL mLoop;
    BOOL mPlaying;
    int mCurrentFrame;
}

- (id)initWithFrame:(SPTexture *)texture fps:(float)fps;

- (int)addFrame:(SPTexture *)texture;
- (int)addFrame:(SPTexture *)texture withDuration:(double)duration;

- (void)insertFrame:(SPTexture *)texture atIndex:(int)frameID;
- (void)removeFrameAtIndex:(int)frameID;

- (void)setFrame:(SPTexture *)texture atIndex:(int)frameID;
- (void)setSound:(SPSoundChannel *)sound atIndex:(int)frameID;
- (void)setDuration:(double)duration atIndex:(int)frameID;

- (SPTexture *)frameAtIndex:(int)frameID;
- (SPSoundChannel *)soundAtIndex:(int)frameID;
- (double)durationAtIndex:(int)frameID;

- (void)play;
- (void)pause;

+ (SPSimpleMovie *)movieWithFrame:(SPTexture *)texture fps:(float)fps;

@property (nonatomic, readonly) int numFrames;
@property (nonatomic, readonly) double duration;
@property (nonatomic, readonly) BOOL isPlaying;
@property (nonatomic, assign)   BOOL loop;
@property (nonatomic, assign)   int currentFrame;

@end
