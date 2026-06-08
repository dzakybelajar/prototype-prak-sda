#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

// Alokasi memori aktual untuk pointer global
PasienNode *rootAVL = NULL;
QueueNode *headQueue = NULL;
CircularLog logSistem = { .head = 0, .count = 0 };
int totalPasien = 0;

int max(int a, int b) { return (a > b) ? a : b; }

int getHeight(PasienNode* n) {
    if (n == NULL) return 0;
    return n->height;
}

int getBalance(PasienNode* n) {
    if (n == NULL) return 0;
    return getHeight(n->left) - getHeight(n->right);
}

PasienNode* rightRotate(PasienNode* y) {
    PasienNode* x = y->left;
    PasienNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    return x;
}

PasienNode* leftRotate(PasienNode* x) {
    PasienNode* y = x->right;
    PasienNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    return y;
}

PasienNode* createPasien(long long nik, char nama[], char alamat[], int urgensi, time_t waktu, char diagnosa[]) {
    PasienNode *newNode = (PasienNode*)malloc(sizeof(PasienNode));
    newNode->nik = nik;
    strcpy(newNode->nama, nama);
    strcpy(newNode->alamat, alamat);
    newNode->urgensi = urgensi;
    
    if(diagnosa == NULL) strcpy(newNode->diagnosa, "Belum Diperiksa");
    else strcpy(newNode->diagnosa, diagnosa);
    
    newNode->left = newNode->right = NULL;
    newNode->waktu_periksa = (waktu == 0) ? time(NULL) : waktu;
    newNode->height = 1;

    totalPasien++;
    return newNode;
}

PasienNode* insertAVL(PasienNode* node, PasienNode* newNode) {
    if (node == NULL) return newNode;

    if (newNode->nik < node->nik) 
        node->left = insertAVL(node->left, newNode);
    else if (newNode->nik > node->nik) 
        node->right = insertAVL(node->right, newNode);
    else 
        return node; 

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int balance = getBalance(node);

    if (balance > 1 && newNode->nik < node->left->nik) return rightRotate(node);
    
    if (balance < -1 && newNode->nik > node->right->nik) return leftRotate(node);
    
    if (balance > 1 && newNode->nik > node->left->nik) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    
    if (balance < -1 && newNode->nik < node->right->nik) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

PasienNode* searchAVL(PasienNode* root, long long nik) {
    if (root == NULL || root->nik == nik) return root;
    if (nik < root->nik) return searchAVL(root->left, nik);
    return searchAVL(root->right, nik);
}

void cariDanTampilkanPasien(long long nik) {
    PasienNode* p = searchAVL(rootAVL, nik);
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
            } 
            else if (j->data->urgensi == minNode->data->urgensi) {
                if (j->data->waktu_periksa < minNode->data->waktu_periksa) {
                    minNode = j;
                }
            }
        }
        if (minNode != i) {
            PasienNode* minData = minNode->data; 
            QueueNode* curr = i;
            PasienNode* prevData = curr->data;
            
            while (curr != minNode) {
                QueueNode* nextNode = curr->next;
                PasienNode* temp = nextNode->data; 
                nextNode->data = prevData;         
                prevData = temp;                  
                curr = nextNode;                  
            }
            i->data = minData;
        }
    }
}

void updateStatusPasien() {
    long long n;
    int statusBaru;
    printf("Masukkan NIK pasien untuk update status: ");
    scanf("%lld", &n);
    PasienNode* p = searchAVL(rootAVL, n);
    if (p) {
        printf("Ditemukan: %s | Status Sekarang: %d\n", p->nama, p->urgensi);
        do{
            statusBaru = 0;
            printf("Pilih Status Baru (1: Darurat, 2: Mendesak, 3: Biasa): ");
            scanf("%d", &statusBaru);
            while(getchar()!='\n');
            if (statusBaru <= 0 || statusBaru > 3)
            { printf("Input tidak valid!\n"); }
        }while(statusBaru <= 0 || statusBaru > 3);
        p->urgensi = statusBaru;
        stablePrioritySort();
        printf("Status berhasil diperbarui dan posisi antrean disesuaikan!\n");
    } else printf("Pasien tidak ditemukan!\n");
}

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
        printf("\n[!] Antrean kosong! Tidak ada pasien untuk diperiksa.\n");
        return;
    }
    PasienNode* p = headQueue->data;
    StackNode* undoStack = NULL;
    char input[500];
    
    printf("\n--- PEMERIKSAAN PASIEN ---\n");
    printf("Nama: %s\n", p->nama);
    printf("NIK : %lld\n", p->nik);
    
    while (1) {
        printf("Input Diagnosa (ketik 'undo' untuk batalkan, 'fix' jika selesai): ");
        scanf(" %[^\n]s", input);
        
        if (strcmp(input, "fix") == 0) {
            break;
        } else if (strcmp(input, "undo") == 0) {
            if (undoStack) {
                StackNode* temp = undoStack;
                undoStack = undoStack->next;
                free(temp);
                
                // Kembalikan ke teks diagnosa sebelumnya di stack jika ada
                if (undoStack) strcpy(p->diagnosa, undoStack->teks_diagnosa);
                else strcpy(p->diagnosa, "Belum Diperiksa");
                
                printf("[Undo Berhasil! Diagnosa saat ini: %s]\n", p->diagnosa);
            } else {
                printf("[!] Tidak ada riwayat input untuk di-undo.\n");
            }
        } else {
            pushUndo(&undoStack, input);
            strcpy(p->diagnosa, input); 
        }
    } 
    
    addLog(p);
    simpanDatabase(); // Auto-save ke file setiap kali selesai periksa
    
    // Dequeue pasien yang sudah diperiksa
    QueueNode* tempQ = headQueue;
    headQueue = headQueue->next;
    free(tempQ);
    
    // Free sisa undo stack
    while(undoStack != NULL) {
        StackNode* t = undoStack;
        undoStack = undoStack->next;
        free(t);
    }
    printf("Pemeriksaan selesai. Data berhasil disimpan!\n");
}

void kumpulData(PasienNode* root, PasienNode** arr, int* index) {
    if (root == NULL) return;
    kumpulData(root->left, arr, index);
    arr[(*index)++] = root;
    kumpulData(root->right, arr, index);
}

void merge(PasienNode** arr, int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    PasienNode **L = malloc(n1 * sizeof(PasienNode*));
    PasienNode **R = malloc(n2 * sizeof(PasienNode*));
    
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
        mergeSort(arr, l, m); 
        mergeSort(arr, m + 1, r); 
        merge(arr, l, m, r);
    }
}

void tampilkanLaporan() {
    if (totalPasien == 0) { printf("\n[!] Belum ada data rekam medis.\n"); return; }
    PasienNode** arr = malloc(totalPasien * sizeof(PasienNode*));
    int index = 0; 
    kumpulData(rootAVL, arr, &index);
    mergeSort(arr, 0, totalPasien - 1);
    
    int hal = 1, totalHal = (totalPasien + 9) / 10;
    char nav;
    do {
        printf("\n--- LAPORAN REKAM MEDIS (Hal %d/%d) ---\n", hal, totalHal);
        printf("%-15s | %-15s | %-12s | %-15s\n", "NIK", "NAMA", "TANGGAL", "DIAGNOSA");
        printf("------------------------------------------------------------------\n");
        for (int i = (hal - 1) * 10; i < hal * 10 && i < totalPasien; i++) {
            struct tm *t = localtime(&arr[i]->waktu_periksa);
            printf("%-15lld | %-15s | %02d/%02d/%d    | %-15s\n", 
                   arr[i]->nik, arr[i]->nama, t->tm_mday, t->tm_mon+1, t->tm_year+1900, arr[i]->diagnosa);
        }
        printf("Navigasi: [n] Next, [p] Prev, [q] Kembali: "); 
        scanf(" %c", &nav);
        if (nav == 'n' && hal < totalHal) hal++; 
        else if (nav == 'p' && hal > 1) hal--;
        else if (nav != 'n' && nav != 'p' && nav != 'q')
        { printf("input tidak valid!\n"); }
        while(getchar()!='\n');
    } while (nav != 'q');
    free(arr);
}

void simpanKeFile(PasienNode* root, FILE* file) {
    if (root == NULL) return;
    // Tulis data terformat ke database.txt
    fprintf(file, "%lld;%s;%s;%d;%ld;%s\n", 
            root->nik, root->nama, root->alamat, root->urgensi, (long)root->waktu_periksa, root->diagnosa);
    simpanKeFile(root->left, file);
    simpanKeFile(root->right, file);
}

void simpanDatabase() {
    FILE* file = fopen("database.txt", "w");
    if (file == NULL) {
        printf("[!] Gagal menyimpan database.\n");
        return;
    }
    simpanKeFile(rootAVL, file);
    fclose(file);
}

void loadDatabase() {
    FILE* file = fopen("database.txt", "r");
    if (file == NULL) return; 
    
    long long nik;
    char nama[50], alamat[100], diagnosa[500];
    int urgensi;
    long waktu;
    
    while (fscanf(file, "%lld;%[^;];%[^;];%d;%ld;%[^\n]\n", &nik, nama, alamat, &urgensi, &waktu, diagnosa) != EOF) {
        PasienNode* baru = createPasien(nik, nama, alamat, urgensi, (time_t)waktu, diagnosa);
        rootAVL = insertAVL(rootAVL, baru);
        
        if(strcmp(diagnosa, "Belum Diperiksa") == 0) {
            enqueue(baru);
        }
    }
    stablePrioritySort();
    fclose(file);
}