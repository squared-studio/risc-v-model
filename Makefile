####################################################################################################
# GENERIC
####################################################################################################

run: build_${TEST}_test
	@build/${TEST}_test.exe

.PHONY: build_${TEST}_test
build_${TEST}_test: build
	@gcc -I./src test/${TEST}_test.c -o build/${TEST}_test.exe

build:
	@mkdir -p build

clean:
	@rm -rf *.exe build dpi

sub/RISC-V-TESTS/Makefile:
	@git submodule update --init --recursive

risc_v_tests: build sub/RISC-V-TESTS/Makefile
	@cd ./sub/RISC-V-TESTS/; make all
	@rm -rf build/risc_v_tests
	@cp -r ./sub/RISC-V-TESTS/build build/risc_v_tests

####################################################################################################
# SPECIFIC
####################################################################################################

build/risc_v_tests/addi.s/addi.s.hex:
	@make -s risc_v_tests

.PHONY: build_byte_read_test
build_byte_read_test: build build/risc_v_tests/addi.s/addi.s.hex
	@gcc -I./src test/byte_read_test.c -o build/byte_read_test.exe

####################################################################################################
# CI
####################################################################################################

CI:
	@make -s run TEST=byte_read
