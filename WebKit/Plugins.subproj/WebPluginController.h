//
//  WebPluginController.h
//  WebKit
//
//  Created by Chris Blumenberg on Wed Oct 23 2002.
//  Copyright (c) 2002 Apple Computer, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class WebHTMLView;

@protocol WebPlugin;
@protocol WebPluginContainer;

@interface WebPluginController : NSObject
{
    NSView *_documentView;
    NSMutableArray *_views;
    BOOL _started;
}

- (id)initWithDocumentView:(NSView *)view;

- (void)addPlugin:(NSView *)view;

- (void)startAllPlugins;
- (void)stopAllPlugins;
- (void)destroyAllPlugins;

@end
