#!/bin/sh

files=( back ball bgm geom gui map-ckk map-paxed map-paxed2 map-paxed3 map-putt map-slippi map-vidski shot-putt snd textures ttf )
sets=( holes-putt.txt holes-paxed.txt holes-paxed2.txt holes-paxed3.txt holes-abc.txt holes-slippi.txt holes-kk.txt holes-vidski.txt )

rm -rf data
mkdir data

for i in "${files[@]}"
do
	cp -r "../../data/$i" data
done

for i in "${sets[@]}"
do
	cp "../../data/$i" data
	echo "$i" >> data/courses.txt
done

find data -type f \! \( -path '*.jpg' -o -path '*.png' -o -path '*.ogg' -o -path '*.sol' -o -path '*.ttf' -o -path '*.txt' -o -path '*.nbr' \) -delete
find data/ball -depth -type d \! -name 'basic*' -mindepth 1 -exec rm -rf {} \;
find data/gui -type f \! -name 'back*' -delete
