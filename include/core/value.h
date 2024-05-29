#ifndef VALUE_H
#define VALUE_H

#include "defines.h"
#include "core/string.h"
#include "memory/object.h"

typedef union CTValue {
    i8 as_byte;
    i16 as_short;
    i32 as_int;
    i64 as_long;
    f32 as_float;
    f64 as_double;
    u8* as_ptr;
    union CTValue* as_value_ptr;
    CTString* as_string;
    CTWideString* as_wide_string;
    CTObject* as_object;
} CTValue;

#define CT_null (CTValue) 0

#endif //VALUE_H
