#include "core/string.h"

bool StringEquals(CTString* string1, CTString* string2) {
    if (string1->length != string2->length) return false;

    for (u64 i = 0; i < string1->length; i++) {
        if (string1->data[i] != string2->data[i]) {
            return false;
        }
    }

    return true;
}
