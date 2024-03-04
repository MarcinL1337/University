#!/bin/bash


ARCHIVE_PATH="/home/marcin/Downloads/linuxes/"


versions=("0.99.15j-1993" "1.1.95-1995" "2.0.27-1997" "2.2.0-1999" "2.4.0-2001" "2.4.20-2003" "2.6.10-2005" "2.6.19-2007" "2.6.28-2009" "2.6.37-2011" "3.7.1-2013" "3.18-2015" "4.9-2017" "4.20-2019" "5.10-2021" "5.15-2023")


for version in "${versions[@]}"
do
  echo "Analizuję wersję $version..."
  
  ARCHIVE="$ARCHIVE_PATH/linux-$version.tar.xz"
  if [ -f "$ARCHIVE" ]; then
    DIRECTORY="$ARCHIVE_PATH/linux-$version"
  	mkdir -p "$DIRECTORY"
    tar -xJf "$ARCHIVE" -C "$DIRECTORY"
    
    if [ -d "$DIRECTORY" ]; then
      # Utwórz katalog na wyniki, jeśli nie istnieje
      mkdir -p "$DIRECTORY/results"
      
      # Analiza cloc
      cloc "$DIRECTORY" --out="$DIRECTORY/results/cloc_results_$version.txt"
      
      # Analiza sloccount
      sloccount "$DIRECTORY" > "$DIRECTORY/results/sloccount_results_$version.txt"
    else
      echo "Nie udało się utworzyć katalogu: $DIRECTORY"
    fi
  else
    echo "Archiwum $ARCHIVE nie istnieje."
  fi
done

