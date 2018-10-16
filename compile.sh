if ! [[ -d bin ]]; then
  mkdir bin
fi
cp config.ini ./bin/
g++ -std=c++11 -W -g *.cpp -o bin/runme
