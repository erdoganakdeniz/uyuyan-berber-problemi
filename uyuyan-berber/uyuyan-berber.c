#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>

int main(int argc, char** args)
{
    int musteriSayisi, sandalyeSayisi;

    if (argc != 3)
    {
        printf("\nKullanım Hatası!\nKullanım Şekli:\t uyuyan-berber <Müşteri Sayısı> <Sandalye Sayısı>\n\n");
        return EXIT_FAILURE;
    }

    musteriSayisi = atoi(args[1]);
    sandalyeSayisi = atoi(args[2]);

    printf("\n\nGirilen Müşteri Sayısı:\t\t%d", musteriSayisi);
    printf("\nGirilen Sandalye Sayısı:\t%d\n\n", sandalyeSayisi);

    return EXIT_SUCCESS;
}
