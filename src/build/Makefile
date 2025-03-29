C_FILES=$(shell find ../kernel/*.c ../drivers/*.c ../*.c)
temp = $(notdir $(C_FILES))
O_FILES = ${temp:.c=.o}

CFLAGS = -g

run: os-image.bin
	qemu-system-i386 -fda os-image.bin
	make clean

os-image.bin: bootsect.bin kernel.bin
	cat bootsect.bin kernel.bin > os-image.bin

bootsect.bin:
	cd ../boot/ && nasm bootsect.asm -f bin -o ../build/bootsect.bin && cd -

kernel.bin: kernel_entry.o kernel.o
	i386-elf-ld -o kernel.bin -Ttext 0x1000 kernel_entry.o $(O_FILES) --oformat binary

kernel_entry.o:
	nasm ../boot/kernel_entry.asm -f elf -o kernel_entry.o

kernel.o:
	i386-elf-gcc ${CFLAGS} -ffreestanding -c $(C_FILES)

clean:
	rm *.bin *.o *.elf

