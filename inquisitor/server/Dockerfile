FROM debian


RUN apt-get update && \
    apt-get install -y vsftpd openssl && \
    mkdir -p /var/run/vsftpd/empty

COPY create_user.sh /create_user.sh
RUN chmod +x /create_user.sh
COPY vsftpd.conf /etc/vsftpd.conf

EXPOSE 21

COPY script.sh /script.sh
RUN chmod +x /script.sh \
    && /create_user.sh
ENTRYPOINT ["/script.sh"]


CMD ["bash"]