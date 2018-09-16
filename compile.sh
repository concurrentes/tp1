if ! [[ -d bin ]]; then
  mkdir bin
fi

g++ *.cpp -o bin/runme
