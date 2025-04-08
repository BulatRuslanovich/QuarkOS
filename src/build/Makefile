# Сборка всех исходных C-файлов из указанных директорий
# Находит все .c файлы в: 
# - директории ядра (../kernel)
# - драйверов (../drivers)
# - корневой директории (../)
C_FILES=$(shell find ../kernel/*.c ../drivers/*.c ../*.c)

# Извлекаем только имена файлов без путей 
# Например: main.c screen.c print.c ...
temp = $(notdir $(C_FILES))

# Преобразуем имена .c файлов в .o объектные файлы
# Пример: main.c -> main.o, screen.c -> screen.o
O_FILES = ${temp:.c=.o}

# Флаги компиляции
CFLAGS = -g  # Включение отладочной информации

# Основная цель по умолчанию - запуск в QEMU
run: os-image.bin
    # Запуск QEMU с флоппи-диском
	qemu-system-i386 -fda os-image.bin
	# Очистка после запуска
	make clean

# Сборка итогового образа ОС
os-image.bin: bootsect.bin kernel.bin
    # Объединение загрузчика и ядра в один образ
	cat bootsect.bin kernel.bin > os-image.bin

# Сборка загрузочного сектора
bootsect.bin:
    # Ассемблирование загрузчика (16-битный код)
	cd ../boot/ && nasm bootsect.asm -f bin -o ../build/bootsect.bin && cd -

# Сборка ядра ОС
kernel.bin: kernel_entry.o kernel.o
    # Линковка объектных файлов с:
    # - архитектурой i386
    # - точкой входа по адресу 0x1000
    # - выходным форматом raw binary
	ld -m elf_i386 -o kernel.bin -Ttext 0x1000 kernel_entry.o $(O_FILES) --oformat binary

# Сборка точки входа в ядро (ассемблерная часть)
kernel_entry.o:
    # Ассемблирование 32-битной точки входа
	nasm ../boot/kernel_entry.asm -f elf -o kernel_entry.o

# Компиляция всех C-файлов
kernel.o:
    # Компиляция с флагами:
    # -m32: 32-битная архитектура
    # -ffreestanding: независимая среда без стандартной библиотеки
	gcc -m32 ${CFLAGS} -ffreestanding -c $(C_FILES)

# Генерирует документацию
docs:
	doxygen ../../docs/Doxyfile
	xdg-open html/index.html

# Очистка артефактов сборки
clean:
    # Удаление всех временных файлов:
	rm -rf *.bin *.o *.elf html/