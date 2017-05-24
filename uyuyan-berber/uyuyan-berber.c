#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MUSTERI_SINIRI 10 /* Müşteriler için iş parçası sınırı */

/* semaforlar */
sem_t berberKoltugu;    /* berber koltuğuna karşılıklı münhasır erişim sağlar */
sem_t beklemeOdasi;     /* bekleyen müşterileri sınırlar */
sem_t musteriBekletici; /* bekleyen müşterileri berberin işi bitene kadar bekletir */
sem_t berberUykusu;     /* müşteri gelene kadar berberin uyumasını sağlar */

/* fonksiyonlar */
void* berber(void* sayi);
void* musteri(void* sayi);

short tamamlanmaDurumu = 0;

int main(int argc, char** args)
{
    srand((unsigned int)time(NULL));

    int musteriSayisi, sandalyeSayisi, koltukSayisi, musteriNumaralari[MUSTERI_SINIRI];

    if (argc != 4)
    {
        printf("\nKullanım Hatası!\nKullanım Şekli:\t uyuyan-berber <Müşteri Sayısı> <Sandalye Sayısı> <Koltuk Sayısı>\n\n");
        return EXIT_FAILURE;
    }

    musteriSayisi = atoi(args[1]);
    sandalyeSayisi = atoi(args[2]);
    koltukSayisi = atoi(args[3]);
    int berberNumaralari[koltukSayisi];

    if (musteriSayisi > MUSTERI_SINIRI)
    {
        printf("\nMüşteri sınırı: %d\n\n", MUSTERI_SINIRI);
        return EXIT_FAILURE;
    }

    printf("\n\nGirilen Müşteri Sayısı:\t\t%d", musteriSayisi);
    printf("\nGirilen Sandalye Sayısı:\t%d", sandalyeSayisi);
    printf("\nGirilen Berber Koltuğu Sayısı:\t%d\n\n", koltukSayisi);

    /* Numaralandırmalar */
    for (int i = 0; i < MUSTERI_SINIRI; i++)
    {
        musteriNumaralari[i] = i + 1;
    }

    for (int i = 0; i < koltukSayisi; i++)
    {
        berberNumaralari[i] = i + 1;
    }

    /* Semaforların oluşturulması */
    sem_init(&berberKoltugu, 0, koltukSayisi);
    sem_init(&beklemeOdasi, 0, sandalyeSayisi);
    sem_init(&musteriBekletici, 0, 0);
    sem_init(&berberUykusu, 0, 0);

    pthread_t berberler[koltukSayisi], musteriler[MUSTERI_SINIRI];

    /* Berber iş parçalarının oluşturulması */
    for (int i = 0; i < koltukSayisi; i++)
    {
        pthread_create(&berberler[i], NULL, berber, (void*)&berberNumaralari[i]);
    }

    /* Müşteri iş parçalarının oluşturulması */
    for (int i = 0; i < musteriSayisi; i++)
    {
        pthread_create(&musteriler[i], NULL, musteri, (void*)&musteriNumaralari[i]);
        pthread_join(musteriler[i], NULL);
    }

    tamamlanmaDurumu = 1;
    sem_post(&berberUykusu);

    for (int i = 0; i < koltukSayisi; i++)
    {
        pthread_join(berberler[i], NULL);
    }

    return EXIT_SUCCESS;
}

void* berber(void* sayi)
{
    int s = *(int*)sayi;

    while (!tamamlanmaDurumu)
    {
        /* Bir müşteri gelene kadar berber uyur */
        printf("%d numaralı berber uyumakta.\n", s);
        sem_wait(&berberUykusu);

        if (!tamamlanmaDurumu)
        {
            /* Berber çalışmasının simule edilmesi */
            printf("%d numaralı berber çalışıyor.\n", s);
            sleep(1);
            printf("%d numaralı berber işini bitirdi.\n", s);

            /* İşi biten müşterinin serbest bırakılması */
            sem_post(&musteriBekletici);
        }
        else
        {
            printf("Berberler bugünkü işlerini bitirdi.\n");
        }
    }
}

void* musteri(void* sayi)
{
    int s = *(int*)sayi;

    /* Müşteri hareketlerinin simule edilmesi */
    printf("\t%d numaralı müşteri dükkana geliyor.\n", s);
    sleep(2);
    printf("\t%d numaralı müşteri dükkana geldi.\n", s);

    /* Bekleme odasında yer açılmasını bekle */
    sem_wait(&beklemeOdasi);
    printf("\t%d numaralı müşteri bekleme odasına girdi.\n", s);

    sem_wait(&berberKoltugu);   /* Berber koltuğu için bekle */
    sem_post(&beklemeOdasi);    /* Koltuk için bekleme odasından ayrıl */

    /* Berberi uyandır */
    printf("\t%d numaralı müşteri berberi uyandırdı.\n", s);
    sem_post(&berberUykusu);

    sem_wait(&musteriBekletici);/* Berberin işini bitirmesini bekle */
    sem_post(&berberKoltugu);   /* Berber koltuğundan ayrıl */

    printf("\t%d numaralı müşteri dükkandan ayrıldı.\n", s);
}
