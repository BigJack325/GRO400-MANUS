GO    := $(shell which go)
ARCH  := arm
OOS   := linux

.PHONY: clean check all
all: Qt

clean:
	rm -f Qt

check: Qt
	./Qt

Qt: MANUS_QT.py
	GOOS=$(OOS) GOARCH=$(ARCH) $(GO) build $<
