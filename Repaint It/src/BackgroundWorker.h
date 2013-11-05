#import <Foundation/Foundation.h>
#include "IAsyncable.h"

@interface BackgroundWorker : NSObject {
	IAsyncable *async;
}

- (id) initWithAsync:(IAsyncable *)async;
- (void) start;

@end
