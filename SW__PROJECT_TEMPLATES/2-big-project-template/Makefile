PLATFORMIO_BIN := ~/.platformio/penv/bin/pio

.PHONY: default
default: deploy ;


.PHONY : serial
serial:
	@echo "$$(tput setab 2) -> Opening Serial Monitor $$(tput sgr0)\n"
	$(PLATFORMIO_BIN) device monitor


.PHONY : deploy
deploy:
	@echo "$$(tput setab 2) -> Building and uploading firmware $$(tput sgr0)\n"
	$(PLATFORMIO_BIN) run --target upload
	@echo "\n\n"
	@$(MAKE) serial
