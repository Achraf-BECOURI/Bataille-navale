# Makefile pour Bataille Navale

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2 -I./include
LDFLAGS = -lSDL3 -lm

# Dossiers
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .

# Fichiers source et objets
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

# Nom de l'exécutable
TARGET = $(BIN_DIR)/bataille_navale

# Règle par défaut
all: $(TARGET)

# Création des dossiers nécessaires
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compilation de l'exécutable
$(TARGET): $(OBJ_DIR) $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "✅ Compilation réussie : $(TARGET)"

# Compilation des fichiers objets
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -rf $(OBJ_DIR)
	rm -f $(TARGET)
	@echo "🧹 Nettoyage effectué"

# Recompilation complète
rebuild: clean all

# Lancement du jeu
run: $(TARGET)
	./$(TARGET)

# Aide
help:
	@echo "Commandes disponibles :"
	@echo "  make        - Compile le projet"
	@echo "  make clean  - Supprime les fichiers compilés"
	@echo "  make rebuild- Recompile tout"
	@echo "  make run    - Compile et lance le jeu"
	@echo "  make help   - Affiche cette aide"

.PHONY: all clean rebuild run help
