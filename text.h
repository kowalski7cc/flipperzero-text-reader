#pragma once
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <input/input.h>

typedef struct {
    Gui* gui;
    ViewDispatcher* view_dispatcher
} TextApp;
