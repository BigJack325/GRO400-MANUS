FROM resin/rpi-raspbian:latest

RUN apt-get update && apt-get install -y make golang
RUN mkdir -p /home/Qt
ADD Hexapod_Qt /home/Qt
WORKDIR /home/Qt/Hexapod_Qt
