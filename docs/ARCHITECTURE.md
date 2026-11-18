# Architecture de l'éditeur AutoTranslate

`src/main.c` démarre `GtkApplication`, crée la fenêtre et y place l'éditeur
réutilisable de `src/ui/`.

`editor.c` construit l'écran et écoute le signal `changed` du `GtkTextBuffer`
source. Ce signal est envoyé chaque fois que le texte change. Le programme lit
alors le texte UTF-8 complet, demande au découpeur les phrases terminées, puis
met à jour l'aperçu en lecture seule et le compteur. Il ne touche jamais au
texte source. L'aperçu affiche un message d'attente ; aucun service de
traduction n'est appelé.

`sentence_parser.c` utilise les outils UTF-8 de GLib et ne dépend pas des
widgets GTK. Il renvoie les phrases finies par un point et ignore le point
entre deux chiffres (par exemple `3.14`). `make test` vérifie ce découpeur sans
ouvrir de fenêtre.

Pour le moment, un point marque la fin d'une phrase. Une abréviation peut donc
couper une phrase trop tôt. Les points de suspension sont ambigus : le premier
est pris comme fin de phrase. Les points d'interrogation et d'exclamation ne
sont pas encore reconnus.
