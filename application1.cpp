#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Interface Telechargeable
class Telechargeable {
public:
    virtual void telecharger() = 0;
    virtual void afficherMessage() = 0;
    virtual ~Telechargeable() = default;
};

// Classe de base Ressource
class Ressource {
protected:
    int id;
    string titre;
    string auteur;
    int annee;

public:
    Ressource(int id, string titre, string auteur, int annee)
        : id(id), titre(titre), auteur(auteur), annee(annee) {}

    virtual void afficherInfos() {
        cout << "ID: " << id << " | Titre: " << titre 
             << " | Auteur: " << auteur << " | Année: " << annee;
    }

    // Méthode virtuelle pour le téléchargement
    virtual void telecharger() {
        cout << "Téléchargement de la ressource: " << titre << endl;
    }

    // Surcharge de l'opérateur ==
    bool operator==(const Ressource& autre) const {
        return this->id == autre.id;
    }

    // Getters
    int getId() const { return id; }
    string getTitre() const { return titre; }
    string getAuteur() const { return auteur; }
    int getAnnee() const { return annee; }

    virtual ~Ressource() = default;
};

// Classe Livre héritant de Ressource et Telechargeable
class Livre : public Ressource, public Telechargeable {
private:
    int nombrePages;

public:
    Livre(int id, string titre, string auteur, int annee, int nombrePages)
        : Ressource(id, titre, auteur, annee), nombrePages(nombrePages) {}

    void afficherInfos() override {
        Ressource::afficherInfos();
        cout << " | Type: Livre | Pages: " << nombrePages << endl;
    }

    // Implémentation de Telechargeable
    void telecharger() override {
        cout << "📖 Téléchargement du livre: " << titre << " (" << nombrePages << " pages)" << endl;
    }

    void afficherMessage() override {
        cout << "Ce livre est disponible au téléchargement." << endl;
    }
};

// Classe Magazine héritant de Ressource et Telechargeable
class Magazine : public Ressource, public Telechargeable {
private:
    int numero;

public:
    Magazine(int id, string titre, string auteur, int annee, int numero)
        : Ressource(id, titre, auteur, annee), numero(numero) {}

    void afficherInfos() override {
        Ressource::afficherInfos();
        cout << " | Type: Magazine | Numéro: " << numero << endl;
    }

    // Implémentation de Telechargeable
    void telecharger() override {
        cout << "📰 Téléchargement du magazine: " << titre << " (N°" << numero << ")" << endl;
    }

    void afficherMessage() override {
        cout << "Ce magazine est disponible au téléchargement." << endl;
    }
};

// Classe Video héritant de Ressource et Telechargeable
class Video : public Ressource, public Telechargeable {
private:
    int duree; // en minutes

public:
    Video(int id, string titre, string auteur, int annee, int duree)
        : Ressource(id, titre, auteur, annee), duree(duree) {}

    void afficherInfos() override {
        Ressource::afficherInfos();
        cout << " | Type: Vidéo | Durée: " << duree << " minutes" << endl;
    }

    // Implémentation de Telechargeable
    void telecharger() override {
        cout << "🎬 Téléchargement de la vidéo: " << titre << " (" << duree << " min)" << endl;
    }

    void afficherMessage() override {
        cout << "Cette vidéo est disponible au téléchargement." << endl;
    }
};

// Classe Médiathèque
class Mediatheque {
private:
    vector<Ressource*> ressources;

public:
    // Ajouter une ressource
    void ajouterRessource(Ressource* ressource) {
        ressources.push_back(ressource);
        cout << "Ressource ajoutée avec succès!" << endl;
    }

    // Afficher toutes les ressources
    void afficherToutesRessources() {
        if (ressources.empty()) {
            cout << "Aucune ressource dans la médiathèque." << endl;
            return;
        }
        
        cout << "\n=== TOUTES LES RESSOURCES ===" << endl;
        for (const auto& ressource : ressources) {
            ressource->afficherInfos();
        }
    }

    // Surcharge de la méthode rechercher - par titre
    vector<Ressource*> rechercher(const string& titre) {
        vector<Ressource*> resultats;
        for (const auto& ressource : ressources) {
            if (ressource->getTitre().find(titre) != string::npos) {
                resultats.push_back(ressource);
            }
        }
        return resultats;
    }

    // Surcharge de la méthode rechercher - par année
    vector<Ressource*> rechercher(int annee) {
        vector<Ressource*> resultats;
        for (const auto& ressource : ressources) {
            if (ressource->getAnnee() == annee) {
                resultats.push_back(ressource);
            }
        }
        return resultats;
    }

    // Surcharge de la méthode rechercher - par auteur et année
    vector<Ressource*> rechercher(const string& auteur, int annee) {
        vector<Ressource*> resultats;
        for (const auto& ressource : ressources) {
            if (ressource->getAuteur() == auteur && ressource->getAnnee() == annee) {
                resultats.push_back(ressource);
            }
        }
        return resultats;
    }

    // Afficher les résultats de recherche
    void afficherResultats(const vector<Ressource*>& resultats, const string& critere) {
        if (resultats.empty()) {
            cout << "Aucun résultat trouvé pour: " << critere << endl;
            return;
        }
        
        cout << "\n=== RÉSULTATS POUR: " << critere << " ===" << endl;
        for (const auto& ressource : resultats) {
            ressource->afficherInfos();
        }
    }

    ~Mediatheque() {
        for (auto& ressource : ressources) {
            delete ressource;
        }
    }
};

int main() {
    cout << "=== GESTION DE LA MÉDIATHÈQUE ===" << endl;
    
    // Création de la médiathèque
    Mediatheque mediatheque;

    // Création des ressources
    Livre* livre1 = new Livre(1, "Le Petit Prince", "Antoine de Saint-Exupéry", 1943, 96);
    Livre* livre2 = new Livre(2, "1984", "George Orwell", 1949, 328);
    Magazine* magazine1 = new Magazine(3, "Science et Vie", "Équipe éditoriale", 2023, 1250);
    Video* video1 = new Video(4, "Tutoriel C++", "Jean Dupont", 2024, 45);
    Video* video2 = new Video(5, "Documentaire Nature", "Marie Curie", 2023, 120);

    // Ajout des ressources à la médiathèque
    mediatheque.ajouterRessource(livre1);
    mediatheque.ajouterRessource(livre2);
    mediatheque.ajouterRessource(magazine1);
    mediatheque.ajouterRessource(video1);
    mediatheque.ajouterRessource(video2);

    cout << "\n";

    // Test d'affichage des informations
    cout << "=== AFFICHAGE DES INFORMATIONS ===" << endl;
    mediatheque.afficherToutesRessources();

    cout << "\n";

    // Test de téléchargement avec qualification de classe en cas de conflit
    cout << "=== TEST DE TÉLÉCHARGEMENT ===" << endl;
    
    // Utilisation de la méthode telecharger() de Ressource
    livre1->Ressource::telecharger();
    
    // Utilisation de la méthode telecharger() de Telechargeable (via Livre)
    livre1->telecharger();
    livre1->afficherMessage();
    
    cout << "---" << endl;
    magazine1->telecharger();
    magazine1->afficherMessage();
    
    cout << "---" << endl;
    video1->telecharger();
    video1->afficherMessage();

    cout << "\n";

    // Test des recherches avec surcharge
    cout << "=== RECHERCHE PAR TITRE ===" << endl;
    auto resultatsTitre = mediatheque.rechercher("Prince");
    mediatheque.afficherResultats(resultatsTitre, "Titre contenant 'Prince'");

    cout << "\n=== RECHERCHE PAR ANNÉE ===" << endl;
    auto resultatsAnnee = mediatheque.rechercher(2023);
    mediatheque.afficherResultats(resultatsAnnee, "Année 2023");

    cout << "\n=== RECHERCHE PAR AUTEUR ET ANNÉE ===" << endl;
    auto resultatsAuteurAnnee = mediatheque.rechercher("Marie Curie", 2023);
    mediatheque.afficherResultats(resultatsAuteurAnnee, "Auteur 'Marie Curie' et année 2023");

    cout << "\n";

    // Test de comparaison avec surcharge de l'opérateur ==
    cout << "=== COMPARAISON DE RESSOURCES ===" << endl;
    Livre* livreTest = new Livre(1, "Test", "Auteur Test", 2000, 100);
    
    cout << "Comparaison livre1 et livreTest (même ID): ";
    if (*livre1 == *livreTest) {
        cout << "ÉGALES" << endl;
    } else {
        cout << "DIFFÉRENTES" << endl;
    }

    cout << "Comparaison livre1 et livre2 (IDs différents): ";
    if (*livre1 == *livre2) {
        cout << "ÉGALES" << endl;
    } else {
        cout << "DIFFÉRENTES" << endl;
    }

    delete livreTest;

    cout << "\n=== FIN DU PROGRAMME ===" << endl;

    return 0;
}