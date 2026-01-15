// Inclusion de la bibliothèque SDL3 (déclarations des types SDL)
#include <SDL3/SDL.h>

// Directive de préprocesseur pour éviter les inclusions multiples
// Si RENDER_H n'est pas défini, définir RENDER_H et inclure le contenu
#ifndef RENDER_H
#define RENDER_H

// Inclusion du fichier d'en-tête contenant les définitions de types personnalisés
// Ce fichier contient les définitions de GraphicsContext, Player, ShipConfig, etc.
#include "types.h"

// ========================================================================
// DÉCLARATIONS DES FONCTIONS DE DESSIN PRIMITIVES ET EFFETS
// ========================================================================

// Fonction : drawGradientRect
// Description : Dessine un rectangle avec dégradé de couleur linéaire vertical
// Paramètres :
//   - renderer : renderer SDL pour dessiner
//   - x, y     : position du rectangle (coin supérieur gauche)
//   - w, h     : largeur et hauteur du rectangle
//   - r1,g1,b1 : couleur RGB du haut du dégradé
//   - r2,g2,b2 : couleur RGB du bas du dégradé
// Retour : aucun (void)
void drawGradientRect(SDL_Renderer *renderer, int x, int y, int w, int h, 
                      int r1, int g1, int b1, int r2, int g2, int b2);

// Fonction : drawFilledCircle
// Description : Dessine un cercle rempli (algorithme de remplissage par balayage)
// Paramètres :
//   - renderer : renderer SDL pour dessiner
//   - cx, cy   : centre du cercle (coordonnées X, Y)
//   - radius   : rayon du cercle
// Retour : aucun (void)
void drawFilledCircle(SDL_Renderer *renderer, int cx, int cy, int radius);

// Fonction : drawCircleOutline
// Description : Dessine le contour d'un cercle (algorithme de tracé de cercle)
// Paramètres :
//   - renderer : renderer SDL pour dessiner
//   - cx, cy   : centre du cercle (coordonnées X, Y)
//   - radius   : rayon du cercle
// Retour : aucun (void)
void drawCircleOutline(SDL_Renderer *renderer, int cx, int cy, int radius);

// Fonction : drawWaves
// Description : Dessine des vagues animées pour simuler l'effet d'eau
// Paramètres :
//   - renderer : renderer SDL pour dessiner
//   - x, y     : position de la zone d'eau
//   - w, h     : largeur et hauteur de la zone
//   - time     : temps actuel (pour l'animation)
// Retour : aucun (void)
void drawWaves(SDL_Renderer *renderer, int x, int y, int w, int h, Uint32 time);

// Fonction : drawExplosion
// Description : Dessine une explosion/impact avec effets visuels
// Paramètres :
//   - renderer : renderer SDL pour dessiner
//   - x, y     : centre de l'explosion
//   - size     : taille de l'explosion
// Retour : aucun (void)
void drawExplosion(SDL_Renderer *renderer, int x, int y, int size);

// Fonction : drawSmoke
// Description : Dessine de la fumée animée (pour les bateaux en feu)
// Paramètres :
//   - renderer : renderer SDL pour dessiner
//   - x, y     : position de la source de fumée
//   - time     : temps actuel (pour l'animation)
// Retour : aucun (void)
void drawSmoke(SDL_Renderer *renderer, int x, int y, Uint32 time);

// Fonction : drawRealisticShip
// Description : Dessine un bateau ultra-réaliste avec nombreux détails
// Paramètres :
//   - gfx        : contexte graphique (fenêtre + renderer)
//   - x, y       : position du bateau (coin supérieur gauche de la cellule)
//   - size       : taille de la cellule (CELL constant)
//   - isVertical : orientation (1 = vertical, 0 = horizontal)
//   - position   : position dans le bateau (0 = avant, length-1 = arrière)
//   - length     : longueur totale du bateau (nombre de cellules)
//   - shipId     : identifiant du bateau (pour les détails spécifiques)
// Retour : aucun (void)
void drawRealisticShip(GraphicsContext *gfx, int x, int y, int size, 
                       int isVertical, int position, int length, int shipId);

// Fonction : drawDamagedShip
// Description : Dessine un bateau endommagé avec effets réalistes (feu, fumée, trous)
// Paramètres :
//   - gfx   : contexte graphique
//   - x, y  : position du bateau
//   - size  : taille de la cellule
//   - time  : temps actuel (pour l'animation)
// Retour : aucun (void)
void drawDamagedShip(GraphicsContext *gfx, int x, int y, int size, Uint32 time);

// Fonction : drawSunkenShip
// Description : Dessine un bateau coulé avec effet de naufrage (sous l'eau)
// Paramètres :
//   - gfx   : contexte graphique
//   - x, y  : position du bateau
//   - size  : taille de la cellule
//   - time  : temps actuel (pour l'animation)
// Retour : aucun (void)
void drawSunkenShip(GraphicsContext *gfx, int x, int y, int size, Uint32 time);

// Fonction : drawMissedShot
// Description : Dessine un tir manqué avec splash réaliste dans l'eau
// Paramètres :
//   - gfx   : contexte graphique
//   - x, y  : position de l'impact
//   - size  : taille de la cellule
//   - time  : temps actuel (pour l'animation)
// Retour : aucun (void)
void drawMissedShot(GraphicsContext *gfx, int x, int y, int size, Uint32 time);

// ========================================================================
// DÉCLARATIONS DES FONCTIONS DE DESSIN DE L'INTERFACE
// ========================================================================

// Fonction : drawGrid
// Description : Dessine une grille complète avec tous ses éléments
// Paramètres :
//   - gfx     : contexte graphique
//   - g       : grille à dessiner (tableau 2D de GRID x GRID)
//   - offsetX : décalage horizontal (pour la grille de droite)
//   - reveal  : flag pour révéler les bateaux intacts (1) ou les cacher (0)
// Retour : aucun (void)
void drawGrid(GraphicsContext *gfx, int g[GRID][GRID], int offsetX, int reveal);

// Fonction : drawTurnScreen
// Description : Dessine l'écran de changement de tour (mode PvP)
// Paramètres :
//   - gfx    : contexte graphique
//   - player : joueur dont c'est le tour (0 = joueur 1, 1 = joueur 2)
// Retour : aucun (void)
void drawTurnScreen(GraphicsContext *gfx, int player);

// Fonction : drawShipPreview
// Description : Dessine l'aperçu du bateau en cours de placement (à droite de l'écran)
// Paramètres :
//   - gfx       : contexte graphique
//   - shipSize  : taille du bateau (ou -10 pour 5x2)
//   - vertical  : orientation (1 = vertical, 0 = horizontal)
// Retour : aucun (void)
void drawShipPreview(GraphicsContext *gfx, int shipSize, int vertical);

// Fonction : drawShipGhost
// Description : Dessine le "fantôme" (aperçu translucide) du bateau pendant le placement manuel
// Paramètres :
//   - gfx       : contexte graphique
//   - grid      : grille actuelle du joueur
//   - mouseX    : position X de la souris
//   - mouseY    : position Y de la souris
//   - shipSize  : taille du bateau (ou -10 pour 5x2)
//   - vertical  : orientation (1 = vertical, 0 = horizontal)
// Retour : aucun (void)
void drawShipGhost(GraphicsContext *gfx, int grid[GRID][GRID], int mouseX, int mouseY,
                   int shipSize, int vertical);

// Fonction : placeShipsSDL
// Description : Gère le placement manuel des bateaux avec interface SDL
// Paramètres :
//   - gfx       : contexte graphique
//   - p         : joueur qui place ses bateaux
//   - config    : configuration des bateaux (tailles)
//   - playerNum : numéro du joueur (0 ou 1)
// Retour : aucun (void)
void placeShipsSDL(GraphicsContext *gfx, Player *p, ShipConfig *config, int playerNum);

// Fin de la condition #ifndef RENDER_H
#endif
// Note : Ce fichier d'en-tête contient uniquement les déclarations (prototypes)
// Les implémentations sont dans le fichier source correspondant (render.c)