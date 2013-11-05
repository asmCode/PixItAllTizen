//
//  Repaint_ItViewController.m
//  Repaint It
//
//  Created by Majak on 2/7/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import "Repaint_ItViewController.h"
#import "EAGLView.h"
#import "Game.h"
#include "Log.h"
#include <GameKit/GameKit.h>
#include <vector>
#include "GameCenterViewProxy.h"
#include "Environment.h"
#include "Utils.h"
#include "SplashScreen.h"

// Uniform index.
enum {
    UNIFORM_TRANSLATE,
    NUM_UNIFORMS
};
GLint uniforms[NUM_UNIFORMS];

// Attribute index.
enum {
    ATTRIB_VERTEX,
    ATTRIB_COLOR,
    NUM_ATTRIBUTES
};

@interface Repaint_ItViewController ()
@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, assign) CADisplayLink *displayLink;
@end

@implementation Repaint_ItViewController

@synthesize animating, context, displayLink;

- (void)awakeFromNib
{
	[[NSThread currentThread] setName:@"main"];
	
	Environment::GetInstance()->SetIOSVersion([[[UIDevice currentDevice] systemVersion] UTF8String]);
	
	game = NULL;
    EAGLContext *aContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        
    if (!aContext)
        NSLog(@"Failed to create ES context");
    else if (![EAGLContext setCurrentContext:aContext])
        NSLog(@"Failed to set ES context current");
    
	self.context = aContext;
	[aContext release];
	
    [(EAGLView *)self.view setContext:context];
    [(EAGLView *)self.view setFramebuffer];
	
	std::string basePath = [[[NSBundle mainBundle] bundlePath] UTF8String];
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	std::string splashFilename;
	switch (Environment::GetInstance()->GetScreenWidth())
	{
		case 320:
			splashFilename = "/Default~iphone.png";
			break;
			
		case 640:
			splashFilename = "/Default@2x~iphone.png";
			break;
			
		case 768:
			splashFilename = "/Default~ipad.png";
			break;
	}

	splashScreen = new SplashScreen(NULL, basePath + splashFilename);
	if (!splashScreen->Initialize())
	{
		assert(false);
		return;
	}
	splashScreen->Draw(0, 0);
	[(EAGLView *)self.view presentFramebuffer];
	
    animating = FALSE;
    animationFrameInterval = 1;
    self.displayLink = nil;
	
	fpsUpdateCooldown = 0.0f;
	fpsCount = 0;
	baseTime = 0;
	lastTime = mach_absolute_time();
	timeElapsed = 0.0f;
	mach_timebase_info(&timeInfo);
	
//	UILongPressGestureRecognizer *longPressGestureRec =
//	[[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(handleLongPressGesture:)];
//	longPressGestureRec.minimumPressDuration = 2.0f;
//	longPressGestureRec.allowableMovement = 0.0f;
//	[self.view addGestureRecognizer:longPressGestureRec];
//	[longPressGestureRec release];
	
	UIPinchGestureRecognizer *pinchGestureRec =
		[[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(handlePinchGesture:)];
	[self.view addGestureRecognizer:pinchGestureRec];
	[pinchGestureRec release];
	
	UIPanGestureRecognizer *panGestureRec =
	[[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handlePanGesture:)];
//	panGestureRec.delegate = self;
	[self.view addGestureRecognizer:panGestureRec];
	[panGestureRec release];
	
	UITapGestureRecognizer *tapGestureRec =
		[[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleTapGesture:)];
	tapGestureRec.numberOfTouchesRequired = 1;
	tapGestureRec.numberOfTapsRequired = 1;
	[self.view addGestureRecognizer:tapGestureRec];
	[tapGestureRec release];
	
	glDepthMask(GL_FALSE);
	
	//if (!game ->LoadContent())
	//	assert(0);
}

//- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch
//{
//	return YES;
//}

- (IBAction)handleLongPressGesture:(UIGestureRecognizer *)sender
{
	//float scaleFactor = [self.view.window.screen scale];
	float scaleFactor = 1.0f;
	UILongPressGestureRecognizer *rec = (UILongPressGestureRecognizer *)sender;
	
	if (rec.state == UIGestureRecognizerStateEnded)
	{
		CGPoint pos = [rec locationInView:self.view];
		game ->HandleRelease(sm::Point<int>(pos.x * scaleFactor, pos.y * scaleFactor));
		game ->HandleTapGesture(sm::Point<int>(pos.x * scaleFactor, pos.y * scaleFactor));
	}
}

- (IBAction)handlePinchGesture:(UIGestureRecognizer *)sender
{
	//float scaleFactor = [self.view.window.screen scale];
	float scaleFactor = 1.0f;
	UIPinchGestureRecognizer *rec = (UIPinchGestureRecognizer *)sender;
	CGFloat factor = [(UIPinchGestureRecognizer *)sender scale];
	CGFloat velocity = [(UIPinchGestureRecognizer *)sender velocity];
	CGPoint pos = [rec locationInView:self.view];
	
	UIPinchGestureRecognizer *pinchGestureRec = (UIPinchGestureRecognizer *)sender;
	game ->HandleRelease(sm::Point<int>(pos.x * scaleFactor, pos.y * scaleFactor));
	switch ([pinchGestureRec state])
	{
		case UIGestureRecognizerStateBegan:
			game ->HandlePinchGesture(IGestureHandler::GestureStatus_Began, factor, velocity);
			break;
			
		case UIGestureRecognizerStateChanged:
			game ->HandlePinchGesture(IGestureHandler::GestureStatus_Changed, factor, velocity);
			break;
			
		case UIGestureRecognizerStateEnded:
			game ->HandlePinchGesture(IGestureHandler::GestureStatus_Ended, factor, velocity);
			break;
			
		default:
			break;
	}
}

- (IBAction)handlePanGesture:(UIGestureRecognizer *)sender
{
	UIPanGestureRecognizer *rec = (UIPanGestureRecognizer *)sender;
	//float scaleFactor = [self.view.window.screen scale];
	float scaleFactor = 1.0f;
	CGPoint pos = [rec locationInView:self.view];
	CGPoint trans = [(UIPanGestureRecognizer *)sender translationInView:self.view];
	CGPoint velocity = [(UIPanGestureRecognizer *)sender velocityInView:self.view];
	
	IGestureHandler::GestureStatus state;
	
	game ->HandleRelease(sm::Point<int>(pos.x * scaleFactor, pos.y * scaleFactor));
	
	switch ([sender state])
	{
		case UIGestureRecognizerStateBegan:
			state = IGestureHandler::GestureStatus_Began;
			break;
		case UIGestureRecognizerStateChanged: state = IGestureHandler::GestureStatus_Changed; break;
		case UIGestureRecognizerStateEnded: state = IGestureHandler::GestureStatus_Ended; break;
		default: return;
	}
	
	game ->HandlePanGesture(state,
							sm::Vec2(pos.x * scaleFactor, pos.y * scaleFactor),
							sm::Vec2(trans.x * scaleFactor, trans.y * scaleFactor),
							sm::Vec2(velocity.x, velocity.y));
}

- (IBAction)handleTapGesture:(UIGestureRecognizer *)sender
{
	//float scaleFactor = [self.view.window.screen scale];
	float scaleFactor = 1.0f;
	UITapGestureRecognizer *rec = (UITapGestureRecognizer *)sender;
		
	CGPoint pos = [rec locationInView:self.view];
	game ->HandleRelease(sm::Point<int>(pos.x * scaleFactor, pos.y * scaleFactor));
	game ->HandleTapGesture(sm::Point<int>(pos.x * scaleFactor, pos.y * scaleFactor));
}

- (void)dealloc
{
	game ->Deinitialize();
	delete game;
	
    if (program)
    {
        glDeleteProgram(program);
        program = 0;
    }
    
    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
    
    [context release];
    
    [super dealloc];
}

- (void)viewWillAppear:(BOOL)animated
{
    [self startAnimation];
		
    [super viewWillAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [self stopAnimation];
    
    [super viewWillDisappear:animated];
}

- (void)viewDidUnload
{
	[super viewDidUnload];
	
    if (program)
    {
        glDeleteProgram(program);
        program = 0;
    }

    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
	self.context = nil;	
}

- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}

- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
    /*
	 Frame interval defines how many display frames must pass between each time the display link fires.
	 The display link will only fire 30 times a second when the frame internal is two on a display that refreshes 60 times a second. The default frame interval setting of one will fire 60 times a second when the display refreshes at 60 times a second. A frame interval setting of less than one results in undefined behavior.
	 */
    if (frameInterval >= 1)
    {
        animationFrameInterval = frameInterval;
        
        if (animating)
        {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

- (void)startAnimation
{
    if (!animating)
    {
        CADisplayLink *aDisplayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawFrame)];
        [aDisplayLink setFrameInterval:animationFrameInterval];
        [aDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        self.displayLink = aDisplayLink;
        animating = TRUE;
    }
}

- (void)stopAnimation
{
    if (animating)
    {
        [self.displayLink invalidate];
        self.displayLink = nil;
        animating = FALSE;
    }
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	[super touchesBegan:touches withEvent:event];

	//float scaleFactor = [self.view.window.screen scale];
	float scaleFactor = 1.0f;
	
	std::vector<sm::Vec2> touchPoints;
	NSEnumerator *enumerator = [touches objectEnumerator];
	UITouch *touch;
	
	while ((touch = [enumerator nextObject]))
	{
		CGPoint point = [touch locationInView:self.view];
		sm::Vec2 touchPos(point.x * scaleFactor, point.y * scaleFactor);
		touchPoints.push_back(touchPos);
	}
	
	game ->HandlePress(sm::Point<int>(touchPoints[0].x, touchPoints[0].y));
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	[super touchesEnded:touches withEvent:event];
	
	//float scaleFactor = [self.view.window.screen scale];
	float scaleFactor = 1.0f;
	
	std::vector<sm::Vec2> touchPoints;
	NSEnumerator *enumerator = [touches objectEnumerator];
	UITouch *touch;
	
	while ((touch = [enumerator nextObject]))
	{
		CGPoint point = [touch locationInView:self.view];
		sm::Vec2 touchPos(point.x * scaleFactor, point.y * scaleFactor);
		touchPoints.push_back(touchPos);
	}
	
	game ->HandleRelease(sm::Point<int>(touchPoints[0].x, touchPoints[0].y));
}

- (void)drawFrame
{
	if (game == NULL)
	{
//		NSData *data = [NSData dataWithBytes:(const void *)"zlo" length:strlen("zlo")];
//		
//		NSURL *url = [NSURL URLWithString:@"http://asmcode.idl.pl/iphone_logs/log.php"];
//		NSMutableURLRequest *urlRequest = [NSMutableURLRequest requestWithURL:url];
//		[urlRequest setHTTPMethod:@"POST"];
//		[urlRequest setHTTPBody:data];
//		NSURLResponse *urlResponse = nil;
//		NSError *urlErr = nil;
//		[NSURLConnection sendSynchronousRequest:urlRequest returningResponse:&urlResponse error:&urlErr];
		
		std::string basePath = [[[NSBundle mainBundle] bundlePath] UTF8String];
		
		gameCenterViewProxy = new GameCenterViewProxy(self);
		
		game = new Game(gameCenterViewProxy, splashScreen);
		splashScreen->SetGame((Game*)game);
	
		// quering for documents path
		NSFileManager *fileMng = [NSFileManager defaultManager];
		NSError *err = nil;
		NSURL *appropriateForUrl = [NSURL fileURLWithPath:@"." isDirectory:YES];
		NSURL *docPathUrl = [fileMng URLForDirectory:NSDocumentDirectory
											inDomain:NSUserDomainMask
								   appropriateForURL:appropriateForUrl
											  create:YES
											   error:&err];
		
		const char *docPath = [[docPathUrl path] UTF8String];
		
		Log::StartLog(true, true, (std::string(docPath) + "/" + std::string(Log::DefaultLogFileName)).c_str());
		Log::LogT("Device name: %s", [[[UIDevice currentDevice] name] UTF8String]);
		Log::LogT("System name: %s", [[[UIDevice currentDevice] systemName] UTF8String]);
		Log::LogT("System ver: %s", [[[UIDevice currentDevice] systemVersion] UTF8String]);
		Log::LogT("Model: %s", [[[UIDevice currentDevice] model] UTF8String]);
		Log::LogT("Localized model: %s", [[[UIDevice currentDevice] localizedModel] UTF8String]);
		
		// creating levels_stats directory (objective c really suck)
		std::string lvlStatsPath = std::string([[docPathUrl path] UTF8String]) + std::string("/levels_stats");
		
		err = nil;
		[fileMng createDirectoryAtPath:[NSString stringWithUTF8String:lvlStatsPath.c_str()]
		   withIntermediateDirectories:NO
							attributes:nil
								 error:&err];
		Log::LogE("%s", [[err localizedDescription] UTF8String]);

		if (!game->Initialize(basePath + std::string("/data/"),
							  std::string(docPath)))
			assert(false);
		
//		[docPathUrl release];
//		[appropriateForUrl release];
//		[err release];
//		[fileMng release];
	
		return;
	}
	
	uint64_t currentTime = mach_absolute_time();
	uint64_t duration = currentTime - lastTime;
	float secondsElapsed = (float)((double)(duration * timeInfo.numer / timeInfo.denom) / 1000000000.0);
	lastTime = currentTime;
	timeElapsed += secondsElapsed;
	
#if 0
	fpsCount++;
	fpsUpdateCooldown += secondsElapsed;
	if (fpsUpdateCooldown >= 1.0f)
	{
		printf("FPS: %.2f\n", (float)fpsCount / fpsUpdateCooldown);
		fpsCount = 0;
		fpsUpdateCooldown = 0.0f;
	}
#endif
	
    [(EAGLView *)self.view setFramebuffer];

	game ->Update(timeElapsed, secondsElapsed);
	game ->Draw(timeElapsed, secondsElapsed);
    
    [(EAGLView *)self.view presentFramebuffer];
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)leaderboardViewControllerDidFinish:(GKLeaderboardViewController*)viewController
{
	[self dismissModalViewControllerAnimated:YES];
}

- (void)achievementViewControllerDidFinish:(GKAchievementViewController*)viewController
{
	[self dismissModalViewControllerAnimated:YES];
}

- (void)handleEnterBackground
{
	[self dismissModalViewControllerAnimated:NO];
	
	game->HandleEnterBackground();
}

- (void)handleEnterForeground
{
	lastTime = mach_absolute_time();
	
	game->HandleEnterForeground();
}


@end
