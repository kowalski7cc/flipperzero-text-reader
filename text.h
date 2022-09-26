#pragma once
#include "text_files.h"
#include <dialogs/dialogs.h>
#include <storage/storage.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <input/input.h>
#include <toolbox/path.h>

#define TAG "Text Reader"
#define ROWS 5
#define COLS 25

typedef struct
{
    Gui *gui;
    ViewPort *view_port;
    FuriMessageQueue *input_queue;
    FuriMutex *mutex;
    string_t file_name;
    string_t file_content;
    uint16_t chars_size;
    uint16_t cursor;
} TextApp;
