FROM ubuntu:18.04

RUN apt-get update
RUN apt-get install python3 -y
RUN apt-get install python3-pip -y

RUN mkdir -p /home/Qt
ADD Hexapod_Qt /home/Qt
WORKDIR /home/Qt/Hexapod_Qt
RUN ./install_venv_pi.sh
