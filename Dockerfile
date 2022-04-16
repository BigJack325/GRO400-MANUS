FROM ubuntu

RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get -y install tzdata
RUN apt-get install -y \
    qtbase5-dev \
    libqt5charts5-dev \
    libqt5charts5 \
    libqt5serialport5-dev \
    libqt5serialport5 \
    python3-pyqt5.qtserialport \
    python3-pyqt5.qtchart
RUN apt-get remove python-PyQt5


RUN apt-get install python3 -y
RUN apt-get install python3-pip -y
RUN apt-get install python3-venv -y

RUN mkdir -p /home/Qt
COPY Hexapod-master/Hexapod_Qt /home/Qt/Hexapod-master/Hexapod_Qt
WORKDIR /home/Qt/Hexapod-master/Hexapod_Qt
RUN chmod u+x install_venv.sh
RUN ./install_venv.sh

ENV VIRTUAL_ENV=/Qt_venv
ENV PATH=”$VIRTUAL_ENV/bin:$PATH”

# Run the application:
RUN python3 MANUS_QT.py
