#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>

#define ELEMAN_SAYISI 1000
#define URETIM_SINIRI 10000

unsigned char kullanilan_sayilar[URETIM_SINIRI] = { 0 };

typedef struct
{
    int baslangic;
    int bitis;
    short ip_no;
} parametre;

int* bazDizi;
int* sonDizi;

void* sirala(void* parametreler);               /* sıralama iş parçası fonksiyonu */
void* birlestirvesirala(void* parametreler);    /* sonuçları birleştiren iş parçası fonksiyonu */

int main()
{
    bazDizi = (int*) malloc(ELEMAN_SAYISI * sizeof(*bazDizi));
    sonDizi = (int*) malloc(ELEMAN_SAYISI * sizeof(*sonDizi));

    srand((unsigned)time(0));
    printf("\nÜRETİLEN ELEMANLAR:\n");

    /* Floyd Algoritması ile rastgele benzersiz sayı üretimi. O(ELEMAN_SAYISI) */
    int in, im = 0;

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
    p->bitis = ELEMAN_SAYISI / 2;
    p->ip_no = 1;
    pthread_create(&isleyiciler[0], 0, sirala, p);
    /* Birinci sıralama iş parçasının oluşturulması */

    /* İkinci sıralama iş parçasının oluşturulması */
    p = (parametre*) malloc(sizeof(parametre));
    p->baslangic = ELEMAN_SAYISI / 2;
    p->bitis = ELEMAN_SAYISI;
    p->ip_no = 2;
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
    p->bitis = ELEMAN_SAYISI;
    p->ip_no = 3;
    pthread_create(&isleyiciler[2], 0, birlestirvesirala, p);
    /* Birleştirmek için kullanılacak iş parçasının oluşturulması */

    pthread_join(isleyiciler[2], NULL); /* Üçüncü iş parçasını bekle */

    free(bazDizi); /* baz diziyi bellekten temizle */

    /* Sıralanmış dizi elemanlarının dosyaya yazdırılması */
    FILE* d = fopen("son.txt", "w");

    if (d == NULL)
    {
        printf("Dosya hatası oluştu.\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < ELEMAN_SAYISI; i++)
    {
        fprintf(d, "%d. Eleman: %d\n", i + 1, sonDizi[i]);
    }

    fclose(d);

    free(sonDizi); /* son diziyi bellekten temizle */

    return EXIT_SUCCESS;
}

void* sirala(void* parametreler)
{
    printf("\n------------------------------------------------------------\n");
    parametre* p = (parametre*) parametreler;

    int baslangic = p->baslangic;
    int bitis = p->bitis;

    /* Alınan dizinin gösterilmesi */
    printf("\n(%d) SIRALAMA İŞ PARÇASI İÇİN ALINAN DİZİ:\n", p->ip_no);
    for (int i = baslangic; i < bitis; i++)
    {
        printf("%d\n", bazDizi[i]);
    }

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
    printf("\n(%d) SIRALAMA İŞ PARÇASI SONRASI SIRALANAN DİZİ:\n", p->ip_no);
    for (int i = baslangic; i < bitis; i++)
    {
        printf("%d\n", bazDizi[i]);
    }
    printf("\n");

    pthread_exit(0);
}

void* birlestirvesirala(void* parametreler)
{
    printf("\n------------------------------------------------------------\n\n");
    parametre* p = (parametre*) parametreler;

    int baslangic = p->baslangic;
    int bitis = p->bitis;

    /* Dizinin sıralanarak birleştirilmesi */
    int i = baslangic, j = baslangic, k = bitis / 2;

    for (i = baslangic; i < bitis; i++)
    {
        if (j == bitis / 2)
        {
            sonDizi[i] = bazDizi[k++];
        }
        else if (k == bitis)
        {
            sonDizi[i] = bazDizi[j++];
        }
        else
        {
            if (bazDizi[j] < bazDizi[k])
            {
                sonDizi[i] = bazDizi[j++];
            }
            else
            {
                sonDizi[i] = bazDizi[k++];
            }
        }
    }
    /* Sıralanmış dizinin yazdırılması */
    printf("(%d) BİRLEŞTİRME VE SIRALAMA İŞ PARÇASI SONRASI SIRALANAN DİZİ:\n", p->ip_no);
    for (int i = baslangic; i < bitis; i++)
    {
        printf("%d\t", sonDizi[i]);

        if (i != 0 && (i + 1) % 10 == 0)
            printf("\n");
    }
    printf("\n");

    pthread_exit(0);
}
