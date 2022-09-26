#pragma once
#include "text_files.h"
#include <dialogs/dialogs.h>
#include <storage/storage.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <input/input.h>
#include <toolbox/path.h>

#define TAG "Text Reader"
#define ROWS 4
#define COLS 30

typedef struct
{
    Gui *gui;
    ViewPort *view_port;
    FuriMessageQueue *input_queue;
    FuriMutex *mutex;
    string_t file_name;
    string_t file_content;
    uint16_t line_nb;
    uint16_t cursor;
} TextApp;
