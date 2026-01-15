// Directive pour SDL : indique que le programme gère lui-même la fonction main()
#define SDL_MAIN_HANDLED

// Inclusions des bibliothèques standard
#include <stdio.h>      // Pour printf, scanf, fprintf, etc.
#include <SDL3/SDL.h>   // Bibliothèque graphique SDL3

// Inclusions des fichiers d'en-tête personnalisés
#include "types.h"      // Définitions des structures et types
#include "random.h"     // Fonctions de génération aléatoire
#include "render.h"     // Fonctions de rendu graphique
#include "ai.h"         // Fonctions d'intelligence artificielle
#include "files.h"      // Fonctions de gestion des fichiers de scores
#include "utils.h"      // Fonctions utilitaires diverses

// ============================================================================
// FONCTION PRINCIPALE : main
// DESCRIPTION : Point d'entrée du programme, gère le déroulement complet du jeu
// RETOUR : Code de sortie (0 = succès, 1 = erreur)
// ============================================================================
int main() {
    int mode;  // Variable pour stocker le mode de jeu choisi (1-4)

    // ===== AFFICHAGE DU MENU PRINCIPAL =====
    printf("1 : Joueur vs Joueur\n");
    printf("2 : Joueur vs IA (Facile)\n");
    printf("3 : Joueur vs IA (Moyen)\n");
    printf("4 : Joueur vs IA (Difficile)\n");
    printf("Choix : ");
    scanf("%d", &mode);  // Lecture du choix utilisateur

    // Vérification de la validité du mode choisi
    if (mode < 1 || mode > 4) {
        printf("Mode invalide. Fin du programme.\n");
        return 1;  // Retourne 1 pour indiquer une erreur
    }

    // ===== SAISIE DES NOMS DES JOUEURS =====
    char player1Name[50];  // Buffer pour le nom du joueur 1 (49 caractères max + \0)
    char player2Name[50];  // Buffer pour le nom du joueur 2 ou IA

    if (mode == 1) {
        // Mode PVP (Joueur contre Joueur)
        printf("\n=== Configuration des joueurs ===\n");
        printf("Entrez le nom du Joueur 1 : ");
        scanf("%49s", player1Name);  // Lecture sécurisée (49 caractères max)
        printf("Entrez le nom du Joueur 2 : ");
        scanf("%49s", player2Name);
        printf("\nBienvenue %s et %s !\n\n", player1Name, player2Name);
    } else {
        // Mode contre IA
        printf("\n=== Configuration du joueur ===\n");
        printf("Entrez votre nom : ");
        scanf("%49s", player1Name);
        printf("\nBienvenue %s !\n\n", player1Name);
        strcpy(player2Name, "IA");  // Définit le nom de l'IA comme "IA"
    }

    // ===== CHOIX DU MODE DE PLACEMENT DES BATEAUX =====
    printf("\nChoix du placement des bateaux :\n");
    printf("1 : Placement automatique\n");
    printf("2 : Placement manuel\n");
    printf("Choix : ");
    scanf("%d", &placementMode);  // Variable placementMode doit être déclarée ailleurs (globalement probablement)
    
    // Vérification de la validité du choix
    if (placementMode < 1 || placementMode > 2) {
        printf("Choix invalide. Fin du programme.\n");
        return 1;  // Retourne 1 pour indiquer une erreur
    }

    // ===== INITIALISATION DE SDL (SYSTÈME GRAPHIQUE) =====
    SDL_Init(SDL_INIT_VIDEO);  // Initialise SDL avec le sous-système vidéo
    seed = SDL_GetTicks();     // Initialise la graine aléatoire avec le temps actuel (seed doit être globale)

    // Création de la fenêtre graphique
    graphics.window = SDL_CreateWindow("Bataille Navale", WIDTH, HEIGHT, 0);  // WIDTH et HEIGHT doivent être définis
    graphics.renderer = SDL_CreateRenderer(graphics.window, NULL);  // Création du renderer
    SDL_SetWindowPosition(graphics.window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);  // Centre la fenêtre

    // ===== INITIALISATION DES STRUCTURES DE JEU =====
    initPlayer(&player1);     // Initialise le joueur 1 (structures globales)
    initPlayer(&player2);     // Initialise le joueur 2/IA
    initAIState(&aiState);    // Initialise l'état de l'IA
    initGameState(&gameState); // Initialise l'état du jeu

    // ===== PHASE DE PLACEMENT DES BATEAUX =====
    if (placementMode == 1) {
        // Placement automatique des bateaux
        placeShips(player1.grid, &shipConfig);  // Place les bateaux du joueur 1
        placeShips(player2.grid, &shipConfig);  // Place les bateaux du joueur 2/IA
        
        if (mode == 1) {
            // Message pour le mode PVP
            char msg[256];
            snprintf(msg, sizeof(msg), 
                "%s (Joueur 1 - vert) VS %s (Joueur 2 - rouge)\n\n%s commence !\n\nCliquez pour continuer...",
                player1Name, player2Name, player1Name);
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Début de partie", msg, graphics.window);
        } else {
            // Message pour le mode solo contre IA
            char msg[256];
            snprintf(msg, sizeof(msg), "%s, c'est parti !\n\nCliquez pour continuer...", player1Name);
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Début de partie", msg, graphics.window);
        }
    } else {
        // Placement manuel des bateaux
        if (mode == 1) {
            // Mode PVP : les deux joueurs placent leurs bateaux
            char msg[256];
            snprintf(msg, sizeof(msg), 
                "%s,\nplacez vos bateaux avec clic gauche\nAppuyez sur V pour changer l'orientation",
                player1Name);
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Placement", msg, graphics.window);
            placeShipsSDL(&graphics, &player1, &shipConfig, 0);  // Placement joueur 1
            
            snprintf(msg, sizeof(msg), 
                "%s,\nplacez vos bateaux avec clic gauche\nAppuyez sur V pour changer l'orientation",
                player2Name);
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Placement", msg, graphics.window);
            placeShipsSDL(&graphics, &player2, &shipConfig, 1);  // Placement joueur 2
            
            snprintf(msg, sizeof(msg), 
                "%s (vert) VS %s (rouge)\n\n%s commence !\n\nCliquez pour continuer...",
                player1Name, player2Name, player1Name);
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Début de partie", msg, graphics.window);
        } else {
            // Mode solo : seul le joueur place, l'IA est placée automatiquement
            char msg[256];
            snprintf(msg, sizeof(msg), 
                "%s,\nplacez vos bateaux avec clic gauche\nAppuyez sur V pour changer l'orientation",
                player1Name);
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Placement", msg, graphics.window);
            placeShipsSDL(&graphics, &player1, &shipConfig, 0);  // Placement joueur
            placeShips(player2.grid, &shipConfig);  // Placement automatique IA
            
            snprintf(msg, sizeof(msg), "%s, c'est parti !\n\nCliquez pour continuer...", player1Name);
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Début de partie", msg, graphics.window);
        }
    }

    // ===== BOUCLE PRINCIPALE DU JEU =====
    int running = 1;  // Flag pour contrôler la boucle de jeu
    SDL_Event e;      // Structure pour stocker les événements SDL

    while (running) {
        // Traitement des événements SDL (clics souris, fermeture fenêtre, etc.)
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = 0;  // L'utilisateur a demandé à quitter
                break;
            }
        
            // Gestion des clics de souris (tirs)
            if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                // Vérifie si on est en attente d'un délai
                if ((gameState.waitingPvPDelay) && mode == 1)
                    break;  // En attente en mode PVP, ignore le clic

                if (gameState.waitingDelay && mode != 1)
                    break;  // En attente en mode IA, ignore le clic

                // En mode PVP, attend que le joueur confirme le changement de tour
                if (mode == 1 && gameState.waitingNextPlayer) {
                    gameState.waitingNextPlayer = 0;  // Commence le tour
                    break;
                }

                // Récupère les coordonnées du clic
                int mx = e.button.x;  // Coordonnée X de la souris
                int my = e.button.y;  // Coordonnée Y de la souris

                // Vérifie que le clic est dans la grille de droite (grille adverse)
                if (mx < GRID * CELL)
                    break;  // Clic dans la grille de gauche (sa propre grille)

                // Convertit les coordonnées écran en coordonnées grille
                int x = my / CELL;                    // Ligne dans la grille
                int y = (mx - GRID * CELL) / CELL;    // Colonne dans la grille de droite

                // Détermine le joueur cible et le joueur actuel
                Player *target = getOpponent(mode, &gameState);   // Joueur à attaquer
                Player *current = getCurrentPlayer(mode, &gameState); // Joueur qui tire

                // Traitement du tir
                if (target->grid[x][y] == 0) {
                    // Tir dans l'eau
                    target->grid[x][y] = -99;  // Marque comme manqué
                    current->score -= 1;       // Pénalité pour tir manqué
                }
                else if (target->grid[x][y] > 0) {
                    // Touché un bateau
                    int shipId = target->grid[x][y];  // ID du bateau touché
                    target->grid[x][y] = -shipId;     // Marque comme touché
                    current->score += 10;             // Bonus pour toucher
                    target->score -= 5;               // Pénalité pour l'adversaire

                    // Vérifie si le bateau est coulé
                    if (isShipSunk(target->grid, shipId)) {
                        markShipSunk(target->grid, shipId);  // Marque comme coulé
                        current->score += 20;  // Bonus supplémentaire pour couler
                        target->score -= 10;   // Pénalité supplémentaire
                    }
                }
                else {
                    // Case déjà visée (touchée ou manquée)
                    break;  // Ignore ce clic
                }

                // VÉRIFICATION DE LA VICTOIRE IMMÉDIATE
                if (mode == 1) {
                    // Mode PVP : vérifie si l'adversaire n'a plus de bateaux
                    if (allDestroyed(target->grid)) {
                        current->score += 100;  // Bonus de victoire
                        target->score -= 50;    // Pénalité de défaite
                        
                        char msg[256];
                        const char *winnerName = (gameState.currentPlayer == 0) ? player1Name : player2Name;
                        
                        snprintf(msg, sizeof(msg), "🏆 VICTOIRE DE %s ! 🏆", winnerName);
                        printf("Victoire de %s\n", winnerName);
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Fin de partie", msg, graphics.window);
                        
                        running = 0;  // Termine le jeu
                        break;
                    }
                    
                    // Pas de victoire, active le délai et prépare le changement de joueur
                    gameState.lastPlayerActionTime = SDL_GetTicks();
                    gameState.waitingPvPDelay = 1;
                } else {
                    // Mode IA : active le délai pour le tour de l'IA
                    gameState.lastActionTime = SDL_GetTicks();
                    gameState.waitingDelay = 1;
                }
            }
        }

        // Gestion du délai entre tours en mode PVP
        if (gameState.waitingPvPDelay && mode == 1) {
            if (SDL_GetTicks() - gameState.lastPlayerActionTime >= PVP_DELAY) {
                // Délai écoulé, change de joueur
                gameState.currentPlayer = 1 - gameState.currentPlayer;  // Alterne 0↔1
                gameState.waitingNextPlayer = 1;   // Affiche l'écran de changement
                gameState.waitingPvPDelay = 0;     // Désactive le délai
            }
        }

        // Gestion du délai pour le tour de l'IA
        if (gameState.waitingDelay && mode != 1) {
            if (SDL_GetTicks() - gameState.lastActionTime >= TURN_DELAY) {
                // Délai écoulé, l'IA joue selon le niveau de difficulté
                if (mode == 2) enemyEasy(&player1, &aiState);
                else if (mode == 3) enemyMedium(&player1, &aiState);
                else enemyHard(&player1, &aiState);

                gameState.waitingDelay = 0;  // Désactive le délai
            }
        }

        // VÉRIFICATION DE LA VICTOIRE APRÈS LE TOUR DE L'IA
        if (mode != 1) {
            // Vérifie si le joueur a gagné (a détruit tous les bateaux de l'IA)
            if (allDestroyed(player2.grid)) {
                player1.score += 100;  // Bonus de victoire
                char msg[256];
                snprintf(msg, sizeof(msg), "🏆 VICTOIRE DE %s ! 🏆\n\nVous avez vaincu l'IA !", player1Name);
                printf("Victoire de %s\n", player1Name);
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Fin de partie", msg, graphics.window);
                break;  // Quitte la boucle de jeu
            }
            // Vérifie si l'IA a gagné
            if (allDestroyed(player1.grid)) {
                player1.score -= 50;  // Pénalité de défaite
                char msg[256];
                snprintf(msg, sizeof(msg), "💀 DÉFAITE DE %s 💀\n\nL'IA a gagné !", player1Name);
                printf("Défaite de %s\n", player1Name);
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Fin de partie", msg, graphics.window);
                break;  // Quitte la boucle de jeu
            }
        }

        // ===== DESSIN DE L'INTERFACE GRAPHIQUE =====
        SDL_SetRenderDrawColor(graphics.renderer, 0, 0, 0, 255);  // Couleur noir pour effacer
        SDL_RenderClear(graphics.renderer);  // Efface l'écran

        if (mode == 1 && gameState.waitingNextPlayer) {
            // Mode PVP : affiche l'écran de changement de tour
            drawTurnScreen(&graphics, gameState.currentPlayer);
        } else {
            // Affiche les deux grilles de jeu
            if (mode == 1) {
                // Mode PVP
                Player *current = getCurrentPlayer(mode, &gameState);
                Player *opponent = getOpponent(mode, &gameState);
                drawGrid(&graphics, current->grid, 0, 1);          // Grille gauche (ses propres bateaux)
                drawGrid(&graphics, opponent->grid, GRID * CELL, 0); // Grille droite (bateaux adverses cachés)
            } else {
                // Mode IA
                drawGrid(&graphics, player1.grid, 0, 1);          // Grille gauche (joueur)
                drawGrid(&graphics, player2.grid, GRID * CELL, 0); // Grille droite (IA, cachée)
            }
        }

        // Dessine la ligne de séparation entre les deux grilles
        SDL_SetRenderDrawColor(graphics.renderer, 200, 200, 200, 255);  // Gris clair
        SDL_RenderLine(graphics.renderer, GRID * CELL - 1, 0, GRID * CELL - 1, HEIGHT);  // Ligne gauche
        SDL_RenderLine(graphics.renderer, GRID * CELL, 0, GRID * CELL, HEIGHT);          // Ligne droite

        // Met à jour l'affichage (swap buffers)
        SDL_RenderPresent(graphics.renderer);
        
        // Petite pause pour éviter une boucle trop rapide (~60 FPS)
        SDL_Delay(16);
    }

    // ===== AFFICHAGE DES RÉSULTATS FINAUX ET SCORES =====
    if (mode != 1) {
        // Mode solo contre IA
        ScoreMode scoreMode = getScoreModeFromGameMode(mode);  // Convertit le mode en ScoreMode
        int bestScore = getBestScore("scores.txt", scoreMode); // Récupère le meilleur score existant
        
        printf("Score final de %s : %d\n", player1Name, player1.score);
        printf("Meilleur score : %d\n", bestScore);
        
        char msg[512];
        if (player1.score > bestScore) {
            // Nouveau record battu !
            snprintf(msg, sizeof(msg), 
                "🎉 NOUVEAU RECORD DE %s ! 🎉\n\nVotre score : %d\nAncien record : %d\n\nFélicitations !", 
                player1Name, player1.score, bestScore);
        } else {
            snprintf(msg, sizeof(msg), 
                "Score final de %s : %d\n\nMeilleur score : %d", 
                player1Name, player1.score, bestScore);
        }
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Résultats", msg, graphics.window);
    } else {
        // Mode PVP
        ScoreMode scoreMode = getScoreModeFromGameMode(mode);
        int bestScore = getBestScore("scores.txt", scoreMode);
        
        printf("Score final de %s : %d\n", player1Name, player1.score);
        printf("Score final de %s : %d\n", player2Name, player2.score);
        printf("Meilleur score : %d\n", bestScore);
        
        char msg[512];
        int maxScore = (player1.score > player2.score) ? player1.score : player2.score;
        
        if (maxScore > bestScore) {
            // Nouveau record (avec le nom du détenteur)
            const char *recordHolder = (player1.score > player2.score) ? player1Name : player2Name;
            snprintf(msg, sizeof(msg), 
                "🎉 NOUVEAU RECORD DE %s ! 🎉\n\n%s : %d\n%s : %d\n\nAncien record : %d\n\nFélicitations !", 
                recordHolder, player1Name, player1.score, player2Name, player2.score, bestScore);
        } else {
            snprintf(msg, sizeof(msg), 
                "Scores finaux :\n\n%s : %d\n%s : %d\n\nMeilleur score : %d", 
                player1Name, player1.score, player2Name, player2.score, bestScore);
        }
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Résultats", msg, graphics.window);
    }

    // ===== ENREGISTREMENT DES SCORES DANS LES FICHIERS =====
    if (mode == 1) {
        // Mode PVP : enregistre les scores des deux joueurs
        ScoreMode scoreMode = getScoreModeFromGameMode(mode);
        updateTopScoresOneFile("scores.txt", scoreMode, player1Name, player1.score);
        updateTopScoresOneFile("scores.txt", scoreMode, player2Name, player2.score);
        
        printf("\nScores enregistrés pour %s et %s !\n", player1Name, player2Name);
    } else {
        // Mode solo : enregistre seulement le score du joueur
        ScoreMode scoreMode = getScoreModeFromGameMode(mode);
        updateTopScoresOneFile("scores.txt", scoreMode, player1Name, player1.score);
        
        printf("\nScore enregistré pour %s !\n", player1Name);
    }

    // ===== NETTOYAGE ET FERMETURE =====
    SDL_DestroyRenderer(graphics.renderer);  // Libère le renderer
    SDL_DestroyWindow(graphics.window);      // Ferme la fenêtre
    SDL_Quit();                              // Quitte SDL
    
    return 0;  // Retourne 0 pour indiquer une exécution réussie
}