#include <stdio.h>

#include "defines.h"
#include "core/vm.h"

i32 main(void) {
    StartVM();
    CTVM* vm = GetVM();

    return 0;
}