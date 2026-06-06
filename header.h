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

void menu();
PasienNode* createPasien(long long nik, char nama[], char alamat[], int urgensi, int manual);
PasienNode* insertBST(PasienNode* root, PasienNode* newNode);
PasienNode* searchBST(PasienNode* root, long long nik);
void cariDanTampilkanPasien(long long nik);
void enqueue(PasienNode* pasien);
void stablePrioritySort();
void updateStatusPasien();
void pushUndo(StackNode** top, char teks[]);
void addLog(PasienNode* p);
void prosesPeriksa();
void kumpulData(PasienNode* root, PasienNode** arr, int* index);
void merge(PasienNode** arr, int l, int m, int r);
void mergeSort(PasienNode** arr, int l, int r);
void tampilkanLaporan();