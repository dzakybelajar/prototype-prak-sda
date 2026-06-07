# Variabel untuk compiler dan file
CC = gcc
SRC = menu.c fungsi.c
TARGET = program_sementara.exe

# Target utama: compile, jalankan, lalu otomatis hapus file .exe
run:
	@echo "Mengompilasi dan menjalankan program..."
	@$(CC) $(SRC) -o $(TARGET)
	@.\$(TARGET)
	@del /f /q $(TARGET)

# Target untuk hapus manual jika sewaktu-waktu program hang/crash sebelum sempat terhapus
clean:
	@del /f /q $(TARGET)