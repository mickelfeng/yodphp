#!/bin/bash

if [ x$1 == x ]
then
	echo "Usage: ./php-build.sh 5.5.5"
	exit 0
fi

tar jxvf ../phpsrc/php-${1}.tar.bz2

cd php-$1

./configure \
--prefix=/usr/local/php/${1}/debug \
--with-config-file-path=/usr/local/php/$1 \
--with-config-file-scan-dir=/usr/local/php/etc \
--with-mysql-dir=/usr \
--with-pdo-mysql=/usr \
--enable-ftp \
--enable-zip \
--with-bz2 \
--with-jpeg-dir=/usr \
--with-png-dir=/usr \
--with-freetype-dir=/usr \
--with-libxml-dir=/usr \
--with-xmlrpc \
--with-zlib-dir=/usr \
--with-gd \
--enable-gd-native-ttf \
--with-curl \
--with-curlwrappers \
--with-ttf \
--with-xsl \
--with-gettext \
--with-pear \
--enable-calendar \
--enable-mbstring \
--enable-bcmath \
--enable-sockets \
--enable-exif \
--with-ncurses \
--with-mcrypt \
--with-mhash \
--with-gmp \
--enable-inline-optimization \
--enable-dbase \
--with-pcre-dir=/usr/bin/pcre-config \
--with-gdbm \
--enable-sigchild \
--enable-sysvsem \
--enable-sysvshm \
--enable-zend-multibyte \
--enable-mbregex \
--enable-wddx \
--enable-shmop \
--enable-soap \
--enable-fastcgi \
--enable-fpm \
--enable-debug

make clean && make && make install

./configure \
--prefix=/usr/local/php/$1 \
--with-config-file-path=/usr/local/php/$1 \
--with-config-file-scan-dir=/usr/local/php/etc \
--with-mysql-dir=/usr \
--with-pdo-mysql=/usr \
--enable-ftp \
--enable-zip \
--with-bz2 \
--with-jpeg-dir=/usr \
--with-png-dir=/usr \
--with-freetype-dir=/usr \
--with-libxml-dir=/usr \
--with-xmlrpc \
--with-zlib-dir=/usr \
--with-gd \
--enable-gd-native-ttf \
--with-curl \
--with-curlwrappers \
--with-ttf \
--with-xsl \
--with-gettext \
--with-pear \
--enable-calendar \
--enable-mbstring \
--enable-bcmath \
--enable-sockets \
--enable-exif \
--with-ncurses \
--with-mcrypt \
--with-mhash \
--with-gmp \
--enable-inline-optimization \
--enable-dbase \
--with-pcre-dir=/usr/bin/pcre-config \
--with-gdbm \
--enable-sigchild \
--enable-sysvsem \
--enable-sysvshm \
--enable-zend-multibyte \
--enable-mbregex \
--enable-wddx \
--enable-shmop \
--enable-soap \
--enable-fastcgi \
--enable-fpm

make clean && make && make install

cd ..

rm -rfv php-${1}

