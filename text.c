#include <furi.h>
#include <string.h>
#include "text.h"

static void render_callback(Canvas *canvas, void *ctx)
{
    TextApp *text_app = ctx;

    // malloc(sizeof(char) * (ROW_SIZE + 5));

    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 0, 10, string_get_cstr(text_app->file_name));
    canvas_set_font(canvas, FontSecondary);

    const char *data = string_get_cstr(text_app->file_content);

    char screen_buffer[ROWS][COLS + 1] = {{0}};

    uint8_t data_offset = 0;
    for (uint8_t i = 0; i < ROWS; i++)
    {
        for (uint8_t j = 0; j < COLS; j++)
        {
            data_offset++;
            if (data[text_app->cursor + data_offset++] == '\n')
            {
                i++;
                data_offset++;
                break;
            }
            else
            {
                screen_buffer[i][j] = data[text_app->cursor + data_offset];
                data_offset++;
                FURI_LOG_T(TAG, "Cursor: %d, Offset: %d", text_app->cursor, data_offset);
                FURI_LOG_T(TAG, "I: %d, J: %d", i, j);
                FURI_LOG_T(TAG, "Data: %c, Buffer: %c", screen_buffer[i][j]);
            }
        }
        for (uint8_t i = 0; i < ROWS; i++)
        {
            canvas_draw_str(canvas, 0, 20 + 5 * i, screen_buffer[i]);
        }
    }
}

static void input_callback(InputEvent *input_event, void *ctx)
{
    FURI_LOG_D(TAG, "input_callback");
    TextApp *text_app = ctx;
    if (input_event->type == InputTypeShort)
    {
        furi_message_queue_put(text_app->input_queue, input_event, 0);
    }
}

bool load_file(const char *file_path, TextApp *text_app)
{
    furi_assert(file_path);
    Storage *storage = furi_record_open(RECORD_STORAGE);
    File *file = storage_file_alloc(storage);

    string_init(text_app->file_content);

    bool success = false;

    do
    {
        if (!storage_file_open(file, file_path, FSAM_READ, FSOM_OPEN_EXISTING))
        {
            FURI_LOG_E(TAG, "Unable to open file");
            break;
        };

        uint16_t ret = 0;
        do
        {
            uint8_t buffer[65] = {0};
            ret = storage_file_read(file, buffer, sizeof(buffer) - 1);
            for (size_t i = 0; i < ret; i++)
            {
                string_push_back(text_app->file_content, buffer[i]);
                text_app->line_nb++;
            }
            FURI_LOG_T(TAG, "%s", buffer);

            if (storage_file_eof(file))
            {
                break;
            }
        } while (ret > 0);

        success = true;

        if (!string_size(text_app->file_content))
        {
            FURI_LOG_I(TAG, "Empty file");
            break;
        }

    } while (0);

    storage_file_seek(file, 0, true);
    storage_file_free(file);
    furi_record_close(RECORD_STORAGE);
    return success;
}

int32_t text_reader_app(void *p)
{
    TextApp *text_app = malloc(sizeof(TextApp));
    string_t file_path;
    string_init(file_path);

    do
    {
        if (p)
        {
            string_cat_str(file_path, p);
            FURI_LOG_I(TAG, "File path: %s", p);
        }
        else
        {
            FURI_LOG_D(TAG, "No argument, showing file browser");

            string_set_str(file_path, TEXT_READER_APP_PATH_FOLDER);

            DialogsFileBrowserOptions browser_options;
            dialog_file_browser_set_basic_options(
                &browser_options, TEXT_READER_APP_EXTENSION, &I_music_10px);
            browser_options.hide_ext = false;
            DialogsApp *dialogs = furi_record_open(RECORD_DIALOGS);
            bool res = dialog_file_browser_show(dialogs, file_path, file_path, &browser_options);

            furi_record_close(RECORD_DIALOGS);
            if (!res)
            {
                FURI_LOG_E(TAG, "No file selected");
                string_clear(file_path);
                free(text_app);
                return 1;
            }

            FURI_LOG_I(TAG, "Selected file path: %s", string_get_cstr(file_path));
        }

        if (!load_file(string_get_cstr(file_path), text_app))
        {
            FURI_LOG_E(TAG, "Error reading file");
            break;
        }

        string_init(text_app->file_name);
        path_extract_filename(file_path, text_app->file_name, false);

        text_app->mutex = furi_mutex_alloc(FuriMutexTypeNormal);

        text_app->input_queue = furi_message_queue_alloc(8, sizeof(InputEvent));
        text_app->view_port = view_port_alloc();
        view_port_draw_callback_set(text_app->view_port, render_callback, text_app);
        view_port_input_callback_set(text_app->view_port, input_callback, text_app);

        // Open GUI and register view_port
        text_app->gui = furi_record_open(RECORD_GUI);
        gui_add_view_port(text_app->gui, text_app->view_port, GuiLayerFullscreen);

        InputEvent input;
        while (furi_message_queue_get(text_app->input_queue, &input, FuriWaitForever) ==
               FuriStatusOk)
        {
            furi_check(furi_mutex_acquire(text_app->mutex, FuriWaitForever) == FuriStatusOk);

            if (input.key == InputKeyBack)
            {
                furi_mutex_release(text_app->mutex);
                break;
            }
            else if (input.key == InputKeyUp)
            {
                // Scroll up
            }
            else if (input.key == InputKeyDown)
            {
                // Scroll down
            }

            furi_mutex_release(text_app->mutex);
            view_port_update(text_app->view_port);
        }
    } while (0);

    string_clear(file_path);
    string_clear(text_app->file_name);
    string_clear(text_app->file_content);
    gui_remove_view_port(text_app->gui, text_app->view_port);
    furi_record_close(RECORD_GUI);
    view_port_free(text_app->view_port);
    furi_message_queue_free(text_app->input_queue);
    furi_mutex_free(text_app->mutex);
    free(text_app);
    return 0;
}