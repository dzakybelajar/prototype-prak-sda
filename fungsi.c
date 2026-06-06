#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// =================================================   
// 1. DEFINISI STRUKTUR DATA
// =================================================

typedef struct PasienNode {
    long long nik;
    char nama[50];
    char alamat[100];
    int urgensi;           // 1: Darurat, 2: Mendesak, 3: Biasa
    time_t waktu_periksa;
    char diagnosa[500];
    struct PasienNode *left, *right;
} PasienNode;

typedef struct QueueNode {
    PasienNode *data;
    struct QueueNode *next;
} QueueNode;

typedef struct StackNode {
    char teks_diagnosa[500];
    struct StackNode *next;
} StackNode;

typedef struct {
    char riwayat[10][200];
    int head;
    int count;
} CircularLog;

// Pointer Global
PasienNode *rootBST = NULL;
QueueNode *headQueue = NULL;
CircularLog logSistem = { .head = 0, .count = 0 };
int totalPasien = 0;

// =================================================
// 2. FUNGSI DATABASE & PENCARIAN DETAIL
// =================================================

PasienNode* createPasien(long long nik, char nama[], char alamat[], int urgensi, int manual) {
    PasienNode *newNode = (PasienNode*)malloc(sizeof(PasienNode));
    newNode->nik = nik;
    strcpy(newNode->nama, nama);
    strcpy(newNode->alamat, alamat);
    newNode->urgensi = urgensi;
    strcpy(newNode->diagnosa, "Belum Diperiksa");
    newNode->left = newNode->right = NULL;
    newNode->waktu_periksa = time(NULL);

    totalPasien++;
    return newNode;
}

PasienNode* insertBST(PasienNode* root, PasienNode* newNode) {
    if (root == NULL) return newNode;
    if (newNode->nik < root->nik) root->left = insertBST(root->left, newNode);
    else if (newNode->nik > root->nik) root->right = insertBST(root->right, newNode);
    return root;
}

PasienNode* searchBST(PasienNode* root, long long nik) {
    if (root == NULL || root->nik == nik) return root;
    if (nik < root->nik) return searchBST(root->left, nik);
    return searchBST(root->right, nik);
}

void cariDanTampilkanPasien(long long nik) {
    PasienNode* p = searchBST(rootBST, nik);
    if (p == NULL) {
        printf("\n[!] Pasien dengan NIK %lld tidak ditemukan.\n", nik);
    } else {
        struct tm *t = localtime(&p->waktu_periksa);
        printf("\n=== DATA DETAIL PASIEN ===\n");
        printf("NIK      : %lld\n", p->nik);
        printf("Nama     : %s\n", p->nama);
        printf("Alamat   : %s\n", p->alamat);
        printf("Urgensi  : %s\n", (p->urgensi == 1) ? "Darurat" : (p->urgensi == 2) ? "Mendesak" : "Biasa");
        printf("Tanggal  : %02d/%02d/%d\n", t->tm_mday, t->tm_mon+1, t->tm_year+1900);
        printf("Diagnosa : %s\n", p->diagnosa);
        printf("==========================\n");
    }
}

// =================================================
// 3. LOGIKA ANTREAN PRIORITAS STABIL (STABLE SORT)
// =================================================

void enqueue(PasienNode* pasien) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->data = pasien;
    newNode->next = NULL;

    if (headQueue == NULL) headQueue = newNode;
    else {
        QueueNode* temp = headQueue;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newNode;
    }
}

void stablePrioritySort() {
    if (!headQueue || !headQueue->next) return;
    for (QueueNode* i = headQueue; i != NULL; i = i->next) {
        QueueNode* minNode = i;
        for (QueueNode* j = i->next; j != NULL; j = j->next) {
            if (j->data->urgensi < minNode->data->urgensi) {
                minNode = j;
            } else if (j->data->urgensi == minNode->data->urgensi) {
                if (j->data->waktu_periksa < minNode->data->waktu_periksa) {
                    minNode = j;
                }
            }
        }
        PasienNode* temp = i->data;
        i->data = minNode->data;
        minNode->data = temp;
    }
}

void updateStatusPasien() {
    long long n;
    int statusBaru;
    printf("Masukkan NIK pasien untuk update status: ");
    scanf("%lld", &n);
    PasienNode* p = searchBST(rootBST, n);
    if (p) {
        printf("Ditemukan: %s | Status Sekarang: %d\n", p->nama, p->urgensi);
        printf("Pilih Status Baru (1: Darurat, 2: Mendesak, 3: Biasa): ");
        scanf("%d", &statusBaru);
        p->urgensi = statusBaru;
        stablePrioritySort();
        printf("Status berhasil diperbarui dan posisi antrean disesuaikan!\n");
    } else printf("Pasien tidak ditemukan!\n");
}

// =================================================
// 4. RUANG PERIKSA & SISTEM UNDO
// =================================================

void pushUndo(StackNode** top, char teks[]) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    strcpy(newNode->teks_diagnosa, teks);
    newNode->next = *top;
    *top = newNode;
}

void addLog(PasienNode* p) {
    sprintf(logSistem.riwayat[logSistem.head], "NIK: %lld | Nama: %s | Diag: %s", 
            p->nik, p->nama, p->diagnosa);
    logSistem.head = (logSistem.head + 1) % 10;
    logSistem.count++;
}

void prosesPeriksa() {
    if (headQueue == NULL) {
        printf("Antrean kosong!\n");
        return;
    }
    PasienNode* p = headQueue->data;
    StackNode* undoStack = NULL;
    char input[500];
    printf("\n--- PEMERIKSAAN: %s ---\n", p->nama);
    while (1) {
        printf("Input Diagnosa (undo/fix): ");
        scanf(" %[^\n]s", input);
        if (strcmp(input, "fix") == 0) break;
        if (strcmp(input, "undo") == 0) {
            if (undoStack) {
                StackNode* temp = undoStack;
                undoStack = undoStack->next;
                free(temp);
                printf("[Undo Berhasil]\n");
            }
        } else {
            pushUndo(&undoStack, input);
            strcpy(p->diagnosa, input); 
        }
    } 
    addLog(p);
    QueueNode* tempQ = headQueue;
    headQueue = headQueue->next;
    free(tempQ);
}

// =================================================
// 5. PELAPORAN (MERGE SORT & PAGINATION)
// =================================================

void kumpulData(PasienNode* root, PasienNode** arr, int* index) {
    if (root == NULL) return;
    kumpulData(root->left, arr, index);
    arr[(*index)++] = root;
    kumpulData(root->right, arr, index);
}

void merge(PasienNode** arr, int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    PasienNode **L = malloc(n1 * sizeof(PasienNode*)), **R = malloc(n2 * sizeof(PasienNode*));
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i]->waktu_periksa <= R[j]->waktu_periksa) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    free(L); free(R);
}

void mergeSort(PasienNode** arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m); mergeSort(arr, m + 1, r); merge(arr, l, m, r);
    }
}

void tampilkanLaporan() {
    if (totalPasien == 0) { printf("\n[!] Belum ada data.\n"); return; }
    PasienNode** arr = malloc(totalPasien * sizeof(PasienNode*));
    int index = 0; kumpulData(rootBST, arr, &index);
    mergeSort(arr, 0, totalPasien - 1);
    int hal = 1, totalHal = (totalPasien + 9) / 10;
    char nav;
    do {
        printf("\n--- LAPORAN REKAM MEDIS (Hal %d/%d) ---\n", hal, totalHal);
        printf("%-15s | %-15s | %-12s | %-15s\n", "NIK", "NAMA", "TANGGAL", "DIAGNOSA");
        for (int i = (hal - 1) * 10; i < hal * 10 && i < totalPasien; i++) {
            struct tm *t = localtime(&arr[i]->waktu_periksa);
            printf("%-15lld | %-15s | %02d/%02d/%d   | %-15s\n", arr[i]->nik, arr[i]->nama, t->tm_mday, t->tm_mon+1, t->tm_year+1900, arr[i]->diagnosa);
        }
        printf("Navigasi: [n] Next, [p] Prev, [q] Keluar: "); scanf(" %c", &nav);
        if (nav == 'n' && hal < totalHal) hal++; else if (nav == 'p' && hal > 1) hal--;
    } while (nav != 'q');
    free(arr);
}

