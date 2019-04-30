# DPDensityBased
Code result of diploma thesis

## Vısledkom práce sú:
	- C++ projekt s core algoritmom.
	- R kninica obsahujúca vo forme Rcpp core algoritmus. Pre modifikovanie kódu a väèšiu rıchlos sa ale odporúèa spúšanie priamo C++ programu.
 	- Zbierka R skriptov na manipuláciu so vstupnımi dátami, predspracovanie pre vstup algoritmu, vyhodnotenie a vizualizáciu vısledkov.

Proces manipulácie s našim riešením môeme rozdeli na 3 pomyselné èasti:
 	- Naèítanie a analıza fcs dát, ich transformácia do podoby pouite¾nej ako vstup algoritmu (èi u R kninice, alebo surového vstupu do C++ programu).
 	- Vykonanie / vykonávanie zhlukovania.
	- Práca s vısledkami.

Nevyhnutnou podmienkou pre vykonanie hociktorej z èastí je nainštalovanie jazyka R (https://www.r-project.org/), ideálne v aktuálnej verzií 3.6.0.
Ïalším odporúèanım softvérom je:
	- R Studio (https://www.rstudio.com/products/rstudio/), ideálne v aktuálnej verzií 1.2.1335.
	- C++ kompilátor s podporou OpenMP.
	- C++ IDE pod¾a vlastnej chuti. 

## Naèítanie a analıza fcs dát:
Pre naèítanie fcs súborov v R jazyku potrebujeme nainštalovanú kninicu "flowCore". Následne si vieme uloi do premennej ¾ubovo¾nı vstupnı cytometrickı súbor ako:
fcs_read = read.FCS("path");.
Matica expresivity génov v naèítanom súbore je premenná objektu fcs_read@exprs. V fcs_read@parameters@data$name si vieme pozrie, ktorı atribút predstavuje ktorı proteín. Ak chceme získa predstavu o rozloení hustoty v stavovom priestore, môeme poui algoritmus OPTICS. Ten sa nachádza v kninici "dbscan". Kód pre túto èas manipulácie s dátami pokrıva skript "readFCSplotOPTICS.R" priloenı na elektronickom nosièi.

## Vykonanie zhlukovania:
Zhlukovanie je moné spusti buï ako funkciu R kninice DPDensityBased::metaCluster() s prípustnımi argumentmi:
	- file - meno vstupného súboru.
	- eps - parameter eps pre zhlukovanie.
	- minPts - parameter minPts pre zhlukovanie.
	- outputFile - explicitné stanovenie názvu vıstupného súboru.
	- saveStatusOutputFile - súbor do ktorého chceme dosta vıslednı stav zhlukovania pre budúce dynamické pouitie.
	- loadFromFile - súbor z ktorého chceme naèíta stav zhlukovania pred zaèatím zhlukovania.

Alebo klasickım spustením kompilátorom vytvoreného DPDensityBased.exe so vstupnımi parametrami rovnakımi ako vyššie pri R volaní. Priloené binárne súbory by nemuseli fungova pod inou architektúrou alebo operaènım systémom, odporúèa sa radšej si vybuildova projekt nanovo.
## 
Práca s vısledkami:
Po prebehnutí algoritmu a obdraní vıstupného súboru máme nieko¾ko moností:
	- Vizualizácia cez dashboard priloenı v DPDensityBased kninici volaním ::visualize() s parametrom premennou vıstupného zhlukovania.
	- Vyhodnotenie kvality zhlukovania validaènımi indexami Rand, Jaccard a Fowlkes-Mallows. Nástrojom na to je priloenı skript "processResultIndexes.r". 
	- Priama vizualizácia cez ggplot kninicu, nástrojom je priloenı skript "visualization.R".
	- Vizualizácia pomocou PCA alebo t-SNE, priloené skripty "pcaScript.R" a "Rtsne.R"
	- Pre skúsenejších monos porovna hodnoty s hodnotami inıch zhlukovacích algoritmov. V repertoári máme skript pre K-Means, flowSOM a RphenoGraph, nainštalova si ich ale je potrebné manuálne, keïe Bioconductor zatia¾ oficiálne nepodporuje najnovšiu verziu Rtools.

Pre kompilovanie Rcpp kninice je potrebné explicitne nastavi kompilátoru podporu pre C++11 napríklad nastavením "Sys.setenv("PKG_CXXFLAGS"="-std=c++11")". Pre podporu OpenMP kompilátorom zasa nesmieme zabudnú na argument "-fopenmp".