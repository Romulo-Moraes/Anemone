#! /usr/bin/bash

build_dir='build'
lib_name='anemone.a'

if [ -a "$build_dir" ]
then
    if [ -d "$build_dir" ]
    then
	rm -r "$build_dir"
    else
	rm "$build_dir"
    fi
fi

mkdir "$build_dir"

IFS=' '
read -a private_source_files <<< $(echo ./private/*.c)

for file in "${private_source_files[@]}";
do
    gcc -c "$file" -o "$build_dir/${file:10:-2}.o"
done

cd "$build_dir"

ar rcs "anemone.a" *.o

rm *.o

cd ..


