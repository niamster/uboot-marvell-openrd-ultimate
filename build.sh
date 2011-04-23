#!/bin/bash

function config () {
	make OpenRDUltimate88f6281a_config LE=1 NBOOT=1
}

function build () {
	rm -rf u-boot*
	make -j4 $@
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
		build-dbg)
			build DEBUG=1
			;;
	esac
	shift
done
