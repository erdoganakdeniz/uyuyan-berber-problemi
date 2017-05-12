#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define ELEMAN_SAYISI 1000
#define URETIM_SINIRI 10000

unsigned char kullanilan_sayilar[URETIM_SINIRI] = { 0 };

int main()
{
    int* bazDizi = malloc(ELEMAN_SAYISI * sizeof(*bazDizi));

    srand((unsigned)time(0));
    printf("\nÜRETİLEN ELEMANLAR: \n");

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

    printf("\n");
    for (unsigned int i = 0; i < ELEMAN_SAYISI; ++i)
    {
        printf("%d\t", bazDizi[i]);

        if ((i + 1) % 10 == 0)
            printf("\n");
    }
    
    printf("\n");

    free(bazDizi);

    return EXIT_SUCCESS;
}
