find . -name '*.ct' -exec grep $1 {} -cH \; | grep '\(:[^0]\)' | sed 's/\(.*\):\(.*\)/\1\t\2/'
