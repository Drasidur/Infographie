#include "Terrain.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <algorithm>
#include <iostream>
#include "SommetTerrain.h"
#include <fstream>

using namespace std;
using namespace DirectX;

namespace PM3D {

   // Constructeur de Terrain :
   // int nb_lignes : nombre de lignes de la grille
   // int nb_colonnes : nombre de colonnes de la grille
   // vecteur de flotant de hauteurs des points dans la grille
   CTerrain::CTerrain(const int nb_lignes, const int nb_colonnes, const float dx, const float dy, vector<float> positions) : nb_lignes{ nb_lignes }, nb_colonnes{ nb_colonnes },
                                                                                                                             dx{ dx }, dy{ dy } 
   {
      ////// Les points //////
      vector<XMFLOAT3> points;
      // Fonction de création des XMFLOAT3
      auto ppb = [&points, x = -1, y = 0, nb_colonnes, dx, dy](float hauteur) mutable {
         if (x >= nb_colonnes - 1) { x = 0; y++; } else { ++x; }
         points.push_back(XMFLOAT3(x*dx, y*dy, hauteur));
      };
      for_each(begin(positions), end(positions), ppb);

      ////// Calculer les normales //////
      XMFLOAT3 nunique(0.0f, 0.0f, 1.0f);

      ////// Calcul des sommets //////
      auto buildSommets = [this, nunique, nb_lignes, nb_colonnes](XMFLOAT3 v) {
         CSommetTerrain cst = CSommetTerrain(v, nunique);
         sommets.push_back(cst);
      };
      for_each(begin(points), end(points), buildSommets);
   };


   // Obtenir le vecteur x, y, z dans la grille à la position X, Y
   XMFLOAT3 CTerrain::ObtenirPositionXMFLOAT3(int X, int Y) {
      return sommets[Y * nb_colonnes + X].position;
   }
   XMVECTOR CTerrain::ObtenirPosition(int X, int Y) {
      XMFLOAT3 v = CTerrain::sommets[Y * CTerrain::nb_colonnes + X].position;
      return XMVectorSet(v.x, v.y, v.z, 0);
   }

   
   // Calcule les normales de chaque points dans la grille
   void CTerrain::CalculerNormales() {

      // calcule la normale d'un sommet via l'algorithme propose
      auto calcNormale = [this](CSommetTerrain& s) {
         XMVECTOR n1, n2, n3, n4;
         XMVECTOR v1, v2, v3, v4;

         // Pour normer bien les vecteurs
         float vec = dx * dy;

         n1 = XMVectorSet(0, 0, vec, 0);
         n2 = XMVectorSet(0, 0, vec, 0);
         n3 = XMVectorSet(0, 0, vec, 0);
         n4 = XMVectorSet(0, 0, vec, 0);

         int x = static_cast<int>(s.position.x) / dx;
         int y = static_cast<int>(s.position.y) / dy;
         XMVECTOR maPosition = ObtenirPosition(x , y);

         // Calcul des vi
         if (y < CTerrain::nb_lignes - 1)   { v3 = ObtenirPosition(x, y + 1) - maPosition; }
         if (x < CTerrain::nb_colonnes - 1) { v2 = ObtenirPosition(x + 1, y) - maPosition; }
         if (y > 0)                         { v1 = ObtenirPosition(x, y - 1) - maPosition; }
         if (x > 0)                         { v4 = ObtenirPosition(x - 1, y) - maPosition; }

         // Calcul des normales
         if (x < nb_colonnes - 1 && y > 0)             { n1 = XMVector3Cross(v1, v2); }
         if (y < nb_lignes - 1 && x < nb_colonnes - 1) { n2 = XMVector3Cross(v2, v3); }
         if (y < nb_lignes - 1 && x > 0)               { n3 = XMVector3Cross(v3, v4); }
         if (y > 0 && x > 0)                           { n4 = XMVector3Cross(v4, v1); }

         // Calcul de la normale resultante
         n1 = n1 + n2 + n3 + n4;
         n1 = XMVector3Normalize(n1);
         XMFLOAT3 res;
         XMStoreFloat3(&res, n1);
         
         // On affecte le resultat
         s.normale = res;
      };

      // Pour tous les sommets, on calcule sa normale
      for_each(begin(sommets), end(sommets), calcNormale);
   }

   // Construit le tableau d'index
   void CTerrain::ConstruireIndex() {

      // Nombre de sommets
      int nombreSommets = nb_lignes * nb_colonnes;

      // Pour chaque sommet qui n'est pas sur le bord droit ni sur le bas de la grille, on construit les deux triangles
      for (int i = 0; i != nombreSommets; ++i) {
         if (((i + 1) % nb_colonnes) != 0 && i < nb_colonnes * (nb_lignes - 1)) {
            index_bloc.push_back(i);
            index_bloc.push_back(i + 1);
            index_bloc.push_back(i + nb_colonnes);
            index_bloc.push_back(i + 1);
            index_bloc.push_back(i + 1 + nb_colonnes);
            index_bloc.push_back(i + nb_colonnes);
         }
      }
   }

   // CheckValide -> Affiche les indices blocs et les positions + normales d'un terrain
   void CTerrain::getAll() {
      int n = index_bloc.size();
      for (int i = 0; i != n; ++i) {
         cout << index_bloc[i] << " ";
         if ((i + 1) % 3 == 0 && i != 0)
            cout << endl;
      }
      auto aff = [this](CSommetTerrain s) {
         cout << s.position.x << " " << s.position.y << " " << s.position.z << " " << s.normale.x << " " << s.normale.y << " " << s.normale.z << endl;
      };
      for_each(begin(sommets), end(sommets), aff);
   }

   // Construit un terrain à partir d'un fichier de data
   CTerrain CTerrain::ConstruireTerrain(string nomFichierSauvegarde) {
      ifstream fichier(nomFichierSauvegarde);
      if (fichier) {
         CTerrain terrain;
         int nbSommets;
         fichier >> nbSommets;
         for (int i = 0; i != nbSommets; ++i) {
            float x, y, z, nx, ny, nz;
            fichier >> x; fichier >>y; fichier >> z; fichier >> nx; fichier >> ny; fichier >> nz;
            XMFLOAT3 pos = XMFLOAT3(x, y, z); XMFLOAT3 normale = XMFLOAT3(nx, ny, nz);
            terrain.sommets.push_back(CSommetTerrain(pos, normale));
         }
         int pos;
         while (fichier >> pos) {
            terrain.index_bloc.push_back(pos);
         }
         return terrain;
      }
      else {
         cerr << "Impossible d'ouvrir le fichier" << endl;
      }
   }
}