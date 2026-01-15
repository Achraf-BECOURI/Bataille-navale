// Inclusion des bibliothèques nécessaires
#include <SDL3/SDL.h>   // Bibliothèque graphique SDL3
#include "types.h"      // Définitions des types personnalisés (Player, AIState, etc.)
#include "utils.h"      // Fonctions utilitaires (myRand, isShipSunk, markShipSunk)
#include "ai.h"         // Déclarations des fonctions d'IA (prototypes)

// ============================================================================
// FONCTION : enemyEasy
// DESCRIPTION : IA de niveau facile - tire complètement au hasard
// PARAMÈTRES :
//   - target : pointeur vers le joueur cible (adversaire)
//   - ai     : pointeur vers l'état de l'IA (historique des tirs)
// ============================================================================
void enemyEasy(Player *target, AIState *ai) {
    int x, y;  // Coordonnées du tir
    
    // Boucle pour trouver une case non encore visée
    do {
        x = myRand() % GRID;  // Génère une coordonnée X aléatoire (0 à GRID-1)
        y = myRand() % GRID;  // Génère une coordonnée Y aléatoire (0 à GRID-1)
    } while (ai->shots[x][y]);  // Continue tant que la case a déjà été visée

    // Marque cette case comme ayant été visée
    ai->shots[x][y] = 1;

    // Vérifie ce qu'il y a dans la case cible
    if (target->grid[x][y] > 0) {
        // CASE CONTIENT UN BATEAU INTACT (>0)
        int shipId = target->grid[x][y];  // Récupère l'ID du bateau touché
        target->grid[x][y] = -shipId;     // Marque comme touché (valeur négative)

        // Vérifie si le bateau est maintenant complètement coulé
        if (isShipSunk(target->grid, shipId))
            markShipSunk(target->grid, shipId);  // Marque toutes ses cases comme coulées
    }
    else if (target->grid[x][y] == 0) {
        // CASE CONTIENT DE L'EAU (0)
        target->grid[x][y] = -99;  // Marque comme tir manqué dans l'eau
    }
    // Si target->grid[x][y] < 0, c'est déjà touché/manqué, rien à faire
}

// ============================================================================
// FONCTION : enemyMedium
// DESCRIPTION : IA de niveau moyen - tire au hasard, mais poursuit si touché
// STRATÉGIE : Si un bateau a été touché, essaye les cases adjacentes
// PARAMÈTRES :
//   - target : pointeur vers le joueur cible
//   - ai     : pointeur vers l'état de l'IA
// ============================================================================
void enemyMedium(Player *target, AIState *ai) {
    int x, y;  // Coordonnées du tir

    // ÉTAPE 1 : Si on a touché un bateau au tour précédent
    if (ai->lastHitX != -1) {  // -1 signifie "pas de dernière touche"
        // Tableaux des déplacements dans les 4 directions
        int dx[4] = {-1, 1, 0, 0};  // Déplacements en X : haut, bas, gauche, droite
        int dy[4] = {0, 0, -1, 1};  // Déplacements en Y : haut, bas, gauche, droite

        // Essaye les 4 cases adjacentes à la dernière touche
        for (int i = 0; i < 4; i++) {
            x = ai->lastHitX + dx[i];  // Calcule la coordonnée X adjacente
            y = ai->lastHitY + dy[i];  // Calcule la coordonnée Y adjacente

            // Vérifie si la case est dans la grille et non encore visée
            if (x >= 0 && x < GRID && y >= 0 && y < GRID && ai->shots[x][y] == 0) {
                ai->shots[x][y] = 1;  // Marque comme visée

                if (target->grid[x][y] > 0) {
                    // TOUCHÉ : Case contient un bateau intact
                    int shipId = target->grid[x][y];  // ID du bateau
                    target->grid[x][y] = -shipId;     // Marque comme touché
                    
                    // Mémorise la nouvelle position de touche
                    ai->lastHitX = x;
                    ai->lastHitY = y;

                    // Vérifie si le bateau est maintenant coulé
                    if (isShipSunk(target->grid, shipId)) {
                        markShipSunk(target->grid, shipId);  // Marque comme coulé
                        ai->lastHitX = -1;  // Réinitialise pour chercher un nouveau bateau
                        ai->lastHitY = -1;
                    }
                } else {
                    // MANQUÉ : Case contient de l'eau ou déjà touché
                    target->grid[x][y] = -99;  // Marque comme eau touchée
                }
                return;  // Tir effectué, on quitte la fonction
            }
        }

        // Si on arrive ici, aucune case adjacente valide n'a été trouvée
        ai->lastHitX = -1;  // Réinitialise la dernière touche
        ai->lastHitY = -1;
    }

    // ÉTAPE 2 : Aucune dernière touche ou adjacents invalides → tire au hasard
    do {
        x = myRand() % GRID;  // Coordonnée X aléatoire
        y = myRand() % GRID;  // Coordonnée Y aléatoire
    } while (ai->shots[x][y]);  // Continue si case déjà visée

    ai->shots[x][y] = 1;  // Marque comme visée

    if (target->grid[x][y] > 0) {
        // TOUCHÉ
        int shipId = target->grid[x][y];  // ID du bateau
        target->grid[x][y] = -shipId;     // Marque comme touché
        
        // Mémorise cette touche pour le prochain tour
        ai->lastHitX = x;
        ai->lastHitY = y;

        // Vérifie si le bateau est coulé
        if (isShipSunk(target->grid, shipId)) {
            markShipSunk(target->grid, shipId);  // Marque comme coulé
            ai->lastHitX = -1;  // Réinitialise
            ai->lastHitY = -1;
        }
    } else {
        // MANQUÉ
        target->grid[x][y] = -99;  // Marque comme eau touchée
    }
}

// ============================================================================
// FONCTION : enemyHard
// DESCRIPTION : IA de niveau difficile - stratégie optimisée avec recherche directionnelle
// STRATÉGIE : Cherche en damier, puis suit la direction d'un bateau touché
// PARAMÈTRES :
//   - target : pointeur vers le joueur cible
//   - ai     : pointeur vers l'état de l'IA
// ============================================================================
void enemyHard(Player *target, AIState *ai) {
    // Tableaux des déplacements (index 0 inutilisé, 1-4 pour les directions)
    int dx[5] = {0, -1, 1, 0, 0};  // 0:rien, 1:haut, 2:bas, 3:gauche, 4:droite
    int dy[5] = {0, 0, 0, -1, 1};  // 0:rien, 1:haut, 2:bas, 3:gauche, 4:droite
    int x, y;  // Coordonnées du tir

    // ÉTAPE 1 : Si on a déjà une direction de recherche active
    if (ai->direction != 0) {
        // Continue dans la même direction à partir de la position actuelle
        x = ai->currentX + dx[ai->direction];  // Avance dans la direction
        y = ai->currentY + dy[ai->direction];  // Avance dans la direction

        // Vérifie si la case est valide et non visée
        if (x >= 0 && x < GRID && y >= 0 && y < GRID && ai->shots[x][y] == 0) {
            ai->shots[x][y] = 1;  // Marque comme visée

            if (target->grid[x][y] > 0) {
                // TOUCHÉ : continue dans la même direction
                int shipId = target->grid[x][y];  // ID du bateau
                target->grid[x][y] = -shipId;     // Marque comme touché
                
                // Met à jour la position courante
                ai->currentX = x;
                ai->currentY = y;

                // Vérifie si le bateau est coulé
                if (isShipSunk(target->grid, shipId)) {
                    markShipSunk(target->grid, shipId);  // Marque comme coulé
                    // Réinitialise complètement l'état de recherche
                    ai->anchorX = ai->anchorY = -1;
                    ai->currentX = ai->currentY = -1;
                    ai->direction = 0;
                }
            } else {
                // MANQUÉ : retourne au point d'ancrage et change de direction
                target->grid[x][y] = -99;  // Marque comme eau touchée
                ai->currentX = ai->anchorX;  // Revient au point de départ
                ai->currentY = ai->anchorY;
                ai->direction = 0;  // Arrête la recherche dans cette direction
            }
            return;  // Tir effectué
        } else {
            // Case invalide (hors grille ou déjà visée)
            ai->currentX = ai->anchorX;  // Revient au point d'ancrage
            ai->currentY = ai->anchorY;
            ai->direction = 0;  // Arrête la recherche
        }
    }

    // ÉTAPE 2 : Si on a un point d'ancrage (première touche d'un bateau)
    if (ai->anchorX != -1) {
        int shotMade = 0;  // Flag pour savoir si un tir a été effectué

        // Essaye les 4 directions autour du point d'ancrage
        for (int dir = 1; dir <= 4 && !shotMade; dir++) {
            x = ai->anchorX + dx[dir];  // Case dans la direction 'dir'
            y = ai->anchorY + dy[dir];

            // Vérifie si la case est valide et non visée
            if (x >= 0 && x < GRID && y >= 0 && y < GRID && ai->shots[x][y] == 0) {
                ai->shots[x][y] = 1;  // Marque comme visée

                if (target->grid[x][y] > 0) {
                    // TOUCHÉ : définit la direction et poursuit
                    int shipId = target->grid[x][y];  // ID du bateau
                    target->grid[x][y] = -shipId;     // Marque comme touché
                    
                    // Configure la recherche directionnelle
                    ai->direction = dir;     // Direction dans laquelle on a touché
                    ai->currentX = x;        // Position actuelle
                    ai->currentY = y;

                    // Vérifie si le bateau est coulé (cas improbable avec une seule touche)
                    if (isShipSunk(target->grid, shipId)) {
                        markShipSunk(target->grid, shipId);  // Marque comme coulé
                        // Réinitialise tout
                        ai->anchorX = ai->anchorY = -1;
                        ai->currentX = ai->currentY = -1;
                        ai->direction = 0;
                    }
                } else {
                    // MANQUÉ
                    target->grid[x][y] = -99;  // Marque comme eau touchée
                }

                shotMade = 1;  // Un tir a été effectué
            }
        }

        // Si aucun tir n'a pu être fait autour du point d'ancrage
        if (!shotMade) {
            // Cherche d'autres bateaux touchés mais non coulés dans toute la grille
            for (int i = 0; i < GRID && !shotMade; i++) {
                for (int j = 0; j < GRID && !shotMade; j++) {
                    // Case touchée mais bateau pas encore coulé (valeur entre -1 et -99)
                    if (target->grid[i][j] < 0 && target->grid[i][j] > -99) {
                        // Essaye les 4 directions autour de cette case touchée
                        for (int dir = 1; dir <= 4 && !shotMade; dir++) {
                            x = i + dx[dir];  // Case adjacente
                            y = j + dy[dir];
                            
                            // Vérifie si la case est valide et non visée
                            if (x >= 0 && x < GRID && y >= 0 && y < GRID && ai->shots[x][y] == 0) {
                                ai->shots[x][y] = 1;  // Marque comme visée

                                if (target->grid[x][y] > 0) {
                                    // TOUCHÉ : configure un nouveau point d'ancrage
                                    int shipId = target->grid[x][y];  // ID du bateau
                                    target->grid[x][y] = -shipId;     // Marque comme touché
                                    
                                    // Définit un nouveau point d'ancrage et direction
                                    ai->anchorX = i;        // Position de la touche connue
                                    ai->anchorY = j;
                                    ai->currentX = x;       // Nouvelle position
                                    ai->currentY = y;
                                    ai->direction = dir;    // Direction du succès

                                    // Vérifie si le bateau est coulé
                                    if (isShipSunk(target->grid, shipId)) {
                                        markShipSunk(target->grid, shipId);  // Marque comme coulé
                                        // Réinitialise tout
                                        ai->anchorX = ai->anchorY = -1;
                                        ai->currentX = ai->currentY = -1;
                                        ai->direction = 0;
                                    }
                                } else {
                                    // MANQUÉ
                                    target->grid[x][y] = -99;  // Marque comme eau touchée
                                }

                                shotMade = 1;  // Un tir a été effectué
                            }
                        }
                    }
                }
            }
        }

        if (shotMade) return;  // Si un tir a été fait, on quitte
    }

    // ÉTAPE 3 : Aucune piste - stratégie de recherche en damier
    do {
        x = myRand() % GRID;  // Coordonnée X aléatoire
        y = myRand() % GRID;  // Coordonnée Y aléatoire
        // Continue tant que la case a déjà été visée OU n'est pas sur une case "paire" du damier
        // (x + y) % 2 != 0 signifie : privilégie les cases où (x+y) est pair
        // Cela crée un motif en damier pour optimiser la recherche
    } while (ai->shots[x][y] || (x + y) % 2 != 0);

    ai->shots[x][y] = 1;  // Marque comme visée

    if (target->grid[x][y] > 0) {
        // TOUCHÉ : établit un nouveau point d'ancrage
        int shipId = target->grid[x][y];  // ID du bateau
        target->grid[x][y] = -shipId;     // Marque comme touché
        
        // Initialise le point d'ancrage pour recherche future
        ai->anchorX = x;    // Point de départ pour recherche directionnelle
        ai->anchorY = y;
        ai->currentX = x;   // Position actuelle (identique à l'ancrage)
        ai->currentY = y;
        // Note : direction reste à 0, sera déterminée au prochain tour
    } else {
        // MANQUÉ
        target->grid[x][y] = -99;  // Marque comme eau touchée
    }
}