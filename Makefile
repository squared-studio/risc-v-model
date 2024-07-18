CLEAN_TARGETS += *.a
CLEAN_TARGETS += *.exe
CLEAN_TARGETS += *.jou
CLEAN_TARGETS += *.log
CLEAN_TARGETS += *.pb
CLEAN_TARGETS += build
CLEAN_TARGETS += xsim.dir

####################################################################################################
# GENERIC
####################################################################################################

.PHONY: run
run: build_${TEST}_test
	@build/${TEST}_test.exe

.PHONY: build_${TEST}_test
build_${TEST}_test: build
	@gcc -I./src test/${TEST}_test.c -o build/${TEST}_test.exe

build:
	@mkdir -p build

.PHONY: clean
clean:
	@rm -rf ${CLEAN_TARGETS}

sub/RISC-V-TESTS/Makefile:
	@git submodule update --init --recursive

risc_v_tests: build sub/RISC-V-TESTS/Makefile
	@cd ./sub/RISC-V-TESTS/; make all INST_BASE=0000000000000000 DATA_BASE=0000000000001000
	@rm -rf build/risc_v_tests
	@cp -r ./sub/RISC-V-TESTS/build build/risc_v_tests

.PHONT: dpi
dpi:
	@xsc ./src/risc_v_model.c -o risc_v_model.a

####################################################################################################
# SPECIFIC
####################################################################################################

build/risc_v_tests/addi.s/addi.s.hex:
	@make -s risc_v_tests

.PHONY: build_byte_read_test
build_byte_read_test: build build/risc_v_tests/addi.s/addi.s.hex
	@gcc -I./src test/byte_read_test.c -o build/byte_read_test.exe
	
build/risc_v_tests/console_print.s/console_print.s.hex:
	@make -s risc_v_tests

.PHONY: build_decoder_test
build_decoder_test: build build/risc_v_tests/console_print.s/console_print.s.hex
	@gcc -I./src test/decoder_test.c -o build/decoder_test.exe

####################################################################################################
# CI
####################################################################################################

CI:
	@make -s run TEST=byte_read
	@make -s run TEST=decoder
