#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Terrain.h"
#include <algorithm>

using namespace std;
using namespace PM3D;

void LireFichierHeightmap(string nomFichier, int& nb_lignes, int& nb_colonnes, float& dx, float& dy, vector<float>& positions) {

   // On ouvre le fichier en lecture
   ifstream fichier(nomFichier, ios::in);

   // Si l'ouverture a réussi
   if (fichier) {
      // On récupère la taille du patch
      fichier >> nb_lignes;
      fichier >> nb_colonnes;
      fichier >> dx;
      fichier >> dy;

      // On construit le vecteur de hauteur
      float hauteur;
      for (int haut = 0; haut != nb_lignes * nb_colonnes; ++haut) {
         fichier >> hauteur;
         positions.push_back(hauteur);
      }

      // Fermeture du descripteur
      fichier.close();
   }
   else {
      cerr << "Impossible d'ouvrir le fichier" << endl;
   }
}

//void EnregistrerTout(string nomFichierSauvegarde, CTerrain terrain) {
//   // Ouverture du fichier en écriture binaire
//   ofstream fichier(nomFichierSauvegarde.c_str(), ios::binary);
//   if (fichier) {
//      fichier.write((char*)&terrain, sizeof(terrain));
//      fichier.close();
//   }
//   else {
//      cerr << "Impossible d'ouvrir le fichier" << endl;
//   }
//}

void EnregistrerToutClassique(string nomFichierSauvegarde, CTerrain terrain) {
   // Ouverture du fichier en écriture binaire
   ofstream fichier(nomFichierSauvegarde, ios::binary);
   auto writeSommet = [&fichier](CSommetTerrain s) {fichier << s.position.x << " " << s.position.y << " " << s.position.z << " " << s.normale.x << " " << s.normale.y << " " << s.normale.z << endl; };
   auto writeIndex = [&fichier](int i) { fichier << i << " "; };
   if (fichier) {
      vector<CSommetTerrain> sommets = terrain.getSommets();
      vector<int> index_bloc = terrain.getIndexBloc();
      fichier << sommets.size() << endl;
      for_each(begin(sommets), end(sommets), writeSommet);
      for_each(begin(index_bloc), end(index_bloc), writeIndex);
      fichier.close();
   }
   else {
      cerr << "Impossible d'ouvrir le fichier" << endl;
   }
}

//CTerrain LireTout(string nomFichierSauvegarde) {
//   // Ouverture du fichier en lecture binaire
//   ifstream fichier(nomFichierSauvegarde.c_str());
//   if (fichier) {
//      CTerrain terrain;
//      fichier.read((char*)&terrain, sizeof(terrain));
//      fichier.close();
//      return terrain;
//   }
//   else {
//      cerr << "Impossible d'ouvrir le fichier" << endl;
//   }
//}


int main(int argc, char *argv[]) {
   // On récupère le nom du fichier à lire et le nom du fichier de stockage
   string nomFichier = argv[1];
   string nomFichierSauvegarde = argv[2];
   int nb_lignes, nb_colonnes;
   float dx, dy;
   vector<float> positions;

   // On lit les donnees dans le fichier d'entree
   LireFichierHeightmap(nomFichier, nb_lignes, nb_colonnes, dx, dy, positions);
   // On construit le terrain
   CTerrain terrain = CTerrain(nb_lignes, nb_colonnes, dx, dy, positions);;
   // On calcule les normales associees aux sommets
   terrain.CalculerNormales();
   // On construit l'index
   terrain.ConstruireIndex();
   // On enregistre les donnees
   EnregistrerToutClassique(nomFichierSauvegarde, terrain);
   // On lit tout pour verifier
   CTerrain terrain2 = CTerrain::ConstruireTerrain(nomFichierSauvegarde);
   terrain2.getAll();

   return 0;
}