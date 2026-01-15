// Inclusion de la bibliothèque SDL3 pour les fonctions de temps éventuellement utilisées ailleurs
#include <SDL3/SDL.h>

// Inclusion du fichier d'en-tête contenant les définitions de types personnalisés
// Ce fichier contient probablement la déclaration de la variable 'seed' utilisée ci-dessous
#include "types.h"

// Inclusion du fichier d'en-tête correspondant à ce fichier source
// Ce fichier contient probablement la déclaration de la fonction myRand()
#include "random.h"

// ============================================================================
// FONCTION : myRand
// DESCRIPTION : Générateur de nombres pseudo-aléatoires maison (LCG - Linear Congruential Generator)
// ALGORITHME : Génération congruentielle linéaire
// FORMULE : Xₙ₊₁ = (a × Xₙ + c) mod m
//   Ici : a = 1103515245, c = 12345, m = 2³² (implicite par dépassement d'entier)
// CARACTÉRISTIQUES :
//   - Période : 2³² (environ 4.29 milliards)
//   - Non cryptographiquement sécurisé
//   - Simple et rapide
// UTILISATION : Pour la génération aléatoire dans le jeu (placement bateaux, IA, etc.)
// RETOUR : Nombre pseudo-aléatoire entre 0 et 32767 (15 bits)
// ============================================================================
int myRand() {
    // Étape 1 : Mise à jour de la graine selon la formule du générateur congruentiel linéaire
    // seed doit être une variable globale de type unsigned int (32 bits)
    // Opération : seed = (seed * 1103515245) + 12345
    // Note : Le modulo 2³² est implicite via le dépassement d'entier non signé 32 bits
    seed = seed * 1103515245 + 12345;
    
    // Étape 2 : Extraction des bits 16 à 30 (15 bits) pour obtenir un nombre entre 0 et 32767
    // (seed >> 16) : décale la graine de 16 bits vers la droite (supprime les 16 bits de poids faible)
    // & 0x7FFF : masque pour garder seulement les 15 bits de poids faible (0x7FFF = 32767 en décimal)
    // Résultat : nombre entre 0 et 32767 inclus
    return (seed >> 16) & 0x7FFF;
}

// NOTES COMPLÉMENTAIRES :
// 1. La variable 'seed' doit être initialisée avant le premier appel à myRand()
//    Exemple : seed = SDL_GetTicks(); (comme fait dans main.c)
// 2. Ce générateur est déterministe : même graine = même séquence de nombres
// 3. Qualité statistique acceptable pour les jeux, mais pas pour les applications cryptographiques
// 4. Les constantes 1103515245 et 12345 sont des paramètres standard du générateur rand() de glibc