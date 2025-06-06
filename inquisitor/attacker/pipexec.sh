echo "===> IP server :"
getent hosts server
echo "===> You'r MAC :"
ip link show eth0 | grep ether
echo "===> IP client :"
getent hosts client
echo "===> MAC client :"
arping -I eth0 client