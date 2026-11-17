#import "PS2Bridge.h"
#include "PS2Core.hpp"
#include <memory>

@implementation PS2Bridge {
    std::unique_ptr<PS2Core> _core;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        _core = std::make_unique<PS2Core>();
    }
    return self;
}

- (BOOL)loadBIOSAtPath:(NSString *)path {
    return _core->loadBIOS([path UTF8String]);
}

- (void)runFrame {
    _core->runFrame();
}

- (const uint32_t *)framebufferPointer {
    return _core->getFramebuffer();
}

- (int)framebufferWidth {
    return PS2Core::FB_WIDTH;
}

- (int)framebufferHeight {
    return PS2Core::FB_HEIGHT;
}

@end
