#include <furi.h>
#include "text.h"

int32_t text_reader_app(void* p) {
    TextApp* text_app = malloc(sizeof(TextApp));

    if(p) {
        // Open file by path
    } else {
        // Show file browser
    }

    free(text_app);
}