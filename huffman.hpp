#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>

namespace Huffman {

    // --------------------------------------------------------------------------
    // Struktur für einen Knoten im Huffman-Baum
    // --------------------------------------------------------------------------
    struct Node {
        char ch;         // Das Zeichen (nur in Blattknoten relevant)
        int freq;        // Häufigkeit des Zeichens
        Node* left;      // linker Teilbaum
        Node* right;     // rechter Teilbaum

        // Konstruktor für Blattknoten
        Node(char character, int frequency);
        // Konstruktor für interne Knoten (Vereinigung zweier Teilbäume)
        Node(Node* left, Node* right);
    };

    // --------------------------------------------------------------------------
    // Vergleichsstruktur für die Prioritätswarteschlange (min-heap)
    // --------------------------------------------------------------------------
    struct Compare {
        bool operator()(const Node* a, const Node* b) const;
    };

    // --------------------------------------------------------------------------
    // Erzeugt rekursiv für jeden Blattknoten einen Huffman-Code.
    // 'code' speichert den aktuell aufgebauten Code, 'codes' die Zuordnung von
    // Zeichen zu Codewörtern.
    // --------------------------------------------------------------------------
    void buildCodes(Node* root, const std::string& code, std::unordered_map<char, std::string>& codes);

    // --------------------------------------------------------------------------
    // Erzeugt den Huffman-Baum anhand einer Häufigkeitstabelle.
    // --------------------------------------------------------------------------
    Node* buildHuffmanTree(const std::unordered_map<char, int>& freq);

    // --------------------------------------------------------------------------
    // Gibt den dynamisch allokierten Huffman-Baum wieder frei (rekursiv).
    // --------------------------------------------------------------------------
    void deleteTree(Node* root);

    // --------------------------------------------------------------------------
    // Serialisiert den Huffman-Baum in einen String (Preorder-Darstellung).
    // Bei einem Blatt wird '1' und das Zeichen gespeichert, bei einem inneren Knoten
    // wird '0' gespeichert, gefolgt von der Serialisierung des linken und rechten
    // Teilbaums.
    // --------------------------------------------------------------------------
    void serializeTree(Node* root, std::string &out);

    // --------------------------------------------------------------------------
    // Deserialisiert den Huffman-Baum aus einem String.
    // 'index' gibt die aktuelle Position im String an (wird rekursiv erhöht).
    // --------------------------------------------------------------------------
    Node* deserializeTree(const std::string &in, size_t &index);

    // --------------------------------------------------------------------------
    // Komprimiert eine Datei mittels Huffman-Algorithmus.
    // Dabei wird im Header neben den komprimierten Daten auch der serialisierte Baum
    // sowie die ursprüngliche Dateiendung gespeichert.
    // Die Ausgabedatei wird automatisch benannt:
    //   <ursprünglicher Dateiname ohne Endung>_komprimiert.huff
    // --------------------------------------------------------------------------
    void compressFile(const std::string &inputFilename);

    // --------------------------------------------------------------------------
    // Dekomprimiert eine mit dem Huffman-Algorithmus komprimierte Datei.
    // Aus dem Header werden der serialisierte Baum und die ursprüngliche Dateiendung
    // ausgelesen. Die Ausgabedatei wird automatisch benannt:
    //   <ursprünglicher Dateiname ohne Endung>_entpackt.<ursprüngliche Endung>
    // --------------------------------------------------------------------------
    void decompressFile(const std::string &inputFilename);

} // namespace Huffman

#endif // HUFFMAN_HPP
