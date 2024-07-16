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

risc_v_tests: build
	@cd ./sub/RISC-V-TESTS/; make all
	@rm -rf build/risc_v_tests
	@cp -r ./sub/RISC-V-TESTS/build build/risc_v_tests

####################################################################################################
# SPECIFIC
####################################################################################################

build/risc_v_tests/addi.s/addi.s.hex: risc_v_tests

.PHONY: build_byte_read_test
build_byte_read_test: build build/risc_v_tests/addi.s/addi.s.hex
	@gcc -I./src test/byte_read_test.c -o build/byte_read_test.exe
