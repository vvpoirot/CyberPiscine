FROM debian

RUN apt-get update \
    && apt-get install -y tcpdump iproute2 iputils-ping \
    && apt-get install -y python3 \
    && apt-get install -y python3-scapy \
    && apt-get install -y iputils-arping

COPY inquisitor.py /inquisitor.py
COPY pipexec.sh /pipexec.sh

CMD [ "bash" ]