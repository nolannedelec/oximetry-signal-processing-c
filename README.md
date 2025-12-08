# ❤️ Traitement du Signal pour Oxymètre de Pouls (C)

> **Projet d'Ingénierie Logicielle & Traitement du Signal**
> Implémentation d'une chaîne complète d'analyse de signaux physiologiques (SpO2 / BPM) en langage C standard.

Ce projet simule la partie logicielle d'un oxymètre de pouls. Il ingère des données brutes (simulant des capteurs optiques) et applique une série de filtres numériques pour extraire la fréquence cardiaque et le taux d'oxygène dans le sang.

---

### Architecture du Traitement
Le pipeline de traitement est conçu pour être modulaire et performant :

1.  **Acquisition (Simulation) :** Lecture de fichiers binaires `.dat` contenant les échantillons bruts (Absorbance Rouge/Infra-rouge).
2.  **Filtrage FIR (Finite Impulse Response) :** Élimination du bruit haute fréquence pour nettoyer le signal brut.
3.  **Filtrage IIR (Infinite Impulse Response) :** Suppression de la composante continue (DC) pour isoler le signal pulsatile (AC).
4.  **Algorithme de Décision :**
    * Détection des pics et des passages par zéro.
    * Calcul du **BPM** (Battements Par Minute).
    * Calcul du **SpO2** (Saturation Pulsée en Oxygène).

📄 **Documentation Technique :** [Voir la présentation détaillée du projet (PDF)](docs/projet_oxymetrie_info_presentation.pdf)

---

### Structure du Projet
Le projet suit une structure MVC adaptée au C, séparant clairement les interfaces et l'implémentation :

```text
.
├── Makefile          # Script de compilation automatisé
├── src/              # Codes sources (.c) - Main, Filtres, IO
├── inc/              # En-têtes (.h) - Prototypes et Structures
├── data/             # Fichiers de données brutes (.dat)
└── docs/             # Documentation technique (PDF) 
```
---

### Stack Technique

* **Langage :** C (Standard ANSI)
* **Build System :** GNU Make (Makefile optimisé avec gestion des dépendances)
* **Mathématiques :** Filtres numériques, Intégration, Gestion de buffers circulaires.
* **Visualisation :** Sortie compatible avec Gnuplot pour l'affichage des courbes.

---

### Installation & Utilisation

#### 1. Pré-requis
* Un compilateur C (GCC recommandé).
* L'outil `make` installé sur votre terminal.

#### 2. Compilation
Placez-vous à la racine du projet et lancez la commande :
```bash
make
```
Cela génère deux exécutables à la racine : simulation (le programme principal) et autotests (pour la validation).

### 3. Lancer la simulation
```bash
./simulation
```
Le programme lira automatiquement les fichiers situés dans le dossier data/ et affichera les résultats (BPM/SpO2) dans la console

### 4. Lancer les tests unitaires
```bash
./autotests
```
Permet de vérifier le bon fonctionnement des briques logicielles (filtres, lectures fichiers) indépendamment du programme principal.

### 5. Nettoyage
Pour supprimer les fichiers objets (.o) et les exécutables et repartir sur une base saine :
```bash
make clean
```
---
_Développé dans le cadre du module Informatique & Électronique de l'ISEN Brest._
