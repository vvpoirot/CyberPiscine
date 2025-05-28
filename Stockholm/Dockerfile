FROM debian

RUN apt-get update \
    && apt-get install -y g++ \
    && apt-get install -y libcrypto++-dev libcrypto++-doc libcrypto++-utils

COPY ./main.cpp /main.cpp
RUN g++ /main.cpp -o /home/Stockholm -lcryptopp

COPY ./script.sh /script.sh
RUN chmod 777 /script.sh \
    && /script.sh

CMD ["/bin/bash"]