#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_APPS 100
#define MAX_NAME 50
#define FILE_NAME "apps.txt"

#define COLOR_RESET   "\x1b[0m"
#define COLOR_NAME    "\x1b[36m"
#define COLOR_CATEGORY "\x1b[33m"
#define COLOR_STATUS   "\x1b[32m"
#define COLOR_ERROR    "\x1b[31m"

typedef struct {
    char name[MAX_NAME];
    char category[MAX_NAME];
    int installed;
} Application;

Application catalog[MAX_APPS];
int appCount = 0;
char username[MAX_NAME];

void saveToFile() {
    FILE *file = fopen(FILE_NAME, "w");
    if (!file) {
        printf(COLOR_ERROR "Eroare la salvarea în fișier!\n" COLOR_RESET);
        return;
    }
    for (int i = 0; i < appCount; i++) {
        fprintf(file, "%s,%s,%d\n", catalog[i].name, catalog[i].category, catalog[i].installed);
    }
    fclose(file);
    printf("Fișierul a fost salvat cu succes!\n");
}

void loadFromFile() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        printf("Fișierul nu există! Se creează automat...\n");
        file = fopen(FILE_NAME, "w");
        if (!file) {
            printf(COLOR_ERROR "Eroare la crearea fișierului!\n" COLOR_RESET);
            return;
        }
        Application defaultApps[] = {
            {"Netflix", "Entertainment", 0}, {"Spotify", "Entertainment", 0},
            {"Facebook", "Social", 0}, {"Instagram", "Social", 0},
            {"Trello", "Productivity", 0}, {"Google Docs", "Productivity", 0},
            {"ChatGPT", "AI Tools", 0}, {"MidJourney", "AI Tools", 0},
            {"TikTok", "Social", 0}, {"Snapchat", "Social", 0},
            {"Notion", "Productivity", 0}, {"Duolingo", "Education", 0},
            {"Adobe Photoshop", "Creativity", 0}, {"Canva", "Creativity", 0},
            {"Google Meet", "Communication", 0}, {"Zoom", "Communication", 0}
        };
        int defaultAppCount = sizeof(defaultApps) / sizeof(Application);
        for (int i = 0; i < defaultAppCount; i++) {
            fprintf(file, "%s,%s,%d\n", defaultApps[i].name, defaultApps[i].category, defaultApps[i].installed);
        }
        fclose(file);
        return;
    }
    while (fscanf(file, "%49[^,],%49[^,],%d\n", catalog[appCount].name, catalog[appCount].category, &catalog[appCount].installed) == 3) {
        appCount++;
    }
    fclose(file);
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void registerUser() {
    clearScreen();
    printf("=== Înregistrare Utilizator ===\n");
    printf("Introdu numele utilizatorului: ");
    fgets(username, MAX_NAME, stdin);
    username[strcspn(username, "\n")] = 0;
    printf("Bine ai venit, %s!\n", username);
}

void viewCatalog() {
    clearScreen();
    printf("=== Catalog Aplicații ===\n");
    for (int i = 0; i < appCount; i++) {
        printf("  - %s [\033[34m%s\033[0m] %s\n",
            catalog[i].name,
            catalog[i].category,
            catalog[i].installed ? "\033[32m(Instalat)\033[0m" : "\033[31m(Neinstalat)\033[0m"
        );
    }
    printf("\n\033[0m");
}

void viewStats() {
    clearScreen();
    printf("=== Statistici Aplicații ===\n\n");

    const char *categories[] = {
        "Entertainment", "Social", "Productivity",
        "AI Tools", "Education", "Creativity", "Communication"
    };

    const char *colors[] = {
        "\033[35m",
        "\033[36m",
        "\033[33m",
        "\033[34m",
        "\033[32m",
        "\033[31m",
        "\033[90m"
    };

    int stats[7] = {0};

    for (int i = 0; i < appCount; i++) {
        if (catalog[i].installed) {
            for (int j = 0; j < 7; j++) {
                if (strcmp(catalog[i].category, categories[j]) == 0) {
                    stats[j]++;
                }
            }
        }
    }

    for (int i = 0; i < 7; i++) {
        printf("%s%s\033[0m: %d aplicații instalate\n", colors[i], categories[i], stats[i]);
    }

    printf("\n\033[0m"); 
}


void installApp() {
    clearScreen();
    printf("=== Instalare Aplicație ===\n");
    viewCatalog();
    printf("\nIntrodu numele aplicației de instalat: ");
    char appName[MAX_NAME];
    fgets(appName, MAX_NAME, stdin);
    appName[strcspn(appName, "\n")] = 0;

    int found = 0;
    for (int i = 0; i < appCount; i++) {
        if (strcmp(catalog[i].name, appName) == 0) {
            found = 1;
            if (catalog[i].installed) {
                printf(COLOR_ERROR "Aplicația este deja instalată!\n" COLOR_RESET);
            } else {
                catalog[i].installed = 1;
                saveToFile();
                printf("Aplicația %s a fost instalată!\n", catalog[i].name);
            }
            break;
        }
    }
    if (!found) {
        printf(COLOR_ERROR "Aplicația introdusă nu există în catalog!\n" COLOR_RESET);
    }
    printf("\nApasă Enter pentru a reveni la meniul principal...");
    getchar();
}

void uninstallApp() {
    clearScreen();
    printf("=== Dezinstalare Aplicație ===\n");
    viewCatalog();
    printf("\nIntrodu numele aplicației de dezinstalat: ");
    char appName[MAX_NAME];
    fgets(appName, MAX_NAME, stdin);
    appName[strcspn(appName, "\n")] = 0;

    int found = 0;
    for (int i = 0; i < appCount; i++) {
        if (strcmp(catalog[i].name, appName) == 0) {
            found = 1;
            if (!catalog[i].installed) {
                printf(COLOR_ERROR "Aplicația nu este instalată!\n" COLOR_RESET);
            } else {
                catalog[i].installed = 0;
                saveToFile();
                printf("Aplicația %s a fost dezinstalată!\n", catalog[i].name);
            }
            break;
        }
    }
    if (!found) {
        printf(COLOR_ERROR "Aplicația introdusă nu există în catalog!\n" COLOR_RESET);
    }
    printf("\nApasă Enter pentru a reveni la meniul principal...");
    getchar();
}

int main() {
    loadFromFile();
    registerUser();
    int choice;
    do {
        clearScreen();
        printf("=== Meniu Principal ===\n");
        printf("1. Vizualizează catalogul\n");
        printf("2. Instalează o aplicație\n");
        printf("3. Dezinstalează o aplicație\n");
        printf("4. Vizualizează statistici\n");
        printf("5. Ieșire\n");
        printf("Alege o opțiune: ");
        char input[10];
        fgets(input, sizeof(input), stdin);
        choice = strtol(input, NULL, 10);
        switch (choice) {
            case 1: viewCatalog(); break;
            case 2: installApp(); break;
            case 3: uninstallApp(); break;
            case 4: viewStats(); break;
            case 5: printf("Ieșire din program.\n"); break;
            default: printf(COLOR_ERROR "Opțiune invalidă!\n" COLOR_RESET); sleep(1);
        }
    } while (choice != 5);
    return 0;
}

