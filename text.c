#include <furi.h>
#include "text.h"

int32_t text_reader_app(void *p)
{
    TextApp *text_app = malloc(sizeof(TextApp));

    string_t file_path;
    string_init(file_path);

    if (p)
    {
        string_cat_str(file_path, p);
    }
    else
    {
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
            break;
        }
    }

    string_clear(file_path);
    free(text_app);
}