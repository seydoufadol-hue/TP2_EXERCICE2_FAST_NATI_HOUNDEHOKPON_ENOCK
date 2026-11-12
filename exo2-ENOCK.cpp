
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

// =================================
// CLASSE MATIERE
// =================================

class Matiere {
private:
    int id;
    string nom;
    double coefficient;

public:
    Matiere(int i, string n, double c) {
        id = i;
        nom = n;
        coefficient = c;
    }

    string getNom() {
        return nom;
    }

    double getCoefficient() {
        return coefficient;
    }
};

// =================================
// CLASSE NOTE
// =================================

class Note {
private:
    double valeur;
    string type;
    Matiere* matiere;

public:
    Note(double v, string t, Matiere* m) {
        valeur = v;
        type = t;
        matiere = m;
    }

    double getValeur() {
        return valeur;
    }

    Matiere* getMatiere() {
        return matiere;
    }
};

// =================================
// CLASSE UE
// =================================

class UE {
private:
    int id;
    string nom;
    int credit;
    vector<Matiere*> matieres;

public:
    UE(int i, string n, int c) {
        id = i;
        nom = n;
        credit = c;
    }

    void ajouterMatiere(Matiere* m) {
        matieres.push_back(m);
    }

    string getNom() {
        return nom;
    }

    int getCredit() {
        return credit;
    }

    vector<Matiere*> getMatieres() {
        return matieres;
    }

    double calculerMoyenneUE(vector<Note> notes);

    bool validerUE(vector<Note> notes);
};

// =================================
// CLASSE FILIERE
// =================================

class Filiere {
private:
    int id;
    string nom;
    vector<UE*> unites;

public:
    Filiere(int i, string n) {
        id = i;
        nom = n;
    }

    void ajouterUE(UE* ue) {
        unites.push_back(ue);
    }

    string getNom() {
        return nom;
    }

    vector<UE*> getUnites() {
        return unites;
    }
};

// =================================
// CLASSE ETUDIANT
// =================================

class Etudiant {
private:
    int id;
    string nom;
    string prenom;
    string matricule;
    string niveau;
    Filiere* filiere;
    vector<Note> notes;

public:
    Etudiant(int i, string n, string p,
             string m, string niv, Filiere* f) {

        id = i;
        nom = n;
        prenom = p;
        matricule = m;
        niveau = niv;
        filiere = f;
    }

    void ajouterNote(double valeur,
                     string type,
                     Matiere* matiere) {

        notes.push_back(Note(valeur, type, matiere));
    }

    double moyenneMatiere(Matiere* matiere) {

        double somme = 0;
        int nombre = 0;

        for (int i = 0; i < (int)notes.size(); i++) {
            if (notes[i].getMatiere() == matiere) {
                somme += notes[i].getValeur();
                nombre++;
            }
        }

        if (nombre == 0)
            return -1;

        return somme / nombre;
    }

    double calculerMoyenne() {

        double somme = 0;
        int totalCredit = 0;

        vector<UE*> unites = filiere->getUnites();

        for (int i = 0; i < (int)unites.size(); i++) {

            double moyenne =
                unites[i]->calculerMoyenneUE(notes);

            if (moyenne >= 0) {
                somme += moyenne * unites[i]->getCredit();
                totalCredit += unites[i]->getCredit();
            }
        }

        if (totalCredit == 0)
            return -1;

        return somme / totalCredit;
    }

    bool validerUE(UE* ue) {
        return ue->validerUE(notes);
    }

    void afficherReleve() {

        cout << "\n========== RELEVE DE NOTES ==========\n";

        cout << "Nom : " << nom << endl;
        cout << "Prenom : " << prenom << endl;
        cout << "Matricule : " << matricule << endl;
        cout << "Niveau : " << niveau << endl;
        cout << "Filiere : " << filiere->getNom() << endl;

        vector<UE*> unites = filiere->getUnites();

        for (int i = 0; i < (int)unites.size(); i++) {

            UE* ue = unites[i];

            cout << "\nUE : " << ue->getNom() << endl;

            vector<Matiere*> matieres = ue->getMatieres();

            for (int j = 0; j < (int)matieres.size(); j++) {

                double moyenne = moyenneMatiere(matieres[j]);

                cout << "Matiere : "
                     << matieres[j]->getNom();

                if (moyenne >= 0)
                    cout << " | Moyenne : " << moyenne << "/20";
                else
                    cout << " | Aucune note";

                cout << endl;
            }

            double moyenneUE = ue->calculerMoyenneUE(notes);

            if (moyenneUE >= 0) {
                cout << "Moyenne UE : " << moyenneUE << "/20";

                if (validerUE(ue))
                    cout << " | VALIDEE";
                else
                    cout << " | NON VALIDEE";

                cout << endl;
            }
            else {
                cout << "UE INCOMPLETE" << endl;
            }
        }

        double moyenne = calculerMoyenne();

        cout << "\nMoyenne generale : ";

        if (moyenne >= 0)
            cout << moyenne << "/20" << endl;
        else
            cout << "Non disponible" << endl;

        cout << "=====================================\n";
    }
};

// =================================
// METHODES DE LA CLASSE UE
// =================================

double UE::calculerMoyenneUE(vector<Note> notes) {

    double somme = 0;
    double totalCoefficient = 0;

    for (int i = 0; i < (int)matieres.size(); i++) {

        double sommeNotes = 0;
        int nombre = 0;

        for (int j = 0; j < (int)notes.size(); j++) {

            if (notes[j].getMatiere() == matieres[i]) {
                sommeNotes += notes[j].getValeur();
                nombre++;
            }
        }

        if (nombre == 0)
            return -1;

        double moyenne = sommeNotes / nombre;

        somme += moyenne * matieres[i]->getCoefficient();

        totalCoefficient += matieres[i]->getCoefficient();
    }

    if (totalCoefficient == 0)
        return -1;

    return somme / totalCoefficient;
}

bool UE::validerUE(vector<Note> notes) {

    double moyenne = calculerMoyenneUE(notes);

    return moyenne >= 10;
}

// =================================
// FONCTION PRINCIPALE
// =================================

int main() {

    cout << fixed << setprecision(2);

    // Creation de la filiere
    Filiere f1(1, "Informatique");

    // Creation des matieres
    Matiere m1(1, "Mathematiques", 3);
    Matiere m2(2, "Informatique", 2);
    Matiere m3(3, "Physique", 1);

    // Creation de l'UE
    UE ue1(1, "UE Fondamentale", 6);

    // Ajouter les matieres
    ue1.ajouterMatiere(&m1);
    ue1.ajouterMatiere(&m2);
    ue1.ajouterMatiere(&m3);

    // Ajouter l'UE a la filiere
    f1.ajouterUE(&ue1);

    // Creation de l'etudiant
    Etudiant e1(1, "ADJOVI", "Paul",
                "ET2026001", "L1", &f1);

    // Ajouter les notes
    e1.ajouterNote(12, "Interrogation", &m1);
    e1.ajouterNote(14, "Examen", &m1);

    e1.ajouterNote(10, "Interrogation", &m2);
    e1.ajouterNote(16, "Examen", &m2);

    e1.ajouterNote(11, "Examen", &m3);

    // Afficher le releve
    e1.afficherReleve();

    return 0;
}

