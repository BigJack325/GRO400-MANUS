FROM ubuntu

RUN apt-get update && apt-get install -y \
    qtbase5-dev \
    libqt5charts5 \
    libqt5charts5-dev \
    libqt5serialport5 \
    libqt5serialport5-dev \
    python3-pyqt5.qtserialport \
    libhdf5-dev \
    libhdf5-serial-dev \
    libatlas-base-dev

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
