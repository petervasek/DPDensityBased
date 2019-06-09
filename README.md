# DPDensityBased
Code result of diploma thesis

## Výsledkom práce sú:
 * C++ projekt s core algoritmom.
 * R knižnica obsahujúca vo forme Rcpp core algoritmus. Pre modifikovanie kódu a väčšiu rýchlosť sa ale odporúča spúšťanie priamo C++ programu.
 * Zbierka R skriptov na manipuláciu so vstupnými dátami, predspracovanie pre vstup algoritmu, vyhodnotenie a vizualizáciu výsledkov.

Proces manipulácie s našim riešením môžeme rozdeliť na 3 pomyselné časti:
 * Načítanie a analýza fcs dát, ich transformácia do podoby použiteľnej ako vstup algoritmu (či už R knižnice, alebo surového vstupu do C++ programu).
 * Vykonanie / vykonávanie zhlukovania.
 * Práca s výsledkami.

Nevyhnutnou podmienkou pre vykonanie hociktorej z častí je nainštalovanie jazyka R (https://www.r-project.org/), ideálne v aktuálnej verzií 3.6.0.
Ďalším odporúčaným softvérom je:
 * R Studio (https://www.rstudio.com/products/rstudio/), ideálne v aktuálnej verzií 1.2.1335.
 * C++ kompilátor s podporou OpenMP.
 * C++ IDE podľa vlastnej chuti. 

## Načítanie a analýza fcs dát:
Pre načítanie fcs súborov v R jazyku potrebujeme nainštalovanú knižnicu "flowCore". Následne si vieme uložiť do premennej ľubovoľný vstupný cytometrický súbor ako:
fcs_read = read.FCS("path");.
Matica expresivity génov v načítanom súbore je premenná objektu fcs_read@exprs. V fcs_read@parameters@data$name si vieme pozrieť, ktorý atribút predstavuje ktorý proteín. Ak chceme získať predstavu o rozložení hustoty v stavovom priestore, môžeme použiť algoritmus OPTICS. Ten sa nachádza v knižnici "dbscan". Kód pre túto časť manipulácie s dátami pokrýva skript "readFCSplotOPTICS.R" priložený na elektronickom nosiči.

## Vykonanie zhlukovania:
Zhlukovanie je možné spustiť buď ako funkciu R knižnice DPDensityBased::metaCluster() s prípustnými argumentmi:
 * file - meno vstupného súboru.
 * eps - parameter eps pre zhlukovanie.
 * minPts - parameter minPts pre zhlukovanie.
 * outputFile - explicitné stanovenie názvu výstupného súboru.
 * saveStatusOutputFile - súbor do ktorého chceme dostať výsledný stav zhlukovania pre budúce dynamické použitie.
 * loadFromFile - súbor z ktorého chceme načítať stav zhlukovania pred začatím zhlukovania.

Alebo klasickým spustením kompilátorom vytvoreného DPDensityBased.exe so vstupnými parametrami rovnakými ako vyššie pri R volaní. Priložené binárne súbory by nemuseli fungovať pod inou architektúrou alebo operačným systémom, odporúča sa radšej si vybuildovať projekt nanovo.
## 
Práca s výsledkami:
Po prebehnutí algoritmu a obdržaní výstupného súboru máme niekoľko možností:
 * Vizualizácia cez dashboard priložený v DPDensityBased knižnici volaním ::visualize() s parametrom premennou výstupného zhlukovania.
 * Vyhodnotenie kvality zhlukovania validačnými indexami Rand, Jaccard a Fowlkes-Mallows. Nástrojom na to je priložený skript "processResultIndexes.r". 
 * Priama vizualizácia cez ggplot knižnicu, nástrojom je priložený skript "visualization.R".
 * Vizualizácia pomocou PCA alebo t-SNE, priložené skripty "pcaScript.R" a "Rtsne.R"
 * Pre skúsenejších možnosť porovnať hodnoty s hodnotami iných zhlukovacích algoritmov. V repertoári máme skript pre K-Means, flowSOM a RphenoGraph, nainštalovať si ich ale je potrebné manuálne, keďže Bioconductor zatiaľ oficiálne nepodporuje najnovšiu verziu Rtools.

Pre kompilovanie Rcpp knižnice je potrebné explicitne nastaviť kompilátoru podporu pre C++11 napríklad nastavením "Sys.setenv("PKG_CXXFLAGS"="-std=c++11")". Pre podporu OpenMP kompilátorom zasa nesmieme zabudnúť na argument "-fopenmp".
