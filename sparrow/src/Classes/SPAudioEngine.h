//
//  SPAudioEngine.h
//  Sparrow
//
//  Created by Daniel Sperl on 14.11.09.
//  Copyright 2011-2015 Gamua. All rights reserved.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the Simplified BSD License.
//

#import <Sparrow/SparrowBase.h>
#import <Sparrow/SPMacros.h>

#if TARGET_OS_IPHONE
    #import <AVFoundation/AVAudioSession.h>
#endif

NS_ASSUME_NONNULL_BEGIN

SP_EXTERN NSString *const SPNotificationMasterVolumeChanged;
SP_EXTERN NSString *const SPNotificationAudioInteruptionBegan;
SP_EXTERN NSString *const SPNotificationAudioInteruptionEnded;

/// Deprecated, use equivalent `AVAudioSession` NSString* constants instead.
SP_DEPRECATED
typedef NS_ENUM(UInt32, SPAudioSessionCategory) {
    SPAudioSessionCategory_AmbientSound     = 'ambi',
    SPAudioSessionCategory_SoloAmbientSound = 'solo',
    SPAudioSessionCategory_MediaPlayback    = 'medi',
    SPAudioSessionCategory_RecordAudio      = 'reca',
    SPAudioSessionCategory_PlayAndRecord    = 'plar',
#if !TARGET_OS_TV
    SPAudioSessionCategory_AudioProcessing  = 'proc'
#endif
};

/** ------------------------------------------------------------------------------------------------ 

 The SPAudioEngine prepares the system for audio playback and controls global volume.
 
 Before you play sounds, you should start an audio session. The type of the audio session
 defines how iOS will handle audio processing and how iPod music will mix with your audio.
 
 - `AVAudioSessionCategoryAmbient`:         iPod music mixes with your audio, audio silences on mute
 - `AVAudioSessionCategorySoloAmbient`:     iPod music is silenced, audio silences on mute
 - `AVAudioSessionCategoryPlayback`:        iPod music is silenced, audio continues on mute
 - `AVAudioSessionCategoryRecord`:          iPod music is silenced, used for audio recording
 - `AVAudioSessionCategoryPlayAndRecord`:   iPod music is silenced, for simultaneous in- and output
 - `AVAudioSessionCategoryAudioProcessing`: For using an audio hardware codec or signal processor
 
 */

@interface SPAudioEngine : NSObject

/// -------------
/// @name Methods
/// -------------

#if TARGET_OS_IPHONE
/// Starts an audio session with a specified `AVAudioSession` category.  Call this at the start of
/// your application.
///
/// @see `AVAudioSession` for details.
+ (void)startWithCategory:(NSString *)avCategory;

/// Deprectated, use `startWithCategory:` instead.
+ (void)start:(SPAudioSessionCategory)category SP_DEPRECATED;
#endif

/// Starts an audio session with with the category 'AVAudioSessionCategorySoloAmbient'.
+ (void)start;

/// Stops the audio session. Call this before the application shuts down.
+ (void)stop;

/// The master volume for all audio. Default: 1.0
+ (float)masterVolume;

/// Set the master volume for all audio. Range: [0.0 - 1.0]
+ (void)setMasterVolume:(float)volume;

@end

NS_ASSUME_NONNULL_END
