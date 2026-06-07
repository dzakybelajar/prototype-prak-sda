# Variabel untuk compiler dan file
CC = gcc
SRC = menu.c fungsi.c
TARGET = program_sementara

# Target utama: compile, jalankan, lalu otomatis hapus file .exe
run:
	@echo "Komputasi dan menjalankan program..."
	@$(CC) $(SRC) -o $(TARGET)
	@./$(TARGET)
	@rm -f $(TARGET).exe

# Target opsional jika ingin menghapus manual (jika program error di tengah jalan)
clean:
	@rm -f $(TARGET).exe