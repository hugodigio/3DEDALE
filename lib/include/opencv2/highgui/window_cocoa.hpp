/* The file is created by Seb  on May 2016 */

/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                         License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2010, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#import <Cocoa/Cocoa.h>

#include <iostream>


@interface CVView : NSView {
    NSImage *image;
}
@property(retain) NSImage *image;
- (void)setImageData:(CvArr *)arr;
@end

@interface CVWindow : NSWindow {
    NSMutableDictionary *sliders;
    CvMouseCallback mouseCallback;
    void *mouseParam;
    BOOL autosize;
    BOOL firstContent;
    int status;
}
@property(assign) CvMouseCallback mouseCallback;
@property(assign) void *mouseParam;
@property(assign) BOOL autosize;
@property(assign) BOOL firstContent;
@property(retain) NSMutableDictionary *sliders;
@property(readwrite) int status;
- (CVView *)contentView;
- (void)cvSendMouseEvent:(NSEvent *)event type:(int)type flags:(int)flags;
- (void)cvMouseEvent:(NSEvent *)event;
- (void)createSliderWithName:(const char *)name maxValue:(int)max value:(int *)value callback:(CvTrackbarCallback)callback;
@end

CV_IMPL int cvInitSystem( int , char** );

static CVWindow *cvGetWindow(const char *name);

CV_IMPL int cvStartWindowThread();

CV_IMPL void cvDestroyWindow( const char* name);


CV_IMPL void cvDestroyAllWindows( void );


CV_IMPL void cvShowImage( const char* name, const CvArr* arr);

CV_IMPL void cvResizeWindow( const char* name, int width, int height);

CV_IMPL void cvMoveWindow( const char* name, int x, int y);

CV_IMPL int cvCreateTrackbar (const char* trackbar_name,
                              const char* window_name,
                              int* val, int count,
                              CvTrackbarCallback on_notify);


CV_IMPL int cvCreateTrackbar2(const char* trackbar_name,
                              const char* window_name,
                              int* val, int count,
                              CvTrackbarCallback2 on_notify2,
                              void* userdata);

CV_IMPL void
cvSetMouseCallback( const char* name, CvMouseCallback function, void* info);

 CV_IMPL int cvGetTrackbarPos( const char* trackbar_name, const char* window_name );

CV_IMPL void cvSetTrackbarPos(const char* trackbar_name, const char* window_name, int pos);

CV_IMPL void cvSetTrackbarMax(const char* trackbar_name, const char* window_name, int maxval);

CV_IMPL void cvSetTrackbarMin(const char* trackbar_name, const char* window_name, int minval);
CV_IMPL void* cvGetWindowHandle( const char* name );


CV_IMPL const char* cvGetWindowName( void* window_handle );

CV_IMPL int cvNamedWindow( const char* name, int flags );
CV_IMPL int cvWaitKey (int maxWait);

double cvGetModeWindow_COCOA( const char* name );

void cvSetModeWindow_COCOA( const char* name, double prop_value );
void cv::setWindowTitle(const String& winname, const String& title);


/* End of file. */
