#include <iostream>

#include "common/logger.h"
#include "math/vec.h"
#include "math/mat.h"

#include "system/window-manager-incl.h"

#import "AppDelegate.h"

using namespace std;

int main() {
    NSApplication * application = [NSApplication sharedApplication];
    AppDelegate * delegate = [[AppDelegate alloc] init];
    [application setDelegate:delegate];
    [application run];
    return 0;
}
