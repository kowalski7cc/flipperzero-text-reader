#pragma once
#include "text_files.h"
#include <dialogs/dialogs.h>
#include <storage/storage.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <input/input.h>
#include <toolbox/path.h>

#define TAG "Text Reader"

typedef struct
{
    Gui *gui;
    ViewPort *view_port;
    FuriMessageQueue *input_queue;
    FuriMutex *mutex;
    string_t file_name;
    string_t file_content;
    uint8_t cursor;
} TextApp;
