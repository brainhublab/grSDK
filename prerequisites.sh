#!/bin/bash

VERSION=0.1.0
PROGNAME=extern_assets
usage()
{
    echo "Usage "$PROGNAME" [option] [comand] <extern assets component>"
    echo
    echo "Options: "
    echo "-h, --help                Show this help"
    echo "-v, --version             Prints the version"
    echo "-a, -all                  Executes the commend for all extern assets components"                 
    echo "install                   Install (if supports) the specified component from extern assets"
    echo "uninstall                 Uninstall (if supported) specified component"
    echo "build                     Build  specified component as a lib in extern assets"
    echo "check                     Chech for all missing dependencies"

}
all_flag=false

if [ $# -lt 1 ]; then
    echo "asdasd"
    usage >&2
    exit 1
fi

if [ "$#" -ne 1 ]; then
    echo "Illegal number of parameters"
    fi
while :; do
    case "$1" in
        -h|--help)
            usage
            exit 0
            ;;
        -v|--version)
            echo $VERSION
            exit 0
            ;;
        -a|-all)
            shift
            all_flag=true
            ;;
        --)
            shift
            break
            ;;
    esac
done

