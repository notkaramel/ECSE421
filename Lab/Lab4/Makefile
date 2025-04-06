ACLI=arduino-cli
BOARD=arduino:avr:uno
PORT=/dev/ttyACM0

build:
	@echo -n "Exercise: "; read exc; \
	echo -n "Task: "; read task; \
	echo -ne "\n-- Compiling! --\n\n"; \
	$(ACLI) compile -b $(BOARD) E$$exc\_T$$task; \
	echo -ne "\n-- Uploading! --\n\n"; \
	$(ACLI) upload -b $(BOARD) --port $(PORT) E$$exc\_T$$task

compile:
	@echo -n "Exercise: "; read exc; \
	echo -n "Task: "; read task; \
	$(ACLI) compile -b $(BOARD) E$$exc\_T$$task

upload:
	@echo -n "Exercise: "; read exc; \
	echo -n "Task: "; read task; \
	$(ACLI) upload -b $(BOARD) --port $(PORT) E$$exc\_T$$task

baudrate:
	@echo -n "Set baudrate for $(PORT): "; read baudrate; \
	stty -F $(PORT) $(baudrate)

sketch:
	@echo -n "Exercise: "; read exc; \
	echo -n "Task: "; read task; \
	$(ACLI) sketch new E$$exc\_T$$task