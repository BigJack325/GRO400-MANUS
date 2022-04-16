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
# RUN chmod u+x install_venv.sh
# RUN ./install_venv.sh

ENV VIRTUAL_ENV=/opt/Qt_venv
RUN python3 -m venv $VIRTUAL_ENV
ENV PATH=”$VIRTUAL_ENV/bin:$PATH”

RUN pip install --upgrade PyQt5

RUN apt-get install -y \
        qtbase5-dev \
        libqt5charts5 \
        libqt5charts5-dev \
        libqt5serialport5 \
        libqt5serialport5-dev \
        python3-pyqt5.qtserialport
        
RUN pip install -r "pip_requirements.txt"


RUN pip list

# Run the application:
RUN python3 MANUS_QT.py
