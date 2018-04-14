C_SOURCE=$(shell find . -name "*.c")
C_OBJECT=$(patsubst %.c,%.o,$(C_SOURCE))
S_SOURCE=$(shell find . -name "*.s")
S_OBJECT=$(patsubst %.s,%.o,$(S_SOURCE))
CC=gcc
LD=ld
ASM=nasm
CFLAGS=-c -Wall -m32 -ggdb -gstabs+ -std=gnu11 -nostdinc -fpack-struct -fno-builtin -fno-stack-protector -Iinclude
LDFLAGS=-T script/link.ld -m elf_i386 -nostdlib
ASMFLAGS=-f elf -g -F stabs
all: $(S_OBJECT) $(C_OBJECT) link update
.c.o:
	@echo "Compiling the source..."
	$(CC) $(CFLAGS) $< -o $@
.s.o:
	@echo "Assembling the source..."
	$(ASM) $(ASMFLAGS) $<
link:
	@echo "Linking kernel..."
	$(LD) $(LDFLAGS) $(S_OBJECT) $(C_OBJECT) -o kernel.elf
	$(LD) $(LDFLAGS) $(S_OBJECT) $(C_OBJECT) -o recovery.elf
	$(LD) $(LDFLAGS) $(S_OBJECT) $(C_OBJECT) -o live.elf
.PHONY: clean
clean:
	$(RM) $(S_OBJECT) $(C_OBJECT) *.elf os.img
.PHONY: update
update:
	test -d /mnt/os || sudo mkdir -p /mnt/os
	cp grub.img os.img
	sudo mount os.img /mnt/os
	sudo cp kernel.elf recovery.elf live.elf /mnt/os/kernel/
	sudo umount /mnt/os
.PHONY: mount_image
mount_image:
	sudo mount os.img /mnt/os
.PHONY: umount_image
umount_image:
	sudo umount /mnt/os
.PHONY: delete_mountpoint
delete_mountpoint:
	sudo rm -rf /mnt/os
.PHONY: qemu
qemu:
	qemu-system-i386 -fda os.img -boot a
.PHONY: bochsgdb
bochsgdb:
	bochsgdb -f tool/bochsrc-gdb.txt
.PHONY: bochsdbg
bochsdbg:
	bochsdbg -f tool/bochsrc.txt
.PHONY: debug
debug:
	qemu-system-i386 -s -S -fda os.img -boot a
