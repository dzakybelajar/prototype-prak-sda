# Variabel untuk compiler dan file
CC = gcc
SRC = menu.c fungsi.c
TARGET = program_sementara

# Deteksi Sistem Operasi
ifeq ($(OS),Windows_NT)
	# Pengaturan khusus Windows
	EXEC = .\$(TARGET).exe
	CLEAN = del /f /q $(TARGET).exe
else
	# Pengaturan khusus Linux/macOS
	EXEC = ./$(TARGET)
	CLEAN = rm -f $(TARGET)
endif

# Target utama: compile, jalankan, lalu hapus otomatis
run:
	@echo "Mengompilasi dan menjalankan program..."
	@$(CC) $(SRC) -o $(TARGET)
	@$(EXEC)
	@$(CLEAN)

# Target untuk hapus manual
clean:
	@$(CLEAN)