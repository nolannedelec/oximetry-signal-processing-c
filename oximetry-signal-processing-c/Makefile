CC      = gcc
CFLAGS  = -Iinc -Wall
LDFLAGS = -lm

# Liste des fichiers communs aux deux programmes
COMMON  = src/fir.o src/iir.o src/mesure.o src/affichage.o src/fichiers.o

# Cible par défaut : tout compiler
all: simulation autotests

# 1. Création de l'exécutable simulation
simulation: src/main_sim.o $(COMMON)
	$(CC) -o simulation $^ $(LDFLAGS)

# 2. Création de l'exécutable autotests
autotests: src/autotests.o src/test.o $(COMMON)
	$(CC) -o autotests $^ $(LDFLAGS)

# Règle automatique pour compiler n'importe quel .c dans src
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -f src/*.o simulation autotests *.exe