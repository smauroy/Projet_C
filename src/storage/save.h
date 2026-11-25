#ifndef AUTOTRANSLATE_SAVE_H
#define AUTOTRANSLATE_SAVE_H

int save_translation(const char *original, const char *translated, const char *source_language, const char *target_language);

int export_history_to_txt(void);

#endif