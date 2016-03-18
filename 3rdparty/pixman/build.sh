#!/bin/sh
cwd=`pwd`
src="pixman-0.34.0"
if [ "$1" = "" ]; then
  dest=`pwd`/../build
else
  dest=$1
fi
mkdir -p ${dest}/src
cd ${dest}/src

if [ ! -d "${src}" ]; then
  if [ `which curl` ]; then
    cmd="curl -O"
  else
    cmd="wget"
  fi
  ${cmd} http://cairographics.org/releases/${src}.tar.gz
  tar -xf ${src}.tar.gz
fi

cd ${src}

./configure --prefix=${dest} --libdir=${dest}/lib \
  --disable-shared --enable-static --with-pic
make -j4
make install

cd ${cwd}
