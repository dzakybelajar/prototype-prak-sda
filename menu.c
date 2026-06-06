#include<stdio.h>
#include"header.h"

// =================================================
// 6. MAIN MENU
// =================================================
int main() {
    int pilihan, urg, m;
    long long n;
    char nm[50], alm[100];
    while (1) {
        printf("\n=== MEDTRACK PRO - FINAL SYSTEM ===\n");
        printf("1. Registrasi Pasien (Auto-Queue)\n");
        printf("2. Cari Detail Pasien (via NIK)\n");
        printf("3. Update Status Pasien (Manual)\n");
        printf("4. Ruang Periksa Dokter (Undo Stack)\n");
        printf("5. Riwayat Aktivitas (Circular Log)\n");
        printf("6. Cetak Laporan (Merge Sort)\n");
        printf("7. Keluar\n");
        printf("Pilih: "); scanf("%d", &pilihan);
        switch (pilihan) {
            case 1:
                printf("NIK: "); scanf("%lld", &n);
                printf("Nama: "); scanf(" %[^\n]s", nm);
                printf("Alamat: "); scanf(" %[^\n]s", alm);
                printf("Urgensi (1:Darurat, 2:Mendesak, 3:Biasa): "); scanf("%d", &urg);
                PasienNode* baru = createPasien(n, nm, alm, urg, m);
                rootBST = insertBST(rootBST, baru);
                enqueue(baru); stablePrioritySort();
                printf("Berhasil terdaftar dan masuk antrean!\n");
                break;
            case 2:
                printf("Masukkan NIK: "); scanf("%lld", &n);
                cariDanTampilkanPasien(n);
                break;
            case 3:
                updateStatusPasien();
                break;
            case 4:
                prosesPeriksa();
                break;
            case 5:
                printf("\n--- LOG 10 RIWAYAT TERAKHIR ---\n");
                int jml = (logSistem.count > 10) ? 10 : logSistem.count;
                int start = (logSistem.count > 10) ? logSistem.head : 0;
                for(int i=0; i<jml; i++) printf("- %s\n", logSistem.riwayat[(start+i)%10]);
                break;
            case 6:
                tampilkanLaporan();
                break;
            case 7:
                printf("terima kasih");
                return 0;
        }
    }
}