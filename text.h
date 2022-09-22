#pragma once
#include <text_files.h>
#include <dialogs/dialogs.h>
#include <storage/storage.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <input/input.h>

typedef struct
{
    Gui *gui;
    ViewDispatcher *view_dispatcher
} TextApp;
