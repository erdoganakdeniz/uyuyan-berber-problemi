#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>

#define ELEMAN_SAYISI 10
#define URETIM_SINIRI 10000

unsigned char kullanilan_sayilar[URETIM_SINIRI] = { 0 };

typedef struct
{
    int baslangic;
    int bitis;
} parametre;

int* bazDizi;
int* sonDizi;

void* sirala(void* parametreler);       /* sıralama iş parçası fonksiyonu */
void* birlestir(void* parametreler);    /* sonuçları birleştiren iş parçası fonksiyonu */

int main()
{
    bazDizi = (int*) malloc(ELEMAN_SAYISI * sizeof(*bazDizi));
    sonDizi = (int*) malloc(ELEMAN_SAYISI * sizeof(*sonDizi));

    srand((unsigned)time(0));
    printf("\nÜRETİLEN ELEMANLAR:\n");

    /* Floyd Algoritması ile rastgele benzersiz sayı üretimi. O(ELEMAN_SAYISI) */
    int in, im;
    im = 0;

    for (in = URETIM_SINIRI - ELEMAN_SAYISI; in < URETIM_SINIRI && im < ELEMAN_SAYISI; ++in)
    {
        int s = rand() % (in + 1);      /* rastgele bir sayı üret */

        if (kullanilan_sayilar[s])      /* üretilen sayıyı zaten kullandık */
            s = in;                     /* üretilen sayı yerine in'yi kullan */

        assert(!kullanilan_sayilar[s]); /* kullanılacak sayı kullanılmamış olmalı */

        bazDizi[im++] = s;              /* sayıyı kullan */
        
        kullanilan_sayilar[s] = 1;      /* kullanılan sayılara indis olarak ekle */
    }
    
    assert(im == ELEMAN_SAYISI);        /* im = ELEMAN_SAYISI olmalı */
    /* Floyd Algoritması */

    /* Elemanların yazdırılması */
    printf("\n");
    
    for (unsigned int i = 0; i < ELEMAN_SAYISI; ++i)
    {
        printf("%d\t", bazDizi[i]);
        
        if ((i + 1) % 10 == 0)
            printf("\n");
    }
    
    printf("\n");
    /* Elemanların yazdırılması */

    pthread_t isleyiciler[3];

    /* Birinci sıralama iş parçasının oluşturulması */
    parametre* p = (parametre*) malloc(sizeof(parametre));
    p->baslangic = 0;
    p->bitis = (ELEMAN_SAYISI / 2) - 1;
    pthread_create(&isleyiciler[0], 0, sirala, p);
    /* Birinci sıralama iş parçasının oluşturulması */

    /* İkinci sıralama iş parçasının oluşturulması */
    p = (parametre*) malloc(sizeof(parametre));
    p->baslangic = (ELEMAN_SAYISI / 2);
    p->bitis = ELEMAN_SAYISI - 1;
    pthread_create(&isleyiciler[1], 0, sirala, p);
    /* İkinci sıralama iş parçasının oluşturulması */

    /* Birleştirmek için ilk iki iş parçasının bitmesini bekle */
    for (int i = 0; i < 2; i++)
    {
        pthread_join(isleyiciler[i], NULL);
    }

    /* Birleştirmek için kullanılacak iş parçasının oluşturulması */
    p = (parametre*) malloc(sizeof(parametre));
    p->baslangic = 0;
    p->bitis = ELEMAN_SAYISI / 2;
    pthread_create(&isleyiciler[2], 0, birlestir, p);
    /* Birleştirmek için kullanılacak iş parçasının oluşturulması */

    pthread_join(isleyiciler[2], NULL); /* Üçüncü iş parçasını bekle */

    free(bazDizi);
    free(sonDizi);

    return EXIT_SUCCESS;
}

void* sirala(void* parametreler)
{
    parametre* p = (parametre*) parametreler;
    
    int baslangic = p->baslangic;
    int bitis = p->bitis + 1;

    /* Alınan dizinin gösterilmesi */
    printf("SIRALAMA İŞ PARÇASI İÇİN ALINAN DİZİ:\n");
    for (int i = baslangic; i < bitis; i++)
    {
        printf("%d\t", bazDizi[i]);
    }
    printf("\n");

    /* Alınan dizinin sıralanması */
    int g = 0;
    for (int i = baslangic; i < bitis; i++)
    {
        for (int j = baslangic; j < bitis - 1; j++)
        {
            if (bazDizi[j] > bazDizi[j + 1])
            {
                g = bazDizi[j];
                bazDizi[j] = bazDizi[j + 1];
                bazDizi[j + 1] = g;
            }
        }
    }

    /* Sıralanmış dizinin yazdırılması */
    printf("SIRALAMA İŞ PARÇASI SONRASI SIRALANAN DİZİ:\n");
    for (int i = baslangic; i < bitis; i++)
    {
        printf("%d\t", bazDizi[i]);
    }
    printf("\n");

    for (int i = baslangic; i < bitis; i++)
    {
        sonDizi[i] = bazDizi[i];
    }
    printf("\n");

    pthread_exit(NULL);
}

void* birlestir(void* parametreler)
{

}