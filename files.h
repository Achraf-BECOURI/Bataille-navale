// ========================================================================
// FICHIER : files.h (Files Header)
// DESCRIPTION : Déclaration des fonctions de gestion des fichiers de scores
//               pour le jeu de bataille navale
// ========================================================================

// Directive de préprocesseur pour éviter les inclusions multiples
// Si FILES_H n'est pas défini, définir FILES_H et inclure le contenu
#ifndef FILES_H
#define FILES_H

// Inclusion du fichier d'en-tête contenant les définitions de types
// Ce fichier contient probablement les définitions de Score, ScoreMode, etc.
#include "types.h"

// ========================================================================
// DÉCLARATIONS DES FONCTIONS DE GESTION DES FICHIERS
// ========================================================================

// Fonction : getScoreTag
// Description : Convertit un mode de score en chaîne de caractères (étiquette)
// Paramètres :
//   - mode : le mode de difficulté (ScoreMode enum)
// Retour : Chaîne de caractères constante représentant le mode
const char *getScoreTag(ScoreMode mode);

// Fonction : saveScore
// Description : Sauvegarde un score dans un fichier au format simple
// Paramètres :
//   - filename   : nom du fichier où sauvegarder
//   - playerName : nom du joueur
//   - score      : score à sauvegarder
// Retour : aucun (void)
void saveScore(const char *filename, const char *playerName, int score);

// Fonction : loadScores
// Description : Charge et affiche tous les scores d'un fichier (version console)
// Paramètres :
//   - filename : nom du fichier à lire
// Retour : aucun (void)
void loadScores(const char *filename);

// Fonction : loadScoresSection
// Description : Charge les scores d'une section spécifique (difficulté) depuis un fichier structuré
// Paramètres :
//   - filename : nom du fichier à lire
//   - mode     : mode de difficulté (détermine quelle section charger)
//   - scores   : tableau où stocker les scores chargés
// Retour : nombre de scores chargés (int)
int loadScoresSection(const char *filename, ScoreMode mode, Score scores[]);

// Fonction : sortScores
// Description : Trie un tableau de scores par ordre décroissant (tri à bulles)
// Paramètres :
//   - scores : tableau de scores à trier
//   - count  : nombre d'éléments dans le tableau
// Retour : aucun (void)
void sortScores(Score scores[], int count);

// Fonction : writeAllScores
// Description : Écrit tous les scores dans un fichier structuré (toutes difficultés)
// Paramètres :
//   - filename : nom du fichier où écrire
//   - easy[]   : tableau des scores en mode facile
//   - ne       : nombre de scores en mode facile
//   - medium[] : tableau des scores en mode moyen
//   - nm       : nombre de scores en mode moyen
//   - hard[]   : tableau des scores en mode difficile
//   - nh       : nombre de scores en mode difficile
//   - pvp[]    : tableau des scores en mode PVP
//   - np       : nombre de scores en mode PVP
// Retour : aucun (void)
void writeAllScores(const char *filename,
                    Score easy[], int ne,
                    Score medium[], int nm,
                    Score hard[], int nh,
                    Score pvp[], int np);

// Fonction : updateTopScoresOneFile
// Description : Met à jour les meilleurs scores dans un fichier unique
//               Ajoute un nouveau score, trie et limite au nombre maximum
// Paramètres :
//   - filename    : nom du fichier de scores
//   - mode        : mode de difficulté du nouveau score
//   - playerName  : nom du joueur
//   - playerScore : score du joueur
// Retour : aucun (void)
void updateTopScoresOneFile(const char *filename,
                            ScoreMode mode,
                            const char *playerName,
                            int playerScore);

// Fonction : getBestScore
// Description : Récupère le meilleur score pour une difficulté donnée
// Paramètres :
//   - filename : nom du fichier de scores
//   - mode     : mode de difficulté
// Retour : le meilleur score (int), ou 0 si aucun score
int getBestScore(const char *filename, ScoreMode mode);

// Fin de la condition #ifndef FILES_H
#endif
// Note : Ce fichier d'en-tête contient uniquement les déclarations
// Les implémentations sont dans le fichier source correspondant