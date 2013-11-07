#if 0

#import <UIKit/UIKit.h>
#import <GameKit/GameKit.h>
#include <mach/mach_time.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#include "IGameCenterViewProxy.h"

#import "IGame.h"

class SplashScreen;

@interface Repaint_ItViewController : UIViewController <GKLeaderboardViewControllerDelegate, GKAchievementViewControllerDelegate> //<UIGestureRecognizerDelegate>
{
    EAGLContext *context;
    GLuint program;
	IGame *game;
	
	mach_timebase_info_data_t timeInfo;
	uint64_t baseTime;
	uint64_t lastTime;
	float timeElapsed;
	
	float fpsUpdateCooldown;
	int fpsCount;
    
    BOOL animating;
    NSInteger animationFrameInterval;
    CADisplayLink *displayLink;
	IGameCenterViewProxy *gameCenterViewProxy;
	SplashScreen *splashScreen;
}

@property (readonly, nonatomic, getter=isAnimating) BOOL animating;
@property (nonatomic) NSInteger animationFrameInterval;

- (void)handleEnterBackground;
- (void)handleEnterForeground;
- (void)startAnimation;
- (void)stopAnimation;
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
//- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch;

@end

#endif
