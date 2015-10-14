#!/bin/bash
#shell param

#cd public
#cd commoncpp2-1.8.1
#make
#cd ./src/.libs
#cp -a *.so* $TEDO_HOME/lib
cd $TEDO_HOME/public
cd OracleDb
make
cd ../rc2crypto
make 
cd ../utility
make 
cd ../base_component
make 
cd ../libclient
make 