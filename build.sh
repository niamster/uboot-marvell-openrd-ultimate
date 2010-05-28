#!/bin/bash

function config () {
	make OpenRDUltimate88f6281a_config LE=1 NBOOT=1
}

function build () {
	rm -rf u-boot*
	make
	cp u-boot-OpenRDUltimate88f6281a_400rd_nand.bin /var/tftp/tftpboot/u-boot.bin.openrd
	cp u-boot-OpenRDUltimate88f6281a_400rd_nand.bin /projects/openrd/openocd/u-boot.bin.openrd
	cp u-boot-OpenRDUltimate88f6281a /projects/openrd/openocd/u-boot.elf.openrd
}

function clean () {
	make mrproper
}

while [ $# -gt 0 ]
do
	case $1 in
		clean)
			clean
			;;
		config)
			config
			;;
		build)
			build
			;;
	esac
	shift
done
