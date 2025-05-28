# HOW TO USE :

./ft_stockholm to execute the program

## Arguments :

No arg : juste encrypt all file in /home/username/infection/

-help or -h : display the help

-version or -v : show the version

-reverse or -r : to uncrypt all file (send key in arg)

-silent or -s : to mute the log

## docker cmd :

docker build -t stockholm .

docker run -it stockholm bash

docker ps -a

docker rmi -f $(docker images -q)

docker rm -f $(docker ps -aq)
