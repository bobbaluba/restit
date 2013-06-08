# build package and run restit-program

cd src/build/
qmake ..
make

fileName="restit_$(date +%Y%m%d_%H%M%S)"

./tetrix >> $fileName.log