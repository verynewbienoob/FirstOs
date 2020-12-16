myos.bin: boot.o kernel.o string.o gdt.o idt.o isrs.o terminal.o timer.o descriptors.o pc.o kboard.o stdio.o shell.o 
	i686-elf-gcc -O0 -T linker.ld -g -o myos.bin -ffreestanding -nostdlib timer.o terminal.o pc.o descriptors.o stdio.o shell.o gdt.o idt.o boot.o string.o isrs.o kboard.o kernel.o -lgcc


kernel.o: kernel.c
	i686-elf-gcc -O0 -c kernel.c -g -o kernel.o -std=gnu99 -ffreestanding -Wall -Wextra

kboard.o: kboard.c
	i686-elf-gcc -O0 -c kboard.c -g -o kboard.o -std=gnu99 -ffreestanding -Wall -Wextra

stdio.o: stdio.c
	i686-elf-gcc -O0 -c stdio.c -g -o stdio.o -std=gnu99 -ffreestanding -Wall -Wextra

shell.o: shell.c
	i686-elf-gcc -O0 -c shell.c -g -o shell.o -std=gnu99 -ffreestanding -Wall -Wextra


timer.o: timer.c
	i686-elf-gcc -O0 -c timer.c -g -o timer.o -std=gnu99 -ffreestanding -Wall -Wextra

pc.o: pc.c
	i686-elf-gcc -O0 -c pc.c -g -o pc.o -std=gnu99 -ffreestanding -Wall -Wextra

isrs.o: isrs.c
	i686-elf-gcc -O0 -c isrs.c -g -o isrs.o -std=gnu99 -ffreestanding -Wall -Wextra

idt.o: idt.c
	i686-elf-gcc -O0 -c idt.c -g -o idt.o -std=gnu99 -ffreestanding -Wall -Wextra

gdt.o: gdt.c
	i686-elf-gcc -O0 -c gdt.c -g -o gdt.o -std=gnu99 -ffreestanding -Wall -Wextra

string.o: string.c
	i686-elf-gcc -O0 -c string.c -g -o string.o -std=gnu99 -ffreestanding -Wall -Wextra

terminal.o: terminal.c
	i686-elf-gcc -O0 -c terminal.c -g -o terminal.o -std=gnu99 -ffreestanding -Wall -Wextra

descriptors.o: descriptors.asm
	nasm -felf32 descriptors.asm -g -o descriptors.o

boot.o: boot.asm
	nasm -felf32 boot.asm -g -o boot.o

iso_build:
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/myos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir

clean:
	rm *.o
	rm myos.bin
	rm myos.iso

cross:
	export PREFIX="$HOME/opt/cross"
	export TARGET=i686-elf
	export PATH="$PREFIX/bin:$PATH"
	export PATH="$HOME/opt/cross/bin:$PATH"

iso:
	qemu-system-i386 -cdrom myos.iso 
bin:
	qemu-system-i386 -monitor stdio -kernel myos.bin

debug:
	qemu-system-i386 -monitor stdio -s -S -kernel myos.bin


launch:
	make
	make iso_build
	make iso