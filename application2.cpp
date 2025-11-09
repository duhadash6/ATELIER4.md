#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

// Déclaration anticipée
class CompteBancaire;
class AgentBancaire;
class Banque;

// Classe Client
class Client {
private:
    string nom;
    string CIN;
    int identifiantClient;

    // Constructeur privé pour contrôler la création
    Client(string nom, string CIN, int identifiant) 
        : nom(nom), CIN(CIN), identifiantClient(identifiant) {}

public:
    // Getters basiques
    string getNom() const { return nom; }
    int getIdentifiant() const { return identifiantClient; }

    // Fonctions amies pour accès contrôlé
    friend class Banque;
    friend class AgentBancaire;
    
    // Factory method pour création contrôlée
    static shared_ptr<Client> creerClient(string nom, string CIN, int identifiant) {
        return make_shared<Client>(nom, CIN, identifiant);
    }

    void afficherInfos() const {
        cout << "Client: " << nom << " | CIN: " << CIN << " | ID: " << identifiantClient << endl;
    }
};

// Classe CompteBancaire
class CompteBancaire {
private:
    string numeroCompte;
    double solde;
    int codeSecret;
    shared_ptr<Client> proprietaire;

    // Constructeur privé
    CompteBancaire(string numero, double soldeInitial, int code, shared_ptr<Client> client)
        : numeroCompte(numero), solde(soldeInitial), codeSecret(code), proprietaire(client) {}

public:
    // Getters basiques (sans accès au code secret)
    string getNumeroCompte() const { return numeroCompte; }
    double getSolde() const { return solde; }
    shared_ptr<Client> getProprietaire() const { return proprietaire; }

    // Méthodes publiques pour opérations non sensibles
    bool deposer(double montant) {
        if (montant > 0) {
            solde += montant;
            return true;
        }
        return false;
    }

    bool retirer(double montant) {
        if (montant > 0 && montant <= solde) {
            solde -= montant;
            return true;
        }
        return false;
    }

    // Fonctions amies pour accès aux données sensibles
    friend class AgentBancaire;
    friend class Banque;

    // Factory method
    static shared_ptr<CompteBancaire> creerCompte(string numero, double soldeInitial, 
                                                 int code, shared_ptr<Client> client) {
        return make_shared<CompteBancaire>(numero, soldeInitial, code, client);
    }

    void afficherInfosPubliques() const {
        cout << "Compte: " << numeroCompte << " | Solde: " << solde 
             << " | Propriétaire: " << proprietaire->getNom() << endl;
    }
};

// Classe AgentBancaire
class AgentBancaire {
private:
    string nomAgent;
    int identifiantAgent;

public:
    AgentBancaire(string nom, int id) : nomAgent(nom), identifiantAgent(id) {}

    // Méthodes pour opérations confidentielles
    bool verifierCodeSecret(shared_ptr<CompteBancaire> compte, int codeSaisi) const {
        return compte->codeSecret == codeSaisi;
    }

    void afficherInfosConfidentielles(shared_ptr<CompteBancaire> compte) const {
        cout << "=== INFORMATIONS CONFIDENTIELLES ===" << endl;
        cout << "Compte: " << compte->numeroCompte << endl;
        cout << "Solde: " << compte->solde << endl;
        cout << "Code secret: " << compte->codeSecret << endl;
        cout << "Propriétaire: " << compte->proprietaire->nom << " (CIN: " 
             << compte->proprietaire->CIN << ")" << endl;
    }

    bool effectuerTransfert(shared_ptr<CompteBancaire> source, 
                           shared_ptr<CompteBancaire> destination, 
                           double montant, int codeSecret) {
        
        // Vérification du code secret
        if (!verifierCodeSecret(source, codeSecret)) {
            cout << "❌ Code secret incorrect - Transfert annulé" << endl;
            return false;
        }

        // Vérification du solde
        if (montant <= 0 || montant > source->solde) {
            cout << "❌ Montant invalide ou solde insuffisant" << endl;
            return false;
        }

        // Exécution du transfert
        source->solde -= montant;
        destination->solde += montant;

        cout << "✅ Transfert réussi: " << montant << " de " 
             << source->numeroCompte << " vers " << destination->numeroCompte << endl;
        return true;
    }

    void afficherInfosAgent() const {
        cout << "Agent: " << nomAgent << " | ID: " << identifiantAgent << endl;
    }
};

// Classe Banque
class Banque {
private:
    string nomBanque;
    vector<shared_ptr<Client>> clients;
    vector<shared_ptr<CompteBancaire>> comptes;

public:
    Banque(string nom) : nomBanque(nom) {}

    // Gestion des clients
    shared_ptr<Client> ajouterClient(string nom, string CIN) {
        int nouvelId = clients.size() + 1;
        auto client = Client::creerClient(nom, CIN, nouvelId);
        clients.push_back(client);
        cout << "✅ Client ajouté: " << nom << endl;
        return client;
    }

    // Gestion des comptes
    shared_ptr<CompteBancaire> ouvrirCompte(shared_ptr<Client> client, 
                                           double soldeInitial, int codeSecret) {
        string numeroCompte = "CPT" + to_string(comptes.size() + 1).insert(0, 6 - to_string(comptes.size() + 1).length(), '0');
        auto compte = CompteBancaire::creerCompte(numeroCompte, soldeInitial, codeSecret, client);
        comptes.push_back(compte);
        cout << "✅ Compte ouvert: " << numeroCompte << " pour " << client->getNom() << endl;
        return compte;
    }

    // Méthodes d'audit interne (accès aux données sensibles)
    void genererRapportAudit() const {
        cout << "\n=== RAPPORT D'AUDIT INTERNE - " << nomBanque << " ===" << endl;
        cout << "Nombre de clients: " << clients.size() << endl;
        cout << "Nombre de comptes: " << comptes.size() << endl << endl;

        for (const auto& compte : comptes) {
            cout << "--- COMPTE " << compte->numeroCompte << " ---" << endl;
            cout << "Solde: " << compte->solde << " DH" << endl;
            cout << "Code secret: " << compte->codeSecret << endl;
            cout << "Propriétaire: " << compte->proprietaire->nom << endl;
            cout << "CIN: " << compte->proprietaire->CIN << endl;
            cout << "ID Client: " << compte->proprietaire->identifiantClient << endl << endl;
        }
    }

    // Recherche de comptes
    shared_ptr<CompteBancaire> trouverCompte(string numeroCompte) const {
        for (const auto& compte : comptes) {
            if (compte->getNumeroCompte() == numeroCompte) {
                return compte;
            }
        }
        return nullptr;
    }

    // Affichage public
    void afficherInfosPubliques() const {
        cout << "\n=== INFORMATIONS PUBLIQUES - " << nomBanque << " ===" << endl;
        for (const auto& compte : comptes) {
            compte->afficherInfosPubliques();
        }
    }

    // Getters
    vector<shared_ptr<CompteBancaire>> getComptes() const { return comptes; }
};

// Fonction principale de démonstration
int main() {
    cout << "=== SYSTÈME BANCAIRE SÉCURISÉ ===" << endl;

    // Création de la banque
    Banque banque("Banque Marocaine du Commerce Extérieur");

    // Création des clients
    auto client1 = banque.ajouterClient("Ahmed Alami", "AB123456");
    auto client2 = banque.ajouterClient("Fatima Zahra", "CD789012");
    auto client3 = banque.ajouterClient("Karim Bennani", "EF345678");

    // Ouverture des comptes
    auto compte1 = banque.ouvrirCompte(client1, 5000.0, 1234);
    auto compte2 = banque.ouvrirCompte(client2, 3000.0, 5678);
    auto compte3 = banque.ouvrirCompte(client3, 10000.0, 9012);

    cout << "\n";

    // Création d'un agent bancaire
    AgentBancaire agent("Mohamed Agent", 1001);
    agent.afficherInfosAgent();

    cout << "\n=== SIMULATION D'OPÉRATIONS ===" << endl;

    // Opérations de base
    cout << "\n--- Dépôt sur compte1 ---" << endl;
    compte1->deposer(2000.0);
    compte1->afficherInfosPubliques();

    cout << "\n--- Retrait sur compte2 ---" << endl;
    if (compte2->retirer(500.0)) {
        compte2->afficherInfosPubliques();
    }

    cout << "\n--- Transfert avec code correct ---" << endl;
    agent.effectuerTransfert(compte1, compte2, 1000.0, 1234);

    cout << "\n--- Tentative de transfert avec code incorrect ---" << endl;
    agent.effectuerTransfert(compte1, compte3, 500.0, 9999);

    cout << "\n--- Consultation confidentielle par l'agent ---" << endl;
    agent.afficherInfosConfidentielles(compte1);

    cout << "\n=== AFFICHAGE PUBLIC ===" << endl;
    banque.afficherInfosPubliques();

    cout << "\n=== RAPPORT D'AUDIT (ACCÈS RESTREINT) ===" << endl;
    banque.genererRapportAudit();

    // Test de vérification de code secret
    cout << "\n=== VÉRIFICATION DE SÉCURITÉ ===" << endl;
    cout << "Code 1234 pour compte1: " 
         << (agent.verifierCodeSecret(compte1, 1234) ? "✅ Correct" : "❌ Incorrect") << endl;
    cout << "Code 0000 pour compte1: " 
         << (agent.verifierCodeSecret(compte1, 0000) ? "✅ Correct" : "❌ Incorrect") << endl;

    cout << "\n=== FIN DU SYSTÈME BANCAIRE ===" << endl;

    return 0;
}