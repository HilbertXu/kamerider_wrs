rm -rf build
mkdir build
cd build
cmake ..
make
cp CamShift_Avatar ../CamShift_Avatar
cd ..
rm -rf build
mkdir build
cp CamShift_Avatar build/CamShift_Avatar
rm CamShift_Avatar
cd build

rm -rf lib
mkdir lib
exe="CamShift_Avatar"
des="$PWD/lib"
deplist=$(ldd $exe | awk  '{if (match($3,"/")){ printf("%s "),$3 } }')
cp $deplist $des
cp -rf ../Resource/PoseDir ./PoseDir

echo "export LD_LIBRARY_PATH=\$PWD/lib && ./CamShift_Avatar -PoseDir \$PWD/PoseDir -Rect 317 93 18 22" >Run.sh
cd .. && cd ..
#tar -zcpPf CamShift_AvatarDemo.tar.gz CamShift_Avatar
