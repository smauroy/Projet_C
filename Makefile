

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

# Déclare les cibles qui ne représentent pas des fichiers.
.PHONY: all run check test clean

# Compile le programme par défaut.
all: $(TARGET)

# Construit l'exécutable à partir du fichier source.
$(TARGET): $(SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(SRC) -o $@ $(GTK_LIBS)

# Compile puis lance le programme.
run: all
	./$(TARGET)

# Vérifie la syntaxe du code sans créer d'exécutable.
check:
	$(CC) $(CFLAGS) $(GTK_CFLAGS) -fsyntax-only $(SRC)

# Effectue la vérification syntaxique du programme.
test: check
	@echo "Vérification syntaxique réussie. Tests unitaires non encore implémentés."

# Supprime les fichiers générés lors de la compilation.
clean:
	rm -rf build

