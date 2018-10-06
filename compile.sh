if ! [[ -d bin ]]; then
  mkdir bin
fi

g++ -std=c++11 -W -g *.cpp -o bin/runme
