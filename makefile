# Makefile for Assembly x86 project

# Default target
all: build

# Clean build artifacts
clean:
	rm -f 86c764x.bin
	rm -f 86c764x.err
	rm -f 86c764x.zip

# Build the project
build: 86c764x.bin

86c764x.bin: research/s3/bios/86c764x.asm
	jwasm -Fw86c764x.err -Fo86c764x.bin -bin research/s3/bios/86c764x.asm;

# Run tests
test:
	md5sum -c 86c764x.md5

# Package the project
package:
    # Add packaging commands here
	zip -9 86c764x.zip 86c764x.bin 86c764x.md5
