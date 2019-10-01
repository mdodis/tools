pushd dmenu 2> /dev/null
sudo make clean install
popd

pushd dwm 2> /dev/null
sudo make clean install EXTRA_FLAGS=-DDWM_USER_SCRIPT_DIRECTORY="/home/miked/scripts/"
popd

pushd st 2> /dev/null
sudo make clean install
popd
