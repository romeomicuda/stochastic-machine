#define _CRT_SECURE_NO_WARNINGS
#define STATE_MAX_LEN 60 // zadajte tak aby to sedelo so sirkou konzoly
#define RULES_MAX_LEN 10 // toto cislo je na vas

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windows.h>
void mySleep(int sleepMs)
{
    Sleep(sleepMs);
}
#elif defined(__APPLE__) || defined(__linux__)
#include <unistd.h>
void mySleep(int sleepMs)
{
    usleep(sleepMs * 1000);   // usleep takes sleep time in us (1 millionth of a second)
}
#else
#error "Unknown OS"
#endif
void init_rules(int rules[], int *pN)
{
    int r, num1;                            // r zmeni velkost pola, num1 je zadana pravdepodobnost uzivatelom
    while(1){
        printf("Enter a number of rules :");
        scanf("%d", &r);
        if (r < RULES_MAX_LEN && (r % 2 == 0))                 // pocet pravidiel musi byt parne cislo a mesie nez MACRO
            break;
    } 
    for (int i = 0; i < r; i++)
    {
        while (1) {
            printf("Enter a probality for rule %d :", i);           // zadavanie pravdepodobnosti uzivatelom
            scanf("%d", &num1);
            if (num1 >= 0 && num1<=100)                                // v rozsahu od 0 po 100
                break;
        }
        rules[i] = num1;
    }
    *pN = r;
}

int randNumber()            // pre nahodne generovanie 0 alebo 1
{
    
    return rand() % 2;
}

void init_state(int pole[])         //  inicializacia pola nahodne vyplnenymi jednotkami alebo nulami
{
    int n;
    for (int i = 0; i < STATE_MAX_LEN; i++)
    {
        pole[i] = randNumber();
    }
}

void print_state(int state[], int s, int count) {           // funkcia na vypisovanie pola do konzoly

    printf("t=%-*d",3, count);
    for (int i = 0; i < s; i++)
        if (state[i] == 1)
            printf("*");
        else
            printf(" ");
    printf("\n");
}

int sumInWindow(int state[], int s, int r, int i)           // funkcia scitava nuly a jednotky v pohyblivom okne 
{
    int num = (r - 1) / 2;
    int count = 0;
    for (int k = i; k <= (num + i); ++k) {                  // pre jednotky na pravej strane
        if (k >= s)                                         // pomocna podmienka aby sa neprekrocilo rozsah pola z prava
            continue;
        count += state[k];
    }
    for (int j = (i-1); j >= (i - num); --j){               // pre jednotky na lavej strane
        if (j < 0)                                          // pomocna podmienka aby sa neprekrocilo rozsah pola z lava
            continue;
        count += state[j];
    }
    return count;                                           // vrati pocet jednotiek v pohyblivom okne
}
int generateBit(int p) {                                    // funkcia nahodne genruje jednotky alebo nuly podla pravdepodobnosti

    int genbit[100];                                                    
    int num = rand() % 100;                                 // nahodne vygenerovane cislo od 0 po 99
    for (int i = 0; i < 100; i++) {                         // napnenie pola nulami a jednotkami
        if (i < p)                                            
            genbit[i] = 1;
        else
            genbit[i] = 0;
    }
    return genbit[num];                                     // vratenie nahodne cisla s pola so zadanou pravdepodobnostou
    
}

void generateNextState(int state[], int new_state[], int s, int rules[], int r)         // funkcia pre generovanie noveho stavu podla algoritmu
{
    int sum, num;
    for (int i = 0; i < s; i++)
    {
        sum = sumInWindow(state, s, r - 1, i);                      // scitanie jednotiek v pohyblivom okne
        num = generateBit(rules[sum]);                              // podla pravdepodobnosti vybratej s pola rules[] vygeneruje 1 alebo 0
        new_state[i] = num;                                         
    }

}
void copy(int state[], int new_state[], int s)          // funkcia na kopirovanie poli
{
    for (int i = 0; i < s; i++)
        state[i] = new_state[i];

}
void header()                                           // hlavicka programu  
{
    printf("\n\nSTOCHASTIC CELLULAR AUTOMATON\n\n");    // nazov programu
    for (int i = 0; i < STATE_MAX_LEN+6; i++)
        printf("_");
    printf("\n\n");
}

void celluar_automation(int num)                        // num je pocet iteracii
{
    
    int state[STATE_MAX_LEN];                           // pole pre 0 alebo 1
    int s = sizeof(state) / sizeof(state[0]);           // velkost pola state
    int rules[RULES_MAX_LEN];                           // pole s definovanymi pravdepodopnostami pre pohyblive okno
    int r = sizeof(rules) / sizeof(rules[0]);   
    int new_state[STATE_MAX_LEN];                       // pomocne pole
    int count = 0;                                      // pocitadlo pre pocet iteracii

    srand(time(0));                                     
    init_state(state);                                  // inicializacia pola state 0 alebo 1 nahodne
    init_rules(rules, &r);                              // uzivatel zada pravidla pre vyvoj algoritmu
    while (count<num) {
        print_state(state, s, count);                   // vypisovanie stavov
        ++count;                                        
        generateNextState(state, new_state, s, rules, r);           // generovanie noveho stavu
        copy(state, new_state, s);                                  // kopirovanie pola new_state[] do pola state[]
        mySleep(400);                                               // oneskorenie
    } 
}

int main()
{
    header();
    int n;
    while (1) {                                                        // user zada pocet iteracii
        printf("Enter a number of iterations :");
        scanf("%d", &n);
        if (n > 0)
            break;
    }
    celluar_automation(n);
    return 0;
}