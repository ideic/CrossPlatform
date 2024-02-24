# syntax=docker/dockerfile:1
FROM debian:bookworm

RUN apt update
RUN apt -y install cmake
RUN apt -y install ninja-build
RUN apt -y install build-essential
RUN apt -y install clang
RUN apt -y install openssh-server
RUN apt -y install rsync
RUN apt -y install gdbserver
RUN apt -y install gdb

#setup ssh
# Create an SSH user
RUN useradd -rm -d /home/sshuser -s /bin/bash -g root -G sudo -u 1000 sshuser
RUN su root
RUN usermod -aG sudo sshuser

# Set the SSH user's password (replace "password" with your desired password)
RUN echo 'sshuser:password' | chpasswd
RUN echo 'root:Carin123!' | chpasswd
# Allow SSH access
RUN mkdir /var/run/sshd
# Expose the SSH port
EXPOSE 22

# Start SSH server on container startup
CMD ["/usr/sbin/sshd", "-D"]