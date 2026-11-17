#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface PS2Bridge : NSObject

- (BOOL)loadBIOSAtPath:(NSString *)path;
- (void)runFrame;
- (const uint32_t *)framebufferPointer;
- (int)framebufferWidth;
- (int)framebufferHeight;

@end

NS_ASSUME_NONNULL_END
