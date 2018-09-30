if ! [[ -d bin ]]; then
  mkdir bin
fi

g++ -g *.cpp -o bin/runme
