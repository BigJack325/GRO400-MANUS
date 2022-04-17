FROM ubuntu

RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get -y install tzdata
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install keyboard-configuration


RUN apt-get install python3 -y
RUN apt-get install python3-pip -y
RUN apt-get install python3-venv -y

RUN mkdir -p /home/Qt
COPY Hexapod-master/Hexapod_Qt /home/Qt/Hexapod-master/Hexapod_Qt
WORKDIR /home/Qt/Hexapod-master/Hexapod_Qt

RUN python3 -m venv Qt_venv

# Install dependencies:
.
RUN . Qt_venv/bin/activate && pip install -r "pip_requirements.txt"

RUN apt-get install -y \
        libqt5charts5 \
        python3-pyqt5.qtserialport
     

RUN pip list

# Run the application:
RUN python3 MANUS_QT.py
