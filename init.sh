#!/bin/bash

# Directorio del proyecto
PROJECT_DIR=$(pwd)
BUILD_DIR=${PROJECT_DIR}/build

mkdir -p $BUILD_DIR

# Función para construir en un modo específico
build_mode() {
	local mode=$1
	echo "Compiling $mode..."

	# Ejecutar cmake y make
	cmake -S $PROJECT_DIR -B $BUILD_DIR -DCMAKE_BUILD_TYPE=$mode
	cmake --build $BUILD_DIR

	# Opcional: Strip del binario si es modo Release
	if [ "$mode" == "Release" ]; then
		echo "Stripping binary..."
		strip ${BUILD_DIR}/Enie
	fi

	echo "Finish"
}

run_binary() {
	local mode=$1
	BUILD_MODE_DIR=${BUILD_DIR}/${mode}
	echo "Running..."

	${BUILD_DIR}/Enie /dev/input/event3
}

# Mostrar menú
echo "Choose compilation mode"
PS3="-> "
options=("Debug" "Release" "Quit")
select opt in "${options[@]}"; do
	case $opt in
	"Debug")
		build_mode Debug
		run_binary Debug
		break
		;;
	"Release")
		build_mode Release
		run_binary Release
		break
		;;
	"Quit")
		break
		;;
	*)
		echo "Err $REPLY"
		;;
	esac
done
