#!/bin/bash

# default flags values
capitalize=false
color=auto
greeting="Hello"
world=false


function print_help {
    echo "Usage: $0 [OPTIONS]"
    echo "Options:"
    echo "  -c, --capitalize             Capitalize the name or word 'world'"
    echo "  --color=[never|auto|always]  Colorize names (never, auto, always)"
    echo "  -g text, --greeting=text     Replace the word 'Hello' with specified text"
    echo "  -h, --help                   Display this help message"
    echo "  -v, --version                Display program name, version"
    echo "  -w, --world                  Print 'Hello, world!'"
}


function print_version {
    echo "hwb version 0.1"
}


ARGS=$(getopt -o c,g:,h,v,w --long capitalize,color:,greeting:,help,version,world -n "$0" -- "$@")
eval set -- "$ARGS"
while true; do
    case "$1" in
        -c|--capitalize)
            capitalize=true
            shift ;;
        --color)
            color="$2"
            shift 2 ;;
        -g|--greeting)
            greeting="$2"
            shift 2 ;;
        -h|--help)
            print_help
            exit 0 ;;
        -v|--version)
            print_version
            exit 0 ;;
        -w|--world)
            world=true
            shift ;;
        --)
            shift
            break ;;
        
    esac
done

function print_greeting {
    local name="$1"
    if $capitalize; then
        name=$(echo "${name^}")
    fi
    if [ "$color" == "always" ] || [ "$color" == "auto" ] && [ -t 1 ]; then
        echo -e "\033[1;32m$greeting, $name!\033[0m"
    else
        echo "$greeting, $name!"
    fi
}


for arg do
    print_greeting "$arg"
done

if $world; then
    print_greeting "world"
fi

