#include <stdio.h>
#include <string.h>
#include "header.h"

int login(int role) {
    char username[20];
    char password[20];
    
    printf("\n--- LOGIN SYSTEM ---\n");
    printf("Username: "); scanf("%s", username);
    printf("Password: "); scanf("%s", password);
    
    if (role == 1) { // Resepsionis
        if (strcmp(username, "admin") == 0 && strcmp(password, "123") == 0) return 1;
    } else if (role == 2) { // Dokter
        if (strcmp(username, "dokter") == 0 && strcmp(password, "dok123") == 0) return 1;
    }
    printf("[!] Username atau Password Salah!\n");
    return 0;
}

void menuResepsionis() {
    int pilihan, urg;
    long long n;
    char nm[50], alm[100];
    
    while (1) {
        pilihan = 0;
        printf("\n=== PORTAL RESEPSIONIS ===\n");
        printf("1. Registrasi Pasien Baru (Auto-Queue)\n");
        printf("2. Cari Detail Pasien (via NIK)\n");
        printf("3. Update Status Urgensi Antrean\n");
        printf("4. Logout\n");
        printf("Pilih: "); 
        scanf("%d", &pilihan);
        while(getchar()!='\n');

        switch (pilihan) {
            case 1:
                do {
                    n = 0;
                    printf("NIK: "); 
                    scanf("%lld", &n);
                    while(getchar()!='\n');
                    if (n == 0)
                    { printf("NIK tidak valid!\n"); }
                }while(n == 0);
                // Validasi Cek NIK ganda di AVL
                if(searchAVL(rootAVL, n) != NULL) {
                    printf("[!] NIK sudah terdaftar di sistem!\n");
                    break;
                }
                printf("Nama: "); scanf(" %[^\n]s", nm);
                printf("Alamat: "); scanf(" %[^\n]s", alm);
                do {
                    urg = 0;
                    printf("Urgensi (1:Darurat, 2:Mendesak, 3:Biasa): "); 
                    scanf("%d", &urg);
                    while(getchar()!='\n');
                    if (urg == 0)
                    { printf("NIK tidak valid!\n"); }
                }while(urg <= 0 || urg > 3);
                
                PasienNode* baru = createPasien(n, nm, alm, urg, 0, NULL);
                rootAVL = insertAVL(rootAVL, baru);
                enqueue(baru); 
                stablePrioritySort();
                simpanDatabase(); // Save database ke file
                printf("Berhasil terdaftar dan masuk antrean!\n");
                break;
            case 2:
                do {
                    n = 0;
                    printf("NIK: "); 
                    scanf("%lld", &n);
                    while(getchar()!='\n');
                    if (n == 0)
                    { printf("NIK tidak valid!\n"); }
                }while(n == 0);
                cariDanTampilkanPasien(n);
                break;
            case 3:
                updateStatusPasien();
                simpanDatabase();
                break;
            case 4:
                return;
            default:
                printf("Pilihan tidak valid!\n");
        }
    }
}

void menuDokter() {
    int pilihan;
    while (1) {
        pilihan = 0;
        printf("\n=== PORTAL DOKTER ===\n");
        printf("1. Periksa Pasien Terdepan (Undo Stack)\n");
        printf("2. Riwayat Aktivitas Log (Circular Log)\n");
        printf("3. Cetak Seluruh Laporan Rekam Medis (Merge Sort)\n");
        printf("4. Logout\n");
        printf("Pilih: "); 
        scanf("%d", &pilihan);
        while(getchar()!='\n');

        switch (pilihan) {
            case 1:
                prosesPeriksa();
                break;
            case 2:
                printf("\n--- LOG 10 RIWAYAT TERAKHIR ---\n");
                int jml = (logSistem.count > 10) ? 10 : logSistem.count;
                int start = (logSistem.count > 10) ? logSistem.head : 0;
                if(jml == 0) printf("[Log Kosong]\n");
                for(int i=0; i<jml; i++) {
                    printf("- %s\n", logSistem.riwayat[(start+i)%10]);
                }
                break;
            case 3:
                tampilkanLaporan();
                break;
            case 4:
                return;
            default:
                printf("Pilihan tidak valid!\n");
        }
    }
}

void menuUtama() {
    int pilihan;
    while (1) {
        pilihan = 0;
        printf("\n=== MEDTRACK PRO - FINAL SYSTEM ===\n");
        printf("1. Masuk Sebagai Resepsionis\n");
        printf("2. Masuk Sebagai Dokter\n");
        printf("3. Keluar Aplikasi\n");
        printf("Pilih Akses: "); 
        scanf("%d", &pilihan);
        while(getchar()!='\n');

        switch (pilihan) {
            case 1:
                if (login(1)) menuResepsionis();
                break;
            case 2:
                if (login(2)) menuDokter();
                break;
            case 3:
                simpanDatabase(); // Pastikan data terselamatkan sebelum keluar
                printf("Terima kasih telah menggunakan MedTrack Pro.\n");
                return;
            default:
                printf("Pilihan tidak valid!\n");
        }
    }
}

int main() {
    // Membaca database eksternal saat aplikasi dijalankan
    loadDatabase(); 
    menuUtama();
    return 0;
} 