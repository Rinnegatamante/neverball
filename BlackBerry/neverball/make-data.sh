#!/bin/sh

files=( back ball bgm geom gui item map-back map-easy map-hard map-medium png shot-easy shot-hard shot-medium snd textures ttf )
sets=( set-easy.txt set-medium.txt set-hard.txt )

rm -rf data
mkdir data

for i in "${files[@]}"
do
	cp -r "../../data/$i" data
done

for i in "${sets[@]}"
do
	cp "../../data/$i" data
	echo "$i" >> data/sets.txt
done

find data -type f \! \( -path '*.jpg' -o -path '*.png' -o -path '*.ogg' -o -path '*.sol' -o -path '*.ttf' -o -path '*.txt' -o -path '*.nbr' \) -delete
