#include "huffman.hpp"
#include <cstring>   // Für std::strncmp
#include <sstream>   // Für std::ostringstream

// Hilfsfunktionen für Dateipfadmanipulation (nur in dieser Datei sichtbar)
namespace {
    // Extrahiert das Verzeichnis (inkl. Schrägstrich) aus einem Pfad.
    std::string getDirectory(const std::string &filepath) {
        size_t pos = filepath.find_last_of("/\\");
        if (pos != std::string::npos)
            return filepath.substr(0, pos + 1);
        else
            return "";
    }

    // Extrahiert den Dateinamen (ohne Verzeichnis) aus einem Pfad.
    std::string getFilename(const std::string &filepath) {
        size_t pos = filepath.find_last_of("/\\");
        if (pos != std::string::npos)
            return filepath.substr(pos + 1);
        else
            return filepath;
    }

    // Entfernt die Dateiendung aus einem Dateinamen.
    std::string removeExtension(const std::string &filename) {
        size_t pos = filename.find_last_of('.');
        if (pos != std::string::npos)
            return filename.substr(0, pos);
        else
            return filename;
    }

    // Gibt die Dateiendung (ohne Punkt) zurück.
    std::string getExtension(const std::string &filename) {
        size_t pos = filename.find_last_of('.');
        if (pos != std::string::npos)
            return filename.substr(pos + 1);
        else
            return "";
    }
}

namespace Huffman {

    // --------------------------------------------------------------------------
    // Node-Konstruktor für Blattknoten
    // --------------------------------------------------------------------------
    Node::Node(char character, int frequency)
        : ch(character), freq(frequency), left(nullptr), right(nullptr) {}

    // --------------------------------------------------------------------------
    // Node-Konstruktor für interne Knoten
    // --------------------------------------------------------------------------
    Node::Node(Node* l, Node* r)
        : ch('\0'), freq(l->freq + r->freq), left(l), right(r) {}

    // --------------------------------------------------------------------------
    // Vergleichsoperator für die Prioritätswarteschlange
    // --------------------------------------------------------------------------
    bool Compare::operator()(const Node* a, const Node* b) const {
        return a->freq > b->freq;
    }

    // --------------------------------------------------------------------------
    // Erzeugt rekursiv die Huffman-Codes für jedes Zeichen.
    // --------------------------------------------------------------------------
    void buildCodes(Node* root, const std::string& code, std::unordered_map<char, std::string>& codes) {
        if (!root)
            return;
        // Blattknoten gefunden: speichere das Zeichen und den zugehörigen Code
        if (!root->left && !root->right) {
            // Sonderfall: Falls nur ein Zeichen vorhanden ist, weise "0" zu
            codes[root->ch] = code.empty() ? "0" : code;
        }
        buildCodes(root->left, code + "0", codes);
        buildCodes(root->right, code + "1", codes);
    }

    // --------------------------------------------------------------------------
    // Erzeugt den Huffman-Baum anhand der Häufigkeitstabelle.
    // --------------------------------------------------------------------------
    Node* buildHuffmanTree(const std::unordered_map<char, int>& freq) {
        std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
        for (auto pair : freq) {
            pq.push(new Node(pair.first, pair.second));
        }
        if (pq.empty())
            return nullptr; // Leere Datei

        while (pq.size() > 1) {
            Node* left = pq.top();
            pq.pop();
            Node* right = pq.top();
            pq.pop();
            Node* parent = new Node(left, right);
            pq.push(parent);
        }
        return pq.top();
    }

    // --------------------------------------------------------------------------
    // Gibt den Huffman-Baum (rekursiv) frei.
    // --------------------------------------------------------------------------
    void deleteTree(Node* root) {
        if (!root)
            return;
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
    }

    // --------------------------------------------------------------------------
    // Serialisiert den Huffman-Baum in einen String (Preorder-Traversal).
    // Bei Blattknoten: speichere '1' gefolgt vom Zeichen.
    // Bei inneren Knoten: speichere '0' und rufe rekursiv für linken und rechten Teilbaum auf.
    // --------------------------------------------------------------------------
    void serializeTree(Node* root, std::string &out) {
        if (!root)
            return;
        if (!root->left && !root->right) {
            // Blattknoten: '1' + Zeichen
            out.push_back('1');
            out.push_back(root->ch);
        } else {
            // Interner Knoten: '0'
            out.push_back('0');
            serializeTree(root->left, out);
            serializeTree(root->right, out);
        }
    }

    // --------------------------------------------------------------------------
    // Deserialisiert den Huffman-Baum aus einem String.
    // 'index' gibt die aktuelle Position im String an.
    // --------------------------------------------------------------------------
    Node* deserializeTree(const std::string &in, size_t &index) {
        if (index >= in.size())
            return nullptr;

        char flag = in[index++];
        if (flag == '1') {
            // Blattknoten: nächstes Zeichen
            char ch = in[index++];
            return new Node(ch, 0); // Häufigkeit ist hier irrelevant
        } else {
            // Interner Knoten
            Node* left = deserializeTree(in, index);
            Node* right = deserializeTree(in, index);
            return new Node(left, right);
        }
    }

    // --------------------------------------------------------------------------
    // Komprimiert eine Datei mittels Huffman-Algorithmus.
    // - Liest die Eingabedatei ein und erstellt eine Häufigkeitstabelle.
    // - Baut den Huffman-Baum und die zugehörigen Codes auf.
    // - Serialisiert den Baum und speichert ihn zusammen mit der ursprünglichen
    //   Dateiendung und der Gesamtzahl der Zeichen im Header.
    // - Schreibt danach die komprimierten Bitdaten in die Ausgabedatei.
    //
    // Die Ausgabedatei wird automatisch benannt:
    //   <Verzeichnis>/<Dateiname ohne Endung>_komprimiert.huff
    // --------------------------------------------------------------------------
    void compressFile(const std::string &inputFilename) {
        std::ifstream inFile(inputFilename, std::ios::binary);
        if (!inFile) {
             std::cerr << "Fehler beim Öffnen der Eingabedatei: " << inputFilename << "\n";
             return;
        }

        // Lese die gesamte Datei und erstelle die Häufigkeitstabelle
        std::unordered_map<char, int> freq;
        char ch;
        int totalChars = 0;
        while (inFile.get(ch)) {
             freq[ch]++;
             totalChars++;
        }
        if(totalChars == 0){
             std::cerr << "Leere Datei, nichts zu komprimieren.\n";
             inFile.close();
             return;
        }
        inFile.clear();
        inFile.seekg(0, std::ios::beg);

        // Erzeuge den Huffman-Baum und die zugehörigen Codes
        Node* root = buildHuffmanTree(freq);
        std::unordered_map<char, std::string> codes;
        buildCodes(root, "", codes);

        // Serialisiere den Huffman-Baum in einen String
        std::string serializedTree;
        serializeTree(root, serializedTree);

        // Ermittle die ursprüngliche Dateiendung
        std::string filename = getFilename(inputFilename);
        std::string origExt = getExtension(filename);

        // Berechne den Ausgabedateinamen: <Verzeichnis>/<Name ohne Endung>_komprimiert.huff
        std::string directory = getDirectory(inputFilename);
        std::string baseName  = removeExtension(filename);
        std::string outputFilename = directory + baseName + "_komprimiert.huff";

        std::ofstream outFile(outputFilename, std::ios::binary);
        if (!outFile) {
             std::cerr << "Fehler beim Öffnen der Ausgabedatei: " << outputFilename << "\n";
             deleteTree(root);
             return;
        }

        // ---------------------------
        // Schreibe den Header:
        // ---------------------------
        // 1. Magic-String (7 Bytes): "HUFTREE"
        outFile.write("HUFTREE", 7);

        // 2. Ursprüngliche Dateiendung: Zuerst die Länge (int), dann die Endung (ohne Punkt)
        int extLen = origExt.size();
        outFile.write(reinterpret_cast<const char*>(&extLen), sizeof(extLen));
        if(extLen > 0)
            outFile.write(origExt.c_str(), extLen);

        // 3. Serialisierter Baum: Zuerst die Länge (int), dann der Baum als String
        int treeLen = serializedTree.size();
        outFile.write(reinterpret_cast<const char*>(&treeLen), sizeof(treeLen));
        if(treeLen > 0)
            outFile.write(serializedTree.c_str(), treeLen);

        // 4. Gesamtzahl der ursprünglichen Zeichen (int)
        outFile.write(reinterpret_cast<const char*>(&totalChars), sizeof(totalChars));

        // ---------------------------
        // Schreibe die komprimierten Daten:
        // Packe die Bitfolge in einen Byte-Puffer.
        unsigned char buffer = 0;
        int bitCount = 0;
        while (inFile.get(ch)) {
             std::string code = codes[ch];
             for (char bit : code) {
                 buffer = buffer << 1;
                 if (bit == '1')
                     buffer |= 1;
                 bitCount++;
                 if (bitCount == 8) {
                     outFile.put(buffer);
                     buffer = 0;
                     bitCount = 0;
                 }
             }
        }
        // Falls Bits übrig bleiben, mit Nullen auffüllen
        if (bitCount > 0) {
             buffer = buffer << (8 - bitCount);
             outFile.put(buffer);
        }

        inFile.close();
        outFile.close();
        deleteTree(root);
        std::cout << "Datei erfolgreich komprimiert: " << outputFilename << "\n";
    }

    // --------------------------------------------------------------------------
    // Dekomprimiert eine komprimierte Datei.
    // - Liest den Header, um den serialisierten Baum, die ursprüngliche Endung und
    //   die Gesamtzahl der Zeichen auszulesen.
    // - Baut den Huffman-Baum über Deserialisierung wieder auf.
    // - Dekodiert die Bitfolge in die ursprünglichen Zeichen.
    // - Die Ausgabedatei wird automatisch benannt:
    //   <Verzeichnis>/<Name ohne Endung>_entpackt.<ursprüngliche Endung>
    //   Falls keine Endung im Header gespeichert wurde, wird ".dat" verwendet.
    // --------------------------------------------------------------------------
    void decompressFile(const std::string &inputFilename) {
        std::ifstream inFile(inputFilename, std::ios::binary);
        if (!inFile) {
             std::cerr << "Fehler beim Öffnen der komprimierten Datei: " << inputFilename << "\n";
             return;
        }

        // ---------------------------
        // Lese den Header:
        // ---------------------------
        char magic[7];
        inFile.read(magic, 7);
        if (inFile.gcount() != 7 || std::strncmp(magic, "HUFTREE", 7) != 0) {
             std::cerr << "Datei ist kein gültiges Huffman-komprimiertes Format.\n";
             inFile.close();
             return;
        }

        // Lese die ursprüngliche Dateiendung
        int extLen = 0;
        inFile.read(reinterpret_cast<char*>(&extLen), sizeof(extLen));
        std::string origExt;
        if (extLen > 0) {
            origExt.resize(extLen);
            inFile.read(&origExt[0], extLen);
        }
        // Falls keine Endung gespeichert wurde, verwende ".dat"
        if (origExt.empty())
            origExt = "dat";

        // Lese den serialisierten Baum
        int treeLen = 0;
        inFile.read(reinterpret_cast<char*>(&treeLen), sizeof(treeLen));
        std::string serializedTree;
        if (treeLen > 0) {
            serializedTree.resize(treeLen);
            inFile.read(&serializedTree[0], treeLen);
        }

        // Lese die Gesamtzahl der ursprünglichen Zeichen
        int totalChars = 0;
        inFile.read(reinterpret_cast<char*>(&totalChars), sizeof(totalChars));

        // Deserialisiere den Baum
        size_t index = 0;
        Node* root = deserializeTree(serializedTree, index);
        if (!root) {
             std::cerr << "Fehler beim Wiederaufbau des Huffman-Baums.\n";
             inFile.close();
             return;
        }

        // Berechne den Ausgabedateinamen:
        // Entferne aus dem Eingabedateinamen (z.B. "beispiel_komprimiert.huff")
        // den Suffix "_komprimiert" und hänge "_entpackt.<origExt>" an.
        std::string filename = getFilename(inputFilename);
        std::string baseName = removeExtension(filename);
        std::string suffix = "_komprimiert";
        if (baseName.size() >= suffix.size() &&
            baseName.compare(baseName.size() - suffix.size(), suffix.size(), suffix) == 0) {
            baseName = baseName.substr(0, baseName.size() - suffix.size());
        }
        std::string directory = getDirectory(inputFilename);
        std::string outputFilename = directory + baseName + "_entpackt." + origExt;

        std::ofstream outFile(outputFilename, std::ios::binary);
        if (!outFile) {
             std::cerr << "Fehler beim Öffnen der Ausgabedatei: " << outputFilename << "\n";
             deleteTree(root);
             inFile.close();
             return;
        }

        // ---------------------------
        // Dekodiere die Bitdaten:
        // Lies Byte für Byte, navigiere im Baum entsprechend den Bits,
        // bis alle 'totalChars' Zeichen dekodiert wurden.
        Node* current = root;
        int decodedChars = 0;
        char byte;
        while (inFile.get(byte) && decodedChars < totalChars) {
             for (int i = 7; i >= 0 && decodedChars < totalChars; i--) {
                 bool bit = ((byte >> i) & 1);
                 current = bit ? current->right : current->left;
                 if (!current->left && !current->right) {
                     outFile.put(current->ch);
                     decodedChars++;
                     current = root;
                 }
             }
        }

        inFile.close();
        outFile.close();
        deleteTree(root);
        std::cout << "Datei erfolgreich dekomprimiert: " << outputFilename << "\n";
    }

} // namespace Huffman
