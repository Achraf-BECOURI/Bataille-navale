// Inclusion des bibliothèques nécessaires
#include <SDL3/SDL.h>   // Bibliothèque graphique SDL3
#include "types.h"      // Définitions des types personnalisés (GraphicsContext, Player, etc.)
#include "utils.h"      // Fonctions utilitaires (canPlaceShip, canPlaceShip5x2Manual, etc.)
#include "render.h"     // Déclarations des fonctions de rendu (prototypes)

// ============================================================================
// FONCTION : drawGradientRect
// DESCRIPTION : Dessine un rectangle avec dégradé de couleur linéaire vertical
// PARAMÈTRES :
//   - renderer : renderer SDL pour dessiner
//   - x, y     : position du rectangle (coin supérieur gauche)
//   - w, h     : largeur et hauteur du rectangle
//   - r1,g1,b1 : couleur RGB du haut du dégradé
//   - r2,g2,b2 : couleur RGB du bas du dégradé
// RETOUR : aucun (void)
// ============================================================================
void drawGradientRect(SDL_Renderer *renderer, int x, int y, int w, int h, 
                      int r1, int g1, int b1, int r2, int g2, int b2) {
    // Parcourt chaque ligne horizontale du rectangle
    for (int i = 0; i < h; i++) {
        // Calcule le ratio de progression verticale (0 en haut, 1 en bas)
        float ratio = (float)i / h;
        
        // Interpolation linéaire des couleurs RGB selon le ratio
        int r = r1 + (int)((r2 - r1) * ratio);
        int g = g1 + (int)((g2 - g1) * ratio);
        int b = b1 + (int)((b2 - b1) * ratio);
        
        // Définit la couleur calculée
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        
        // Dessine une ligne horizontale à cette hauteur
        SDL_RenderLine(renderer, x, y + i, x + w, y + i);
    }
}

// ============================================================================
// FONCTION : drawFilledCircle
// DESCRIPTION : Dessine un cercle rempli (algorithme de remplissage par balayage)
// PARAMÈTRES :
//   - renderer : renderer SDL pour dessiner
//   - cx, cy   : centre du cercle (coordonnées X, Y)
//   - radius   : rayon du cercle
// RETOUR : aucun (void)
// ALGORITHME : Teste tous les points dans un carré englobant
// ============================================================================
void drawFilledCircle(SDL_Renderer *renderer, int cx, int cy, int radius) {
    // Parcourt un carré englobant le cercle (de -radius à +radius)
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            // Vérifie si le point (x,y) est à l'intérieur du cercle
            // Équation du cercle : x² + y² ≤ r²
            if (x*x + y*y <= radius*radius) {
                // Dessine le pixel à la position (cx + x, cy + y)
                SDL_RenderPoint(renderer, cx + x, cy + y);
            }
        }
    }
}

// ============================================================================
// FONCTION : drawCircleOutline
// DESCRIPTION : Dessine le contour d'un cercle (algorithme de tracé de cercle)
// PARAMÈTRES :
//   - renderer : renderer SDL pour dessiner
//   - cx, cy   : centre du cercle (coordonnées X, Y)
//   - radius   : rayon du cercle
// RETOUR : aucun (void)
// ALGORITHME : Algorithme de Bresenham pour les cercles
// ============================================================================
void drawCircleOutline(SDL_Renderer *renderer, int cx, int cy, int radius) {
    // Initialise les variables pour l'algorithme de Bresenham
    int x = radius;   // Position X initiale (à droite du centre)
    int y = 0;        // Position Y initiale (sur l'axe horizontal)
    int err = 0;      // Variable d'erreur pour décider du prochain pixel

    // Boucle principale : tant qu'on est dans l'octant (x ≥ y)
    while (x >= y) {
        // Dessine les 8 points symétriques du cercle (les 8 octants)
        SDL_RenderPoint(renderer, cx + x, cy + y);   // Octant 1
        SDL_RenderPoint(renderer, cx + y, cy + x);   // Octant 2
        SDL_RenderPoint(renderer, cx - y, cy + x);   // Octant 3
        SDL_RenderPoint(renderer, cx - x, cy + y);   // Octant 4
        SDL_RenderPoint(renderer, cx - x, cy - y);   // Octant 5
        SDL_RenderPoint(renderer, cx - y, cy - x);   // Octant 6
        SDL_RenderPoint(renderer, cx + y, cy - x);   // Octant 7
        SDL_RenderPoint(renderer, cx + x, cy - y);   // Octant 8

        // Logique de Bresenham pour décider du prochain pixel
        if (err <= 0) {
            y += 1;            // Avance verticalement
            err += 2*y + 1;    // Met à jour l'erreur
        }
        if (err > 0) {
            x -= 1;            // Recule horizontalement
            err -= 2*x + 1;    // Met à jour l'erreur
        }
    }
}

// ============================================================================
// FONCTION : drawWaves
// DESCRIPTION : Dessine des vagues animées pour simuler l'effet d'eau
// PARAMÈTRES :
//   - renderer : renderer SDL pour dessiner
//   - x, y     : position de la zone d'eau
//   - w, h     : largeur et hauteur de la zone
//   - time     : temps actuel (pour l'animation)
// RETOUR : aucun (void)
// ============================================================================
void drawWaves(SDL_Renderer *renderer, int x, int y, int w, int h, Uint32 time) {
    // Dessine un fond océan avec dégradé bleu (bleu foncé en haut → bleu clair en bas)
    drawGradientRect(renderer, x, y, w, h, 0, 100, 160, 0, 140, 200);
    
    // Active le mode de mélange (blending) pour la transparence
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    // Calcule un décalage basé sur le temps pour animer les vagues
    int offset = (time / 100) % 20;
    
    // Dessine 3 vagues parallèles
    for (int i = 0; i < 3; i++) {
        // Calcule la position Y de la vague (avec décalage pour l'animation)
        int waveY = y + h/4 + i * h/4 + offset - 10;
        
        // Définit la couleur des vagues (bleu clair semi-transparent)
        SDL_SetRenderDrawColor(renderer, 40, 160, 220, 80);
        
        // Crée un rectangle fin pour représenter la vague
        SDL_FRect wave = { x + 3, waveY, w - 6, 2 };
        
        // Dessine la vague
        SDL_RenderFillRect(renderer, &wave);
    }
    
    // Dessine des reflets lumineux (effet de soleil sur l'eau)
    SDL_SetRenderDrawColor(renderer, 100, 200, 255, 60);
    SDL_FRect shine = { x + w/4, y + h/3, w/2, 3 };
    SDL_RenderFillRect(renderer, &shine);
    
    // Désactive le mode de mélange
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

// ============================================================================
// FONCTION : drawExplosion
// DESCRIPTION : Dessine une explosion/impact avec effets visuels
// PARAMÈTRES :
//   - renderer : renderer SDL pour dessiner
//   - x, y     : centre de l'explosion
//   - size     : taille de l'explosion
// RETOUR : aucun (void)
// ============================================================================
void drawExplosion(SDL_Renderer *renderer, int x, int y, int size) {
    // Active le mode de mélange pour la transparence
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    // Cercle rouge central (noyau de l'explosion)
    SDL_SetRenderDrawColor(renderer, 255, 50, 0, 200);
    drawFilledCircle(renderer, x, y, size);
    
    // Cercle orange intermédiaire
    SDL_SetRenderDrawColor(renderer, 255, 120, 0, 150);
    drawFilledCircle(renderer, x, y, size + 3);
    
    // Cercle jaune externe
    SDL_SetRenderDrawColor(renderer, 255, 200, 0, 100);
    drawFilledCircle(renderer, x, y, size + 6);
    
    // Tableau des directions pour les particules de feu (8 directions principales)
    int directions[8][2] = {
        {1, 0}, {1, 1}, {0, 1}, {-1, 1},
        {-1, 0}, {-1, -1}, {0, -1}, {1, -1}
    };
    
    // Dessine des particules de feu dans les 8 directions
    for (int i = 0; i < 8; i++) {
        // Calcule la position de la particule
        int px = x + (size + 8) * directions[i][0];
        int py = y + (size + 8) * directions[i][1];
        
        // Définit la couleur de la particule (orange semi-transparent)
        SDL_SetRenderDrawColor(renderer, 255, 150, 0, 180);
        
        // Crée un petit carré pour représenter la particule
        SDL_FRect particle = { px - 2, py - 2, 4, 4 };
        
        // Dessine la particule
        SDL_RenderFillRect(renderer, &particle);
    }
    
    // Désactive le mode de mélange
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

// ============================================================================
// FONCTION : drawSmoke
// DESCRIPTION : Dessine de la fumée animée (pour les bateaux en feu)
// PARAMÈTRES :
//   - renderer : renderer SDL pour dessiner
//   - x, y     : position de la source de fumée
//   - time     : temps actuel (pour l'animation)
// RETOUR : aucun (void)
// ============================================================================
void drawSmoke(SDL_Renderer *renderer, int x, int y, Uint32 time) {
    // Active le mode de mélange pour la transparence
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    // Calcule un décalage basé sur le temps pour animer la fumée
    int offset = (time / 50) % 15;
    
    // Dessine 3 cercles de fumée empilés
    for (int i = 0; i < 3; i++) {
        // Calcule la position Y de la fumée (qui monte avec le temps)
        int smokeY = y - offset - i * 8;
        
        // Calcule la taille de la fumée (augmente avec la hauteur)
        int smokeSize = 6 + i * 2;
        
        // Définit la couleur de la fumée (gris de plus en plus transparent)
        SDL_SetRenderDrawColor(renderer, 80, 80, 80, 120 - i * 30);
        
        // Dessine le cercle de fumée
        drawFilledCircle(renderer, x, smokeY, smokeSize);
    }
    
    // Désactive le mode de mélange
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

// ============================================================================
// FONCTION : drawRealisticShip
// DESCRIPTION : Dessine un bateau ultra-réaliste avec nombreux détails
// PARAMÈTRES :
//   - gfx        : contexte graphique (fenêtre + renderer)
//   - x, y       : position du bateau (coin supérieur gauche de la cellule)
//   - size       : taille de la cellule (CELL constant)
//   - isVertical : orientation (1 = vertical, 0 = horizontal)
//   - position   : position dans le bateau (0 = avant, length-1 = arrière)
//   - length     : longueur totale du bateau (nombre de cellules)
//   - shipId     : identifiant du bateau (pour les détails spécifiques)
// RETOUR : aucun (void)
// ============================================================================
void drawRealisticShip(GraphicsContext *gfx, int x, int y, int size, 
                       int isVertical, int position, int length, int shipId) {
    // Définit le rectangle de la cellule (avec -1 pour la bordure)
    SDL_FRect cell = { x, y, size - 1, size - 1 };
    
    // Dessine un gradient métallique pour la coque (orientation affecte le dégradé)
    if (isVertical) {
        // Dégradé vertical : gris foncé en haut → gris clair en bas
        drawGradientRect(gfx->renderer, x, y, size - 1, size - 1, 
                        60, 70, 80, 90, 100, 110);
    } else {
        // Dégradé horizontal : gris clair à gauche → gris foncé à droite
        drawGradientRect(gfx->renderer, x, y, size - 1, size - 1,
                        90, 100, 110, 60, 70, 80);
    }
    
    // Dessine la bordure de la coque (gris foncé)
    SDL_SetRenderDrawColor(gfx->renderer, 40, 50, 60, 255);
    SDL_RenderRect(gfx->renderer, &cell);
    
    // Active le mode de mélange pour les effets 3D (ombres et lumières)
    SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_BLEND);
    
    // Ombre en bas/droite (effet de profondeur)
    SDL_SetRenderDrawColor(gfx->renderer, 0, 0, 0, 80);
    SDL_RenderLine(gfx->renderer, x, y + size - 2, x + size - 1, y + size - 2); // Bas
    SDL_RenderLine(gfx->renderer, x + size - 2, y, x + size - 2, y + size - 1); // Droite
    
    // Lumière en haut/gauche (effet d'éclairage)
    SDL_SetRenderDrawColor(gfx->renderer, 255, 255, 255, 60);
    SDL_RenderLine(gfx->renderer, x + 1, y + 1, x + size - 2, y + 1); // Haut
    SDL_RenderLine(gfx->renderer, x + 1, y + 1, x + 1, y + size - 2); // Gauche
    
    // Désactive le mode de mélange
    SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_NONE);
    
    // Détails selon la position dans le bateau
    if (isVertical) {
        // BATEAU VERTICAL
        // PROUE (avant du bateau)
        if (position == 0) {
            // Forme pointue caractéristique
            SDL_SetRenderDrawColor(gfx->renderer, 100, 110, 120, 255);
            for (int i = 0; i < size/3; i++) {
                SDL_RenderLine(gfx->renderer, 
                              x + size/2 - i/2, y + i,   // Point gauche
                              x + size/2 + i/2, y + i);  // Point droit
            }
            // Ancre
            SDL_SetRenderDrawColor(gfx->renderer, 180, 150, 100, 255);
            SDL_FRect anchor = { x + size/2 - 3, y + size/4, 6, 8 };
            SDL_RenderFillRect(gfx->renderer, &anchor);
            SDL_FRect anchorBottom = { x + size/2 - 5, y + size/4 + 6, 10, 3 };
            SDL_RenderFillRect(gfx->renderer, &anchorBottom);
        }
        // POUPE (arrière du bateau)
        else if (position == length - 1) {
            // Moteur/hélice
            SDL_SetRenderDrawColor(gfx->renderer, 50, 50, 50, 255);
            SDL_FRect engine = { x + size/3, y + size - size/3, size/3, size/4 };
            SDL_RenderFillRect(gfx->renderer, &engine);
            
            // Gouvernail
            SDL_SetRenderDrawColor(gfx->renderer, 70, 70, 70, 255);
            SDL_FRect rudder = { x + size/2 - 2, y + size - size/4, 4, size/5 };
            SDL_RenderFillRect(gfx->renderer, &rudder);
            
            // Hélices (représentées par des petits rectangles)
            SDL_SetRenderDrawColor(gfx->renderer, 90, 90, 90, 255);
            SDL_FRect prop1 = { x + size/3 - 2, y + size - size/4, 2, 6 };
            SDL_FRect prop2 = { x + 2*size/3, y + size - size/4, 2, 6 };
            SDL_RenderFillRect(gfx->renderer, &prop1);
            SDL_RenderFillRect(gfx->renderer, &prop2);
        }
        // MILIEU du bateau
        else {
            // Tour de contrôle (au milieu du bateau)
            if (position == length / 2) {
                SDL_SetRenderDrawColor(gfx->renderer, 80, 90, 100, 255);
                SDL_FRect tower = { x + size/3, y + size/4, size/3, size/2 };
                SDL_RenderFillRect(gfx->renderer, &tower);
                
                // Bordure de la tour
                SDL_SetRenderDrawColor(gfx->renderer, 100, 110, 120, 255);
                SDL_RenderRect(gfx->renderer, &tower);
                
                // Radar/antenne au sommet
                SDL_SetRenderDrawColor(gfx->renderer, 120, 130, 140, 255);
                SDL_RenderLine(gfx->renderer, x + size/2, y + size/4, x + size/2, y + 2);
                drawFilledCircle(gfx->renderer, x + size/2, y + 3, 3);  // Boule du radar
            }
            
            // Hublots illuminés (fenêtres)
            int numWindows = 2;
            for (int w = 0; w < numWindows; w++) {
                SDL_FRect window = { 
                    x + size/4 + w * (size/2),  // Position X
                    y + size/2 - 3,             // Position Y
                    5, 5                         // Taille
                };
                SDL_SetRenderDrawColor(gfx->renderer, 255, 240, 180, 255);  // Jaune lumineux
                SDL_RenderFillRect(gfx->renderer, &window);
                
                // Bordure de fenêtre
                SDL_SetRenderDrawColor(gfx->renderer, 100, 100, 100, 255);
                SDL_RenderRect(gfx->renderer, &window);
                
                // Reflet dans la fenêtre (effet de verre)
                SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(gfx->renderer, 255, 255, 255, 100);
                SDL_FRect reflection = { window.x + 1, window.y + 1, 2, 2 };
                SDL_RenderFillRect(gfx->renderer, &reflection);
                SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_NONE);
            }
            
            // Canons/tourelles (uniquement sur les grands bateaux, ID ≥ 4)
            if (shipId >= 4) {
                SDL_SetRenderDrawColor(gfx->renderer, 60, 60, 60, 255);
                SDL_FRect cannon1 = { x + 3, y + size/3, 4, size/4 };
                SDL_FRect cannon2 = { x + size - 7, y + size/3, 4, size/4 };
                SDL_RenderFillRect(gfx->renderer, &cannon1);
                SDL_RenderFillRect(gfx->renderer, &cannon2);
                
                // Barils des canons (qui dépassent)
                SDL_SetRenderDrawColor(gfx->renderer, 40, 40, 40, 255);
                SDL_FRect barrel1 = { x - 2, y + size/3 + 2, 5, 3 };
                SDL_FRect barrel2 = { x + size - 3, y + size/3 + 2, 5, 3 };
                SDL_RenderFillRect(gfx->renderer, &barrel1);
                SDL_RenderFillRect(gfx->renderer, &barrel2);
            }
            
            // Pont (deck) en bois
            SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(gfx->renderer, 100, 80, 60, 100);  // Brun transparent
            SDL_FRect deck = { x + 5, y + size/2 + 5, size - 10, 2 };
            SDL_RenderFillRect(gfx->renderer, &deck);
            SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_NONE);
        }
    } else {
        // BATEAU HORIZONTAL - mêmes effets adaptés à l'orientation
        
        // PROUE (gauche)
        if (position == 0) {
            SDL_SetRenderDrawColor(gfx->renderer, 100, 110, 120, 255);
            for (int i = 0; i < size/3; i++) {
                SDL_RenderLine(gfx->renderer, 
                              x + i, y + size/2 - i/2,   // Haut
                              x + i, y + size/2 + i/2);  // Bas
            }
            // Ancre horizontale
            SDL_SetRenderDrawColor(gfx->renderer, 180, 150, 100, 255);
            SDL_FRect anchor = { x + size/4, y + size/2 - 3, 8, 6 };
            SDL_RenderFillRect(gfx->renderer, &anchor);
            SDL_FRect anchorBottom = { x + size/4 + 6, y + size/2 - 5, 3, 10 };
            SDL_RenderFillRect(gfx->renderer, &anchorBottom);
        }
        // POUPE (droite)
        else if (position == length - 1) {
            // Moteur
            SDL_SetRenderDrawColor(gfx->renderer, 50, 50, 50, 255);
            SDL_FRect engine = { x + size - size/3, y + size/3, size/4, size/3 };
            SDL_RenderFillRect(gfx->renderer, &engine);
            
            // Hélices horizontales
            SDL_SetRenderDrawColor(gfx->renderer, 90, 90, 90, 255);
            SDL_FRect prop1 = { x + size - size/4, y + size/3 - 2, 6, 2 };
            SDL_FRect prop2 = { x + size - size/4, y + 2*size/3, 6, 2 };
            SDL_RenderFillRect(gfx->renderer, &prop1);
            SDL_RenderFillRect(gfx->renderer, &prop2);
        }
        // MILIEU horizontal
        else {
            // Tour de contrôle centrale
            if (position == length / 2) {
                SDL_SetRenderDrawColor(gfx->renderer, 80, 90, 100, 255);
                SDL_FRect tower = { x + size/4, y + size/3, size/2, size/3 };
                SDL_RenderFillRect(gfx->renderer, &tower);
                SDL_SetRenderDrawColor(gfx->renderer, 100, 110, 120, 255);
                SDL_RenderRect(gfx->renderer, &tower);
                
                // Radar
                SDL_SetRenderDrawColor(gfx->renderer, 120, 130, 140, 255);
                SDL_RenderLine(gfx->renderer, x + size/2, y + size/3, x + size/2, y + 2);
                drawFilledCircle(gfx->renderer, x + size/2, y + 3, 3);
            }
            
            // Hublots verticaux
            for (int w = 0; w < 2; w++) {
                SDL_FRect window = { 
                    x + size/2 - 3, 
                    y + size/4 + w * (size/2), 
                    5, 5 
                };
                SDL_SetRenderDrawColor(gfx->renderer, 255, 240, 180, 255);
                SDL_RenderFillRect(gfx->renderer, &window);
                SDL_SetRenderDrawColor(gfx->renderer, 100, 100, 100, 255);
                SDL_RenderRect(gfx->renderer, &window);
                
                SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(gfx->renderer, 255, 255, 255, 100);
                SDL_FRect reflection = { window.x + 1, window.y + 1, 2, 2 };
                SDL_RenderFillRect(gfx->renderer, &reflection);
                SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_NONE);
            }
            
            // Canons pour grands bateaux horizontaux
            if (shipId >= 4) {
                SDL_SetRenderDrawColor(gfx->renderer, 60, 60, 60, 255);
                SDL_FRect cannon1 = { x + size/3, y + 3, size/4, 4 };
                SDL_FRect cannon2 = { x + size/3, y + size - 7, size/4, 4 };
                SDL_RenderFillRect(gfx->renderer, &cannon1);
                SDL_RenderFillRect(gfx->renderer, &cannon2);
                
                SDL_SetRenderDrawColor(gfx->renderer, 40, 40, 40, 255);
                SDL_FRect barrel1 = { x + size/3 + 2, y - 2, 3, 5 };
                SDL_FRect barrel2 = { x + size/3 + 2, y + size - 3, 3, 5 };
                SDL_RenderFillRect(gfx->renderer, &barrel1);
                SDL_RenderFillRect(gfx->renderer, &barrel2);
            }
            
            // Pont horizontal
            SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(gfx->renderer, 100, 80, 60, 100);
            SDL_FRect deck = { x + size/2 + 5, y + 5, 2, size - 10 };
            SDL_RenderFillRect(gfx->renderer, &deck);
            SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_NONE);
        }
    }
    
    // Numéro d'identification du bateau (petite boîte)
    SDL_SetRenderDrawColor(gfx->renderer, 200, 200, 200, 255);
    SDL_FRect numBox = { x + size - 12, y + 2, 8, 8 };
    SDL_RenderRect(gfx->renderer, &numBox);
    
    // Lignes de rivetage sur la coque (effet métal)
    SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(gfx->renderer, 40, 40, 50, 150);
    for (int r = 0; r < 3; r++) {
        if (isVertical) {
            int lineY = y + size/4 + r * size/4;  // Ligne horizontale
            SDL_RenderLine(gfx->renderer, x + 3, lineY, x + size - 4, lineY);
        } else {
            int lineX = x + size/4 + r * size/4;  // Ligne verticale
            SDL_RenderLine(gfx->renderer, lineX, y + 3, lineX, y + size - 4);
        }
    }
    SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_NONE);
}

// ============================================================================
// FONCTION : drawDamagedShip
// DESCRIPTION : Dessine un bateau endommagé avec effets réalistes (feu, fumée, trous)
// PARAMÈTRES :
//   - gfx   : contexte graphique
//   - x, y  : position du bateau
//   - size  : taille de la cellule
//   - time  : temps actuel (pour l'animation)
// RETOUR : aucun (void)
// ============================================================================
void drawDamagedShip(GraphicsContext *gfx, int x, int y, int size, Uint32 time) {
    SDL_FRect cell = { x, y, size - 1, size - 1 };
    
    // Coque endommagée avec gradient rouge/orange
    drawGradientRect(gfx->renderer, x, y, size - 1, size - 1,
                    150, 40, 30, 100, 20, 10);
    
    // Bordure déchirée (rouge foncé)
    SDL_SetRenderDrawColor(gfx->renderer, 80, 20, 10, 255);
    SDL_RenderRect(gfx->renderer, &cell);
    
    // Trous de balles/impacts (noirs)
    SDL_SetRenderDrawColor(gfx->renderer, 0, 0, 0, 255);
    drawFilledCircle(gfx->renderer, x + size/3, y + size/3, 3);
    drawFilledCircle(gfx->renderer, x + 2*size/3, y + 2*size/3, 3);
    drawFilledCircle(gfx->renderer, x + size/2, y + size - size/4, 2);
    
    // Explosion active au centre
    drawExplosion(gfx->renderer, x + size/2, y + size/2, 8);
    
    // Fumée s'échappant
    drawSmoke(gfx->renderer, x + size/2, y + size/4, time);
    
    // Flammes animées (avec effet de scintillement)
    SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_BLEND);
    int flicker = (time / 100) % 3;  // Variation pour effet de scintillement
    SDL_SetRenderDrawColor(gfx->renderer, 255, 100 + flicker * 30, 0, 200);
    
    // Flammes principales
    SDL_FRect fire1 = { x + size/4, y + size/3, 8, 12 };
    SDL_FRect fire2 = { x + size - size/3, y + size/2, 6, 10 };
    SDL_RenderFillRect(gfx->renderer, &fire1);
    SDL_RenderFillRect(gfx->renderer, &fire2);
    
    // Pointes de flammes (plus claires)
    SDL_SetRenderDrawColor(gfx->renderer, 255, 200, 0, 180);
    SDL_FRect fire1Top = { x + size/4 + 2, y + size/3 - 4, 4, 5 };
    SDL_FRect fire2Top = { x + size - size/3 + 1, y + size/2 - 3, 4, 4 };
    SDL_RenderFillRect(gfx->renderer, &fire1Top);
    SDL_RenderFillRect(gfx->renderer, &fire2Top);
    
    SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_NONE);
    
    // Croix rouge de dégâts
    SDL_SetRenderDrawColor(gfx->renderer, 255, 0, 0, 255);
    for (int i = 0; i < 3; i++) {  // Épaisseur de 3 pixels
        SDL_RenderLine(gfx->renderer, x + 5 + i, y + 5, x + size - 6 + i, y + size - 6);  // Diagonale \
        SDL_RenderLine(gfx->renderer, x + size - 6 - i, y + 5, x + 5 - i, y + size - 6);  // Diagonale /
    }
    
    // Étincelles (effet animé)
    SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(gfx->renderer, 255, 255, 100, 200);
    int sparkOffset = (time / 80) % 4;  // Animation des étincelles
    SDL_RenderPoint(gfx->renderer, x + size/3 + sparkOffset, y + size/4);
    SDL_RenderPoint(gfx->renderer, x + 2*size/3 - sparkOffset, y + 3*size/4);
    SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_NONE);
}

// ============================================================================
// FONCTION : drawSunkenShip
// DESCRIPTION : Dessine un bateau coulé avec effet de naufrage (sous l'eau)
// PARAMÈTRES :
//   - gfx   : contexte graphique
//   - x, y  : position du bateau
//   - size  : taille de la cellule
//   - time  : temps actuel (pour l'animation)
// RETOUR : aucun (void)
// ============================================================================
void drawSunkenShip(GraphicsContext *gfx, int x, int y, int size, Uint32 time) {
    SDL_FRect cell = { x, y, size - 1, size - 1 };
    
    // Fond océan sombre (bleu très foncé)
    drawGradientRect(gfx->renderer, x, y, size - 1, size - 1,
                    10, 30, 50, 5, 15, 25);
    
    // Épave visible sous l'eau (transparente)
    SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(gfx->renderer, 40, 40, 50, 150);  // Gris bleuté transparent
    SDL_FRect wreck = { x + 5, y + 5, size - 11, size - 11 };
    SDL_RenderFillRect(gfx->renderer, &wreck);
    
    // Détails de l'épave (lignes structurelles)
    SDL_SetRenderDrawColor(gfx->renderer, 30, 30, 40, 120);
    SDL_FRect wreckDetail1 = { x + 7, y + 7, size - 15, 3 };
    SDL_FRect wreckDetail2 = { x + 7, y + size - 10, size - 15, 3 };
    SDL_RenderFillRect(gfx->renderer, &wreckDetail1);
    SDL_RenderFillRect(gfx->renderer, &wreckDetail2);
    
    // Bulles d'air remontant à la surface (animation)
    int bubbleOffset = (time / 200) % (size - 10);
    for (int i = 0; i < 3; i++) {
        int by = y + size - bubbleOffset - i * (size / 4);  // Position Y de la bulle
        if (by > y && by < y + size) {  // Si la bulle est dans la cellule
            SDL_SetRenderDrawColor(gfx->renderer, 150, 200, 255, 100);  // Intérieur bulle
            drawFilledCircle(gfx->renderer, x + size/2 + i * 5 - 5, by, 2 + i);
            
            // Contour des bulles (plus visible)
            SDL_SetRenderDrawColor(gfx->renderer, 200, 230, 255, 150);
            drawCircleOutline(gfx->renderer, x + size/2 + i * 5 - 5, by, 2 + i);
        }
    }
    
    SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_NONE);
    
    // X noir épais pour marquer le naufrage (4 pixels d'épaisseur)
    SDL_SetRenderDrawColor(gfx->renderer, 0, 0, 0, 255);
    for (int i = 0; i < 4; i++) {
        SDL_RenderLine(gfx->renderer, x + 3 + i, y + 3, x + size - 4 + i, y + size - 4);  // Diagonale \
        SDL_RenderLine(gfx->renderer, x + size - 4 - i, y + 3, x + 3 - i, y + size - 4);  // Diagonale /
    }
    
    // Contour blanc pour contraste (derrière le X noir)
    SDL_SetRenderDrawColor(gfx->renderer, 200, 200, 200, 255);
    for (int i = 0; i < 2; i++) {
        SDL_RenderLine(gfx->renderer, x + 2 + i, y + 2, x + size - 3 + i, y + size - 3);
        SDL_RenderLine(gfx->renderer, x + size - 3 - i, y + 2, x + 2 - i, y + size - 3);
    }
    
    // Effet d'huile à la surface (marée noire)
    SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(gfx->renderer, 80, 60, 40, 80);  // Marron transparent
    SDL_FRect oil = { x + 2, y + 2, size - 4, 4 };
    SDL_RenderFillRect(gfx->renderer, &oil);
    
    // Reflets arc-en-ciel de l'huile (effet chromatique animé)
    int oilShimmer = (time / 150) % 3;  // Change de couleur toutes les 150ms
    if (oilShimmer == 0) {
        SDL_SetRenderDrawColor(gfx->renderer, 150, 100, 200, 60);  // Violet
    } else if (oilShimmer == 1) {
        SDL_SetRenderDrawColor(gfx->renderer, 100, 150, 200, 60);  // Bleu
    } else {
        SDL_SetRenderDrawColor(gfx->renderer, 200, 150, 100, 60);  // Orange
    }
    SDL_FRect oilShine = { x + 3, y + 3, size - 6, 2 };
    SDL_RenderFillRect(gfx->renderer, &oilShine);
    
    SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_NONE);
}

// ============================================================================
// FONCTION : drawMissedShot
// DESCRIPTION : Dessine un tir manqué avec splash réaliste dans l'eau
// PARAMÈTRES :
//   - gfx   : contexte graphique
//   - x, y  : position de l'impact
//   - size  : taille de la cellule
//   - time  : temps actuel (pour l'animation)
// RETOUR : aucun (void)
// ============================================================================
void drawMissedShot(GraphicsContext *gfx, int x, int y, int size, Uint32 time) {
    // Eau avec vagues animées
    drawWaves(gfx->renderer, x, y, size - 1, size - 1, time);
    
    // Impact/splash avec transparence
    SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_BLEND);
    
    // Cercles d'eau concentriques (ondes)
    int ripple = (time / 100) % 10;  // Animation des ondulations
    for (int i = 0; i < 3; i++) {
        int radius = 5 + i * 4 + ripple;  // Rayon croissant + animation
        if (radius < size/2) {  // Ne dépasse pas de la cellule
            SDL_SetRenderDrawColor(gfx->renderer, 255, 255, 255, 150 - i * 40);  // Blanc de plus en plus transparent
            drawCircleOutline(gfx->renderer, x + size/2, y + size/2, radius);
            // Épaissir le cercle (double trait)
            drawCircleOutline(gfx->renderer, x + size/2, y + size/2, radius + 1);
        }
    }
    
    // Éclaboussures dans les 8 directions principales
    int splashPositions[8][2] = {
        {12, 0}, {8, 8}, {0, 12}, {-8, 8},
        {-12, 0}, {-8, -8}, {0, -12}, {8, -8}
    };
    
    SDL_SetRenderDrawColor(gfx->renderer, 200, 230, 255, 180);  // Bleu clair transparent
    for (int i = 0; i < 8; i++) {
        int sx = x + size/2 + splashPositions[i][0];  // Position X éclaboussure
        int sy = y + size/2 + splashPositions[i][1];  // Position Y éclaboussure
        SDL_FRect splash = { sx - 2, sy - 2, 4, 4 };  // Petite goutte carrée
        SDL_RenderFillRect(gfx->renderer, &splash);
    }
    
    // Point d'impact central (goutte principale)
    SDL_SetRenderDrawColor(gfx->renderer, 255, 255, 255, 220);
    drawFilledCircle(gfx->renderer, x + size/2, y + size/2, 3);
    
    SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_NONE);
}

// ============================================================================
// FONCTION : drawGrid
// DESCRIPTION : Dessine une grille complète avec tous ses éléments
// PARAMÈTRES :
//   - gfx     : contexte graphique
//   - g       : grille à dessiner (tableau 2D)
//   - offsetX : décalage horizontal (pour la grille de droite)
//   - reveal  : flag pour révéler les bateaux intacts (1) ou les cacher (0)
// RETOUR : aucun (void)
// ============================================================================
void drawGrid(GraphicsContext *gfx, int g[GRID][GRID], int offsetX, int reveal) {
    // Récupère le temps actuel pour les animations
    Uint32 currentTime = SDL_GetTicks();
    
    // Parcourt toutes les cellules de la grille
    for (int i = 0; i < GRID; i++) {
        for (int j = 0; j < GRID; j++) {
            // Calcule la position à l'écran de cette cellule
            int x = offsetX + j * CELL;
            int y = i * CELL;
            
            // Récupère la valeur de la cellule
            int cellValue = g[i][j];

            // Détermine quoi dessiner selon la valeur de la cellule
            if (cellValue <= -200) {
                // Bateau coulé - effet de naufrage réaliste
                drawSunkenShip(gfx, x, y, CELL, currentTime);
            }
            else if (cellValue < 0 && cellValue != -99) {
                // Bateau touché - en feu avec animations
                drawDamagedShip(gfx, x, y, CELL, currentTime);
            }
            else if (cellValue == -99) {
                // Tir manqué - splash animé dans l'eau
                drawMissedShot(gfx, x, y, CELL, currentTime);
            }
            else if (cellValue > 0 && reveal) {
                // Bateau intact - ultra-détaillé (seulement si reveal=1)
                int shipId = cellValue;  // ID du bateau
                int position = 0;        // Position dans le bateau
                int length = 0;          // Longueur totale du bateau
                int isVertical = 0;      // Orientation
                
                // Analyse la grille pour déterminer l'orientation et la position
                // Compte les cases alignées verticalement
                int vCount = 0;
                for (int k = 0; k < GRID; k++) {
                    if (g[k][j] == shipId || g[k][j] == -shipId || g[k][j] <= -200) {
                        vCount++;
                        if (k < i) position++;  // Cases au-dessus = position antérieure
                    }
                }
                
                // Compte les cases alignées horizontalement
                int hCount = 0;
                int hPos = 0;
                for (int k = 0; k < GRID; k++) {
                    if (g[i][k] == shipId || g[i][k] == -shipId || g[i][k] <= -200) {
                        hCount++;
                        if (k < j) hPos++;  // Cases à gauche = position antérieure
                    }
                }
                
                // Détermine l'orientation (verticale si plus de cases alignées verticalement)
                if (vCount > 1) {
                    isVertical = 1;
                    length = vCount;
                } else {
                    isVertical = 0;
                    length = hCount;
                    position = hPos;  // Position horizontale
                }
                
                // Utilise le design ultra-réaliste pour le bateau intact
                drawRealisticShip(gfx, x, y, CELL, isVertical, position, length, shipId);
            }
            else {
                // Eau normale avec vagues animées (case vide)
                drawWaves(gfx->renderer, x, y, CELL - 1, CELL - 1, currentTime);
            }
            
            // Grille de séparation subtile entre les cellules
            SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(gfx->renderer, 255, 255, 255, 30);  // Blanc très transparent
            SDL_RenderLine(gfx->renderer, x + CELL - 1, y, x + CELL - 1, y + CELL);  // Ligne droite
            SDL_RenderLine(gfx->renderer, x, y + CELL - 1, x + CELL, y + CELL - 1);  // Ligne bas
            SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_NONE);
        }
    }
}

// ============================================================================
// FONCTION : drawTurnScreen
// DESCRIPTION : Dessine l'écran de changement de tour (mode PvP)
// PARAMÈTRES :
//   - gfx    : contexte graphique
//   - player : joueur dont c'est le tour (0 = joueur 1, 1 = joueur 2)
// RETOUR : aucun (void)
// ============================================================================
void drawTurnScreen(GraphicsContext *gfx, int player) {
    // ===== Fond =====
    SDL_SetRenderDrawColor(gfx->renderer, 15, 25, 40, 255);  // Bleu nuit
    SDL_RenderClear(gfx->renderer);

    // ===== Boîte centrale =====
    int boxW = WIDTH / 2;    // Largeur de la boîte (moitié de la largeur de l'écran)
    int boxH = HEIGHT / 4;   // Hauteur de la boîte (quart de la hauteur de l'écran)
    int boxX = (WIDTH - boxW) / 2;  // Position X centrée
    int boxY = (HEIGHT - boxH) / 2; // Position Y centrée

    SDL_FRect box = { boxX, boxY, boxW, boxH };

    // Ombre portée (décalée)
    SDL_SetRenderDrawColor(gfx->renderer, 0, 0, 0, 120);
    SDL_FRect shadow = { boxX + 6, boxY + 6, boxW, boxH };
    SDL_RenderFillRect(gfx->renderer, &shadow);

    // Fond de la boîte
    SDL_SetRenderDrawColor(gfx->renderer, 40, 60, 90, 255);  // Bleu moyen
    SDL_RenderFillRect(gfx->renderer, &box);

    // Bordure de la boîte
    SDL_SetRenderDrawColor(gfx->renderer, 180, 200, 220, 255);  // Bleu clair
    SDL_RenderRect(gfx->renderer, &box);

    // ===== Indicateur joueur (carré coloré) =====
    SDL_FRect playerBox = {
        boxX + boxW / 4,          // Décalage depuis gauche
        boxY + boxH / 2 - 25,     // Centré verticalement
        boxW / 2,                 // Largeur moitié de la boîte
        50                        // Hauteur fixe
    };

    // Couleur différente selon le joueur
    if (player == 0) {
        // Joueur 1 : Vert
        SDL_SetRenderDrawColor(gfx->renderer, 60, 180, 120, 255);
    } else {
        // Joueur 2 : Rouge
        SDL_SetRenderDrawColor(gfx->renderer, 200, 70, 70, 255);
    }

    SDL_RenderFillRect(gfx->renderer, &playerBox);

    // Bordure du carré joueur
    SDL_SetRenderDrawColor(gfx->renderer, 255, 255, 255, 255);
    SDL_RenderRect(gfx->renderer, &playerBox);

    // ===== Texte simple "JOUEUR X" (dessiné avec des lignes) =====
    SDL_SetRenderDrawColor(gfx->renderer, 255, 255, 255, 255);

    int cx = playerBox.x + playerBox.w / 2;  // Centre du carré
    int cy = playerBox.y + 12;               // Position verticale pour le texte

    // Lettre "P" (pour Player)
    SDL_RenderLine(gfx->renderer, cx - 30, cy, cx - 30, cy + 20); // barre verticale
    SDL_RenderLine(gfx->renderer, cx - 30, cy, cx - 10, cy);     // haut
    SDL_RenderLine(gfx->renderer, cx - 10, cy, cx - 10, cy + 10);// côté droit
    SDL_RenderLine(gfx->renderer, cx - 30, cy + 10, cx - 10, cy + 10); // milieu

    // Numéro du joueur
    if (player == 0) {
        // Chiffre 1
        SDL_RenderLine(gfx->renderer, cx + 10, cy, cx + 10, cy + 20);  // Barre verticale
    } else {
        // Chiffre 2
        SDL_RenderLine(gfx->renderer, cx + 5, cy, cx + 25, cy);        // Haut
        SDL_RenderLine(gfx->renderer, cx + 25, cy, cx + 25, cy + 10);  // Droite haut
        SDL_RenderLine(gfx->renderer, cx + 5, cy + 10, cx + 25, cy + 10); // Milieu
        SDL_RenderLine(gfx->renderer, cx + 5, cy + 10, cx + 5, cy + 20);  // Gauche bas
        SDL_RenderLine(gfx->renderer, cx + 5, cy + 20, cx + 25, cy + 20); // Bas
    }

    // Met à jour l'affichage
    SDL_RenderPresent(gfx->renderer);
}

// ============================================================================
// FONCTION : drawShipPreview
// DESCRIPTION : Dessine l'aperçu du bateau en cours de placement (à droite de l'écran)
// PARAMÈTRES :
//   - gfx       : contexte graphique
//   - shipSize  : taille du bateau (ou -10 pour 5x2)
//   - vertical  : orientation (1 = vertical, 0 = horizontal)
// RETOUR : aucun (void)
// ============================================================================
void drawShipPreview(GraphicsContext *gfx, int shipSize, int vertical) {
    // Fond de la zone d'aperçu (moitié droite de l'écran)
    SDL_FRect bg = { GRID * CELL, 0, GRID * CELL, HEIGHT };
    SDL_SetRenderDrawColor(gfx->renderer, 10, 10, 10, 255);  // Noir
    SDL_RenderFillRect(gfx->renderer, &bg);

    // Centre de la zone d'aperçu
    int centerX = GRID * CELL + (GRID * CELL) / 2;
    int centerY = HEIGHT / 2;

    // Détermine les dimensions du bateau selon sa taille et orientation
    int width, height;
    if (shipSize > 0) {
        // Bateau normal
        width  = vertical ? 1 : shipSize;
        height = vertical ? shipSize : 1;
    } else {
        // Bateau 5x2
        width  = vertical ? 2 : 5;
        height = vertical ? 5 : 2;
    }

    // Position de départ pour centrer le bateau
    int startX = centerX - (width * CELL) / 2;
    int startY = centerY - (height * CELL) / 2;

    // Dessine le bateau case par case
    SDL_FRect cell;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cell.x = startX + j * CELL;
            cell.y = startY + i * CELL;
            cell.w = CELL - 2;  // Légèrement plus petit pour la bordure
            cell.h = CELL - 2;

            // Intérieur gris
            SDL_SetRenderDrawColor(gfx->renderer, 180, 180, 180, 255);
            SDL_RenderFillRect(gfx->renderer, &cell);

            // Bordure noire
            SDL_SetRenderDrawColor(gfx->renderer, 0, 0, 0, 255);
            SDL_RenderRect(gfx->renderer, &cell);
        }
    }
}

// ============================================================================
// FONCTION : drawShipGhost
// DESCRIPTION : Dessine le "fantôme" (aperçu translucide) du bateau pendant le placement manuel
// PARAMÈTRES :
//   - gfx       : contexte graphique
//   - grid      : grille actuelle du joueur
//   - mouseX    : position X de la souris
//   - mouseY    : position Y de la souris
//   - shipSize  : taille du bateau (ou -10 pour 5x2)
//   - vertical  : orientation (1 = vertical, 0 = horizontal)
// RETOUR : aucun (void)
// ============================================================================
void drawShipGhost(GraphicsContext *gfx, int grid[GRID][GRID], int mouseX, int mouseY,
                   int shipSize, int vertical) {
    // Convertit les coordonnées souris en coordonnées grille
    int gx = mouseX / CELL;
    int gy = mouseY / CELL;

    // Vérifie que la souris est dans la grille
    if (gx < 0 || gx >= GRID || gy < 0 || gy >= GRID) return;

    // Détermine les dimensions selon le type de bateau et l'orientation
    int width, height;
    if (shipSize > 0) {
        width  = vertical ? 1 : shipSize;
        height = vertical ? shipSize : 1;
    } else {
        width  = vertical ? 2 : 5;
        height = vertical ? 5 : 2;
    }

    // Vérifie la validité du placement
    int valid = 1;
    
    // Vérifie les limites de la grille
    if (gx + width > GRID || gy + height > GRID) {
        valid = 0;
    } else {
        // Vérifie que la zone (avec marge) est libre
        for (int i = gy - 1; i <= gy + height; i++) {
            for (int j = gx - 1; j <= gx + width; j++) {
                if (i < 0 || i >= GRID || j < 0 || j >= GRID)
                    continue;  // Hors grille
                if (grid[i][j] != 0)
                    valid = 0;  // Case occupée
            }
        }
    }

    // Couleur du fantôme : vert si valide, rouge si invalide
    if (valid)
        SDL_SetRenderDrawColor(gfx->renderer, 0, 200, 0, 120);  // Vert semi-transparent
    else
        SDL_SetRenderDrawColor(gfx->renderer, 200, 0, 0, 120);  // Rouge semi-transparent

    // Active la transparence
    SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_BLEND);

    // Dessine le fantôme case par case
    SDL_FRect r;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            r.x = (gx + j) * CELL + 1;
            r.y = (gy + i) * CELL + 1;
            r.w = CELL - 2;
            r.h = CELL - 2;
            SDL_RenderFillRect(gfx->renderer, &r);
        }
    }

    // Désactive la transparence
    SDL_SetRenderDrawBlendMode(gfx->renderer, SDL_BLENDMODE_NONE);
}

// ============================================================================
// FONCTION : placeShipsSDL
// DESCRIPTION : Gère le placement manuel des bateaux avec interface SDL
// PARAMÈTRES :
//   - gfx       : contexte graphique
//   - p         : joueur qui place ses bateaux
//   - config    : configuration des bateaux (tailles)
//   - playerNum : numéro du joueur (0 ou 1)
// RETOUR : aucun (void)
// ============================================================================
void placeShipsSDL(GraphicsContext *gfx, Player *p, ShipConfig *config, int playerNum) {
    int currentShip = 0;  // Index du bateau en cours de placement
    int vertical = 0;     // Orientation (0=horizontal, 1=vertical)
    SDL_Event e;
    int placing = 1;      // Flag de continuation

    // Boucle principale de placement
    while(placing) {
        // Efface l'écran avec fond gris foncé
        SDL_SetRenderDrawColor(gfx->renderer, 20, 20, 20, 255);
        SDL_RenderClear(gfx->renderer);

        // Dessine la grille actuelle (avec les bateaux déjà placés)
        drawGrid(gfx, p->grid, 0, 1);

        // Récupère la position de la souris
        float mouseXF, mouseYF;
        SDL_GetMouseState(&mouseXF, &mouseYF);
        int mouseX = (int)mouseXF;
        int mouseY = (int)mouseYF;

        // Dessine le fantôme du bateau en cours
        drawShipGhost(gfx, p->grid, mouseX, mouseY, config->sizes[currentShip], vertical);
        
        // Dessine l'aperçu du bateau à droite
        drawShipPreview(gfx, config->sizes[currentShip], vertical);

        // Met à jour l'affichage
        SDL_RenderPresent(gfx->renderer);

        // Gère les événements
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_EVENT_QUIT) exit(0);  // Fermeture de la fenêtre
            
            else if(e.type == SDL_EVENT_KEY_DOWN) {
                // Touche V pour changer l'orientation
                if(e.key.scancode == SDL_SCANCODE_V) {
                    vertical = !vertical;  // Inverse l'orientation
                }
            }
            else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                // Clic pour placer le bateau
                // Note : attention, x et y sont inversés (SDL utilise Y pour la ligne)
                int x = e.button.y / CELL;  // Ligne
                int y = e.button.x / CELL;  // Colonne

                // Vérifie le type de bateau
                if (config->sizes[currentShip] < 0) {
                    // Bateau 5x2
                    if (canPlaceShip5x2Manual(p->grid, x, y, vertical)) {
                        placeShip5x2Manual(p->grid, x, y, currentShip + 1, vertical);
                        currentShip++;  // Passe au bateau suivant
                    } else {
                        // Affiche un message d'erreur
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Erreur",
                            "Placement impossible pour le bateau 5x2", gfx->window);
                    }
                } else {
                    // Bateau normal
                    if (canPlaceShip(p->grid, x, y, config->sizes[currentShip], vertical)) {
                        // Place le bateau case par case
                        for (int i = 0; i < config->sizes[currentShip]; i++) {
                            int nx = x + (vertical ? i : 0);  // Nouvelle ligne
                            int ny = y + (vertical ? 0 : i);  // Nouvelle colonne
                            p->grid[nx][ny] = currentShip + 1;  // ID du bateau
                        }
                        currentShip++;  // Passe au bateau suivant
                    } else {
                        // Affiche un message d'erreur
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Erreur",
                            "Placement impossible", gfx->window);
                    }
                }

                // Vérifie si tous les bateaux sont placés
                if (currentShip >= SHIP_COUNT) {
                    placing = 0;  // Termine le placement
                    break;
                }
            }
        }
    }
}