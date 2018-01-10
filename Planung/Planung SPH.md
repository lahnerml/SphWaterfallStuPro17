# Planung des SPH Teils

## Allgemeines

- Der SPH Manager wird auf allen Prozessoren ausgeführt und ist die Kommunikationsschnittstelle der Prozessoren

## Domänen

- staische Domänen (ändert sich spätestens im Optimierungspart des StuPro)
- viele Domänen. Jeder Prozessor übernimmt eine Anzahl davon
    - bei 4 Prozessoren gilt:
        - Prozessor 1 übernimmt Domäne 1, 5, 9, ...
        - Prozessor 2 übernimmt Domäne 2, 6, 10, ...
        - Prozessor 3 übernimmt Domäne 3, 7, 11, ...
        - Prozessor 4 übernimmt Domäne 4, 8, 12, ...

## Nachbarschaftssuche

- Nachbarschaftssuche Listensuche (genauer Algorithmus wird noch bestimmt)
- Jede Domäne hält eine Liste für Randpartikel (pointer auf Partikel) zu jeder Seite hin sowie den Ecken, diese sind standartmäßig leer
- Nachbardomänen können Randpartikel requesten, empfängt dann die Liste mithilfe von MPI und speichern diese für zukünftige Berechnungen in einer Liste ab (konkrete Partikel)
    - Wenn die Randpartikelliste leer ist wird diese erstellt und dann gesendet
    - Wenn die Randpartikelliste nicht leer ist wird die Liste an Partikeln gesendet.

## Partikelgeneratoren

- Partikelgeneratoren, Initiator, sowie andere noch nicht festgelegte Klassen laufen auf dem Master Prozess

## Baking

- Exporter läuft auf dem Master
- Exporter mit gemeinsamer Interface für Exportieren verschiedener Dateien
- verschiedene Exporter bekommen die gleichen Informationen von der Simulation und sind daraufhin für das handling der Daten zuständig