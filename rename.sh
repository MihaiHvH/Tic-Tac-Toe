#!/bin/bash

replacement="$1"

if [ -z "$replacement" ]; then
    replacement="Tic-Tac-Toe"
fi

find . -type f -name "*Tic-Tac-Toe*" | while read -r file; do
    dir=$(dirname "$file")
    base=$(basename "$file")
    newbase="${base//Tic-Tac-Toe/$replacement}"
    if [ "$base" != "$newbase" ]; then
        mv -v "$file" "$dir/$newbase"
    fi
done

find . -type f ! -path "./.git/*" -exec grep -Il "" {} + | while read -r file; do
    if grep -q "Tic-Tac-Toe" "$file"; then
        echo "Updating: $file"
        sed -i "s/Tic-Tac-Toe/$replacement/g" "$file"
    fi
done
