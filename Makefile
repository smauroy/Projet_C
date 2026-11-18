

# Compilateur C utilisé pour construire le programme.
CC = gcc

# Options de compilation :
# -std=c11    : utilise la norme C11.
# -Wall       : active les avertissements courants.
# -Wextra     : active des avertissements supplémentaires.
# -Wpedantic  : vérifie le respect strict de la norme C.
# -g          : ajoute les informations de débogage.
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -g

# Récupère les options de compilation nécessaires pour GTK4.
GTK_CFLAGS = $(shell pkg-config --cflags gtk4)

# Récupère les bibliothèques nécessaires pour GTK4.
GTK_LIBS = $(shell pkg-config --libs gtk4)
GLIB_CFLAGS = $(shell pkg-config --cflags glib-2.0)
GLIB_LIBS = $(shell pkg-config --libs glib-2.0)

# Détecte le système d'exploitation.
# Sous Windows, l'exécutable possède l'extension .exe.
# Sous Linux, aucune extension n'est nécessaire.
ifeq ($(OS),Windows_NT)
    EXT = .exe
else
    EXT =
endif

# Définit le chemin et le nom du programme exécutable.
TARGET = build/autotranslate$(EXT)

# Définit le fichier source principal du programme.
SRC = src/main.c
APP_SOURCES = src/main.c src/ui/editor.c src/core/sentence_parser.c
PARSER_TEST = build/sentence_parser_test$(EXT)

# Déclare les cibles qui ne représentent pas des fichiers.
.PHONY: all run check test clean

# Compile le programme par défaut.
all: $(TARGET)

# Construit l'exécutable à partir du fichier source.
$(TARGET): $(APP_SOURCES)
	@mkdir -p build
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(APP_SOURCES) -o $@ $(GTK_LIBS)

# Compile puis lance le programme.
run: all
	./$(TARGET)

# Vérifie la syntaxe du code sans créer d'exécutable.
check:
	$(CC) $(CFLAGS) $(GTK_CFLAGS) -Isrc -fsyntax-only $(APP_SOURCES)
	$(CC) $(CFLAGS) $(GLIB_CFLAGS) -Isrc -fsyntax-only src/core/sentence_parser.c tests/sentence_parser_test.c

# Compile les tests du découpeur et les lance sans ouvrir de fenêtre.
$(PARSER_TEST): src/core/sentence_parser.c src/core/sentence_parser.h tests/sentence_parser_test.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(GLIB_CFLAGS) -Isrc src/core/sentence_parser.c tests/sentence_parser_test.c -o $@ $(GLIB_LIBS)

test: check $(PARSER_TEST)
	./$(PARSER_TEST)

# Supprime les fichiers générés lors de la compilation.
clean:
	rm -rf build

