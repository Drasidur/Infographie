#pragma once
#include <vector>
#include "SommetTerrain.h"

using namespace std;

namespace PM3D {
   // Classe CTerrain
   // BUT : Classe modélisant le terrain
   class CTerrain {
   private:
      // nombre de lignes de la grille
      int nb_lignes;
      // nombre de colonnes de la grille
      int nb_colonnes;
      // dx et dy coeff de multiplication (echelle)
      float dx, dy;
      // vecteur d'index de polygones
      vector<int> index_bloc;
      // vecteurs des sommets : position + normale
      vector<CSommetTerrain> sommets;

   public :

      // Constructeur de Terrain :
      // int nb_lignes : nombre de lignes de la grille
      // int nb_colonnes : nombre de colonnes de la grille
      // vecteur de flotant de hauteurs des points dans la grille
      CTerrain(const int nb_lignes, const int nb_colonnes, const float dx, const float dy, vector<float> positions);

      // Calcule les normales de chaque points dans la grille
      void CalculerNormales();

      // Obtenir le vecteur x, y, z dans la grille à la position X, Y
      XMFLOAT3 ObtenirPositionXMFLOAT3(int X, int Y);
      XMVECTOR ObtenirPosition(int X, int Y);

      // Construit le tableau d'index
      void ConstruireIndex();

      // Construit un terrain à partir d'un fichier de data
      static CTerrain ConstruireTerrain(string nomFichierSauvegarde);

      // Getteurs
      vector<int> getIndexBloc() { return index_bloc; }
      vector<CSommetTerrain> getSommets() { return sommets; }

      // Constructeur par defaut
      CTerrain() = default;

      // CheckValide -> Affiche les indices blocs et les positions + normales d'un terrain
      void getAll();
   };
}