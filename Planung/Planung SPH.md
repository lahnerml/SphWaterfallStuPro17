# Planung des SPH Teils

## Allgemeines

- Der SPH Manager wird auf allen Prozessoren ausgeführt und ist eine Kommunikationsschnittstelle der Prozessoren
- Partikelgeneratoren, Initiator, Exporter sowie andere noch nicht festgelegte Klassen laufen auf dem Master Prozess

## Domänen

- semi-dynamische Domänen
- viele Domänen. Jeder Prozessor übernimmt eine Anzahl davon
    - Es gilt: ProzessorID = DomainID mod AnzahlProzessoren
    - bei 4 Prozessoren gilt also:
        - Prozessor 1 übernimmt Domäne 1, 5, 9, ...
        - Prozessor 2 übernimmt Domäne 2, 6, 10, ...
        - Prozessor 3 übernimmt Domäne 3, 7, 11, ...
        - Prozessor 4 übernimmt Domäne 4, 8, 12, ...


## Einfügen von Partikeln in eine Domäne

Es gibt zwei Möglichekeiten wie Partikel in eine Domain gelangen können
- wenn eine Domain Decomposition durchgeführt wird
    - Wird ein Partikel vom DomainDecomposer gesender, so findet eine Domain Decomposition statt und der DomainDecomposer berechnet die ProzessorID des Empfangenden Prozesses. Gesendete Partikel landen daraufhin im Buffer des Prozesses und nach der Sendephase findet im DomainDecomposer eine Empfangsphase statt bei der alle gesendeten Partikel aus dem Buffer genommen werden und in die jeweils Richtige Domain eingetragen werden.
- wenn ein Partikel von einem Generator, nach einem Berechnungszyklus, an den SphManager gesendet wird
    - Wird ein Partikel vom einem Generator in eine Domain eingefügt, so wird die ProzessorID für die zuständige DomainID berechnet und der Partikel wird an den SphManager dieses Prozesses gesendet.

## Generieren von Domänen

- Wird ein Partikel in eine Domäne eingefügt, welche noch nicht existiert, wird diese Domäne erzeugt. Welche Domäne erzeugt werden muss kann mithilfe der Hashfunktion erkannt werden.
- Im Optimierungsteil können leer Domänen noch erkannt und aus der berechnung ausgeschlossen werden, solange bis wieder Partikel enthalten sind Domänen werden "deaktiviert" und nicht gelöscht, wenn diese einmal erstellt sind.

## Nachbarschaftssuche

- Nachbarschaftssuche Listensuche (genauer Algorithmus wird noch bestimmt)
- Jede Domäne hält eine Liste für Randpartikel (pointer auf Partikel) zu jeder Seite hin sowie den Ecken, diese sind standartmäßig leer
- Nachbardomänen können Randpartikel requesten, empfängt dann die Liste mithilfe von MPI und speichern diese für zukünftige Berechnungen in einer Liste ab (konkrete Partikel)
    - Wenn die Randpartikelliste leer ist wird diese erstellt und dann gesendet
    - Wenn die Randpartikelliste nicht leer ist wird die Liste an Partikeln gesendet.

## Partikelgeneratoren

- Partikelgeneratoren laufen auf dem Master Prozess

## Baking

- Exporter läuft auf dem Master
- Exporter mit gemeinsamer Interface für Exportieren verschiedener Dateien
- verschiedene Exporter bekommen die gleichen Informationen von der Simulation und sind daraufhin für das handling der Daten zuständig