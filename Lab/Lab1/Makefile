ACLI=arduino-cli
BOARD=arduino:avr:uno
PORT=/dev/ttyACM0

build:
	@echo -n "Exercise: "; read exc; \
	$(ACLI) compile -b $(BOARD) E$$exc; \
	$(ACLI) upload -b $(BOARD) --port $(PORT) E$$exc

compile:
	@echo -n "Exercise: "; read exc; \
	$(ACLI) compile -b $(BOARD) E$$exc

upload:
	@echo -n "Exercise: "; read exc; \
	$(ACLI) upload -b $(BOARD) --port $(PORT) E$$exc

baudrate:
	@echo -n "Set baudrate for $(PORT): "; read baudrate; \
	stty -F $(PORT) $(baudrate)