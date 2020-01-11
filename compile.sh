mkdir build || echo
cd build
rm -r *
cmake .. || exit 1 #-DBOOST_INCLUDEDIR=$BOOST_I -DBOOST_LIBRARYDIR=$BOOST_L ..
make || exit 1
echo "Stripping the binary executable of unneeded garbage and moving it to root directory..."
strip h && mv h ..
echo "Happy Game Testing!"
#if [ -x mkx.b ]; then #pplus.binx64
#    echo "Found a 64 bit binary file!"
#    strip ./mkx.b #pplus.binx64
#elif [ -x hiddenchest.bin ]; then
#    strip hiddenchest.bin
#fi
