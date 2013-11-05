#import "BackgroundWorker.h"

@implementation BackgroundWorker

- (id) initWithAsync:(IAsyncable *)_async
{
	if (self = [super init])
		async = _async;
	
	return self;
}

- (void) start
{
	[NSThread detachNewThreadSelector:@selector(run) toTarget:self withObject:nil];
}

- (void) run
{
	async->RunAsync();
}

@end
