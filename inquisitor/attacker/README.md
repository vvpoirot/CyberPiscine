## How it work

python3 inquisitor -h

> Get other IP
getent hosts "client" or "server"

> Get other MAC
arping -I eth0 |IP-target|

> Get self MAC
ip link show eth0 | grep ether