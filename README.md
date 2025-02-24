# Huffman
Huffman Kompressor / Dekompressor - Endanwenderanleitung
---------------------------------------------------------

Dieses Programm ermöglicht es, Dateien mithilfe des Huffman-Algorithmus zu komprimieren
und wieder zu dekomprimieren. Dabei wird der vollständige Huffman-Baum sowie die
ursprüngliche Dateiendung im Header gespeichert, sodass beim Dekomprimieren die Originaldatei
wiederhergestellt werden kann.

Bedienungsanleitung:
--------------------
1. Programm starten:
   - Öffnen Sie ein Terminal und führen Sie das Programm aus:
         ./huffman

2. Moduswahl:
   - Nach dem Start werden Sie gefragt, ob Sie komprimieren (c) oder dekomprimieren (d) möchten.
   - Geben Sie "c" für Komprimierung oder "d" für Dekomprimierung ein und bestätigen Sie mit Enter.

3. Dateipfad eingeben:
   - Geben Sie den vollständigen Pfad der Datei ein, die Sie verarbeiten möchten.
     Beispiele:
         Komprimieren:   /home/user/Dokumente/beispiel.txt
         Dekomprimieren: /home/user/Dokumente/beispiel_komprimiert.huff

4. Ausgabe:
   - Beim Komprimieren wird im selben Verzeichnis eine Datei mit dem Namen
         <Dateiname ohne Endung>_komprimiert.huff
     erstellt.
   - Beim Dekomprimieren wird im selben Verzeichnis eine Datei mit dem Namen
         <ursprünglicher Dateiname ohne Suffix>_entpackt.<ursprüngliche Endung>
     erstellt.

Mögliche Fehlermeldungen:
-------------------------
1. "Fehler beim Öffnen der Eingabedatei: <Pfad>"
   - Tritt auf, wenn die angegebene Datei nicht gefunden wird oder nicht geöffnet werden kann.

2. "Fehler beim Öffnen der Ausgabedatei: <Pfad>"
   - Tritt auf, wenn die Ausgabedatei nicht erstellt oder geöffnet werden kann (z. B. wegen fehlender Schreibrechte).

3. "Leere Datei, nichts zu komprimieren."
   - Tritt auf, wenn die zu komprimierende Datei keine Inhalte enthält.

4. "Datei ist kein gültiges Huffman-komprimiertes Format."
   - Tritt auf, wenn beim Dekomprimieren der Header nicht dem erwarteten Format entspricht, d. h. die Datei ist entweder beschädigt oder wurde nicht korrekt komprimiert.

5. "Fehler beim Wiederaufbau des Huffman-Baums."
   - Tritt auf, wenn der im Header gespeicherte, serialisierte Baum nicht korrekt gelesen oder interpretiert werden kann.

6. "Ungültiger Modus. Bitte 'c' oder 'd' eingeben."
   - Tritt auf, wenn bei der Moduswahl ein falscher Buchstabe eingegeben wird.

7. "Fehler beim Öffnen der komprimierten Datei: <Pfad>"
   - Tritt auf, wenn die zu dekomprimierende Datei nicht gefunden wird oder nicht geöffnet werden kann.

Hinweise:
---------
- Achten Sie darauf, dass Sie die richtigen Dateipfade und Dateinamen angeben.
- Zum Dekomprimieren muss die Eingabedatei eine komprimierte Datei mit der Endung ".huff" sein.
- Prüfen Sie bei Fehlern die Zugriffsrechte der betroffenen Dateien.

Viel Erfolg mit dem Huffman Kompressor / Dekompressor!

