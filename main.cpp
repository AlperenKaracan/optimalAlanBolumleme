//210201096
// Alperen KARACAN
#include <stdio.h>
#include <graphics.h>
#include <stdbool.h>
#define MAX_SEKIL 10
#define MAX_NOKTA 100

typedef struct {
    int x[MAX_NOKTA];
    int y[MAX_NOKTA];
    int nokta_sayisi;
} Sekil;

Sekil sekiller[MAX_SEKIL];
int sekil_sayisi = 0;

bool icinde_mi(int x[], int y[], int n, double noktaX, double noktaY) {
    int i, j;
    bool son = false;
    for (i = 0, j = n-1; i < n; j = i++) {
        if ((y[i] > noktaY) != (y[j] > noktaY) &&
            (noktaX < (x[j] - x[i]) * (noktaY - y[i]) / (y[j] - y[i]) + x[i])) {
            son = !son;
        }
    }
    return son;
}

void cizVeHesapla(Sekil sekil, int olcekFaktoru, double birimSondajMaliyeti, double birimPlatformMaliyeti) {
    
    int platformSayisi = 0;
    double toplamPlatformMaliyeti = 0.0;

    int olceklendirilmisNoktalar[2 * MAX_NOKTA];
    for (int i = 0; i < sekil.nokta_sayisi; ++i) {
        olceklendirilmisNoktalar[2 * i] = sekil.x[i] * olcekFaktoru;
        olceklendirilmisNoktalar[2 * i + 1] = sekil.y[i] * olcekFaktoru;
    }

     int sayac = 0;

    drawpoly(sekil.nokta_sayisi, olceklendirilmisNoktalar);

    for (int i = 0; i < getmaxx(); i += olcekFaktoru) {
        for (int j = 0; j < getmaxy(); j += olcekFaktoru) {
            bool merkezIcindemi = icinde_mi(sekil.x, sekil.y, sekil.nokta_sayisi, (i + olcekFaktoru / 2.0) / olcekFaktoru, (j + olcekFaktoru / 2.0) / olcekFaktoru);
            if (merkezIcindemi) {
                sayac++;
                rectangle(i, j, i + olcekFaktoru, j + olcekFaktoru);
                platformSayisi++;
            }
        }
    }
    double toplamSondajMaliyeti = sayac * birimSondajMaliyeti;
    char sayac_str[50];
    sprintf(sayac_str, "%d", sayac);
    int agirlikMerkeziX = 0, agirlikMerkeziY = 0;
    for (int i = 0; i < sekil.nokta_sayisi; i++) {
        agirlikMerkeziX += sekil.x[i];
        agirlikMerkeziY += sekil.y[i];
    }
    agirlikMerkeziX /= sekil.nokta_sayisi;
    agirlikMerkeziY /= sekil.nokta_sayisi;
    outtextxy(agirlikMerkeziX * olcekFaktoru - 10, agirlikMerkeziY * olcekFaktoru, sayac_str);
    // sayac, birim kare sayýsýný tutuyor. Bu da yüzey alanýný verir.
    double rezervDegeri = sayac * 10;  // Alaný 10'la çarptým
   
    toplamPlatformMaliyeti = platformSayisi * birimPlatformMaliyeti;
    double toplamMaliyet = toplamSondajMaliyeti + toplamPlatformMaliyeti;

    
    double karMiktari = rezervDegeri - toplamMaliyet;
    char rezervDegeri_str[20];
    sprintf(rezervDegeri_str, "Rezerv Degeri: %.0lf", rezervDegeri);
    outtextxy(10, getmaxy() - 60, rezervDegeri_str);  // Ekranýn altýna rezerv deðerini yazýyor
    // Maliyet ve kar bilgilerini ekranda gösterme
    char info[150];
    sprintf(info, "Toplam Platform: %.2lf, Toplam Sondaj: %.2lf",
            toplamPlatformMaliyeti,toplamSondajMaliyeti );
            outtextxy(10, getmaxy() - 40, info);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
     sprintf(info, "Toplam Maliyet: %.2f, Kar: %.2f",
             toplamMaliyet, karMiktari);
             outtextxy(10, getmaxy() - 20, info);
    
    
}




void dosyadanSekilleriOku() {
    FILE *dosya = fopen("prolab1.txt", "r");
    char satir[300];
    while (fgets(satir, sizeof(satir), dosya)) {
        Sekil sekil;
        int x, y;
        sscanf(satir, "%*[^(](%d,%d)", &x, &y);

        sekil.x[0] = x;
        sekil.y[0] = y;
        sekil.nokta_sayisi = 1;

        char *ptr = satir;
        while ((ptr = strstr(ptr + 1, "("))) {
            sscanf(ptr, "(%d,%d)", &x, &y);
            sekil.x[sekil.nokta_sayisi] = x;
            sekil.y[sekil.nokta_sayisi] = y;
            sekil.nokta_sayisi++;
        }

        sekiller[sekil_sayisi++] = sekil;
    }
    fclose(dosya);
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);
     
    double birimSondajMaliyeti, birimPlatformMaliyeti;
    do {
        printf("Birim sondaj maliyetini giriniz : ");
        scanf("%lf", &birimSondajMaliyeti);
        if (birimSondajMaliyeti < 1 || birimSondajMaliyeti > 10) {
            printf("Deger 1 ile 10 arasinda olmalidir.\n");
        }
    } while (birimSondajMaliyeti < 1 || birimSondajMaliyeti > 10);

    do {
        printf("Birim platform maliyetini giriniz: ");
        scanf("%lf", &birimPlatformMaliyeti);
        if (birimPlatformMaliyeti < 1 || birimPlatformMaliyeti > 10) {
            printf("Deger 1 ile 10 arasinda olmalidir.\n");
        }
    } while (birimPlatformMaliyeti < 1 || birimPlatformMaliyeti > 10);
     
     
     
    dosyadanSekilleriOku();

    int secim = 0;
    int olcekFaktoru = 10;

    while (secim != -1) {
        cleardevice();
        printf("Lutfen cizmek istediginiz sekli secin:\n");
        for (int i = 0; i < sekil_sayisi; i++) {
            printf("%d: Sekil %d\n", i + 1, i + 1);
        }
        printf("-1'e basarak cikabilirsiniz.\n");
        scanf("%d", &secim);

        if (secim > 0 && secim <= sekil_sayisi) {
            cizVeHesapla(sekiller[secim - 1], olcekFaktoru ,birimSondajMaliyeti, birimPlatformMaliyeti);
            delay(5000);
        } else if (secim != -1 || secim > sekil_sayisi || secim <= 0 ) {
            printf("Gecersiz secim!\n");
        }
    }

    closegraph();
    return 0;
}
