# AutoTranslate editor architecture

`src/main.c` initializes `GtkApplication`, creates the application window, and
sets its child to the reusable editor component in `src/ui/`.

`editor.c` owns the GTK layout and connects the editable source
`GtkTextBuffer`'s `changed` signal. Each edit reads the current full UTF-8 text,
asks the GTK independent parser for completed sentences, and refreshes the
read-only preview buffer and completed sentence count. It never writes to the
source buffer. The preview currently labels results as pending; it does not
call a translation service.

`sentence_parser.c` uses GLib UTF-8 helpers and has no GTK widget dependency.
It returns completed sentences ending in a period and ignores periods between
two ASCII digits, so values such as `3.14` remain within one sentence. Parser
tests run without opening a window through `make test`.

The MVP treats periods as sentence boundaries. Abbreviations can produce
premature boundaries, and ellipses are ambiguous: their first period is
currently treated as the boundary. Exclamation and question marks are not
boundaries yet.
