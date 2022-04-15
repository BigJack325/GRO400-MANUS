GO    := $(shell which go)
ARCH  := arm
OOS   := linux

.PHONY: clean check all
all: example

clean:
	rm -f qt
check: qt
	./qt

Qt: Hexapod-master/Hexapod-Qt/MANUS_QT.py
	GOOS=$(OOS) GOARCH=$(ARCH) $(GO) build $<
