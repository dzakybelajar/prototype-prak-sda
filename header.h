#ifndef HEADER_H
#define HEADER_H

#include <time.h>



typedef struct PasienNode {
    long long nik;
    char nama[50];
    char alamat[100];
    int urgensi;           // 1: Darurat, 2: Mendesak, 3: Biasa
    time_t waktu_periksa;
    char diagnosa[500];
    int height;            // Tambahan untuk fitur AVL Tree
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

// Pointer Global (Menggunakan extern untuk mencegah duplikasi link error)
extern PasienNode *rootAVL;
extern QueueNode *headQueue;
extern CircularLog logSistem;
extern int totalPasien;


void menuResepsionis();
void menuDokter();
int login(int role); // 1: Resepsionis, 2: Dokter

// Fungsi Basis Data (AVL Tree)
int getHeight(PasienNode* n);
int getBalance(PasienNode* n);
PasienNode* rightRotate(PasienNode* y);
PasienNode* leftRotate(PasienNode* x);
PasienNode* createPasien(long long nik, char nama[], char alamat[], int urgensi, time_t waktu, char diagnosa[]);
PasienNode* insertAVL(PasienNode* node, PasienNode* newNode);
PasienNode* searchAVL(PasienNode* root, long long nik);
void cariDanTampilkanPasien(long long nik);

// Fungsi Antrean (Priority Queue)
void enqueue(PasienNode* pasien);
void stablePrioritySort();
void updateStatusPasien();

// Ruang Periksa (Stack & Log)
void pushUndo(StackNode** top, char teks[]);
void addLog(PasienNode* p);
void prosesPeriksa();

// Pelaporan (Merge Sort)
void kumpulData(PasienNode* root, PasienNode** arr, int* index);
void merge(PasienNode** arr, int l, int m, int r);
void mergeSort(PasienNode** arr, int l, int r);
void tampilkanLaporan();

// File Database System
void simpanKeFile(PasienNode* root, FILE* file);
void simpanDatabase();
void loadDatabase();

#endif