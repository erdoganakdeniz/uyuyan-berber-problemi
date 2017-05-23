#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <pthread.h>
#include <semaphore.h>

#define MUSTERI_SINIRI 10 /* Müşteriler için iş parçası sınırı */

/* semaforlar */
sem_t berberKoltugu;    /* berber koltuğuna karşılıklı münhasır erişim sağlar */
sem_t beklemeOdasi;     /* bekleyen müşterileri sınırlar */
sem_t musteriBekletici; /* bekleyen müşterileri berberin işi bitene kadar bekletir */
sem_t berberUykusu;     /* müşteri gelene kadar berberin uyumasını sağlar */

/* fonksiyonlar */
void* berber();
void* musteri(void* sayi);
void* rastgeleBekle(int saniye);

short tamamlanmaDurumu = 0;

int main(int argc, char** args)
{
    srand((unsigned int)time(NULL));

    int musteriSayisi, sandalyeSayisi, musteriNumaralari[MUSTERI_SINIRI];

    if (argc != 3)
    {
        printf("\nKullanım Hatası!\nKullanım Şekli:\t uyuyan-berber <Müşteri Sayısı> <Sandalye Sayısı>\n\n");
        return EXIT_FAILURE;
    }

    musteriSayisi = atoi(args[1]);
    sandalyeSayisi = atoi(args[2]);

    if (musteriSayisi > MUSTERI_SINIRI)
    {
        printf("\nMüşteri sınırı: %d\n\n", MUSTERI_SINIRI);
        return EXIT_FAILURE;
    }

    printf("\n\nGirilen Müşteri Sayısı:\t\t%d", musteriSayisi);
    printf("\nGirilen Sandalye Sayısı:\t%d\n\n", sandalyeSayisi);

    for (int i = 0; i < MUSTERI_SINIRI; i++)
    {
        musteriNumaralari[i] = i + 1;
    }

    /* Semaforların oluşturulması */
    sem_init(&berberKoltugu, 0, 1);
    sem_init(&beklemeOdasi, 0, sandalyeSayisi);
    sem_init(&musteriBekletici, 0, 0);
    sem_init(&berberUykusu, 0, 0);

    pthread_t ipBerber, musteriler[MUSTERI_SINIRI];

    /* Berber iş parçasının oluşturulması */
    pthread_create(&ipBerber, NULL, berber, NULL);

    /* Müşteri iş parçalarının oluşturulması */
    for (int i = 0; i < musteriSayisi; i++)
    {
        pthread_create(&musteriler[i], NULL, musteri, (void*)&musteriNumaralari[i]);
        pthread_join(musteriler[i], NULL);
    }

    tamamlanmaDurumu = 1;
    sem_post(&berberUykusu);
    pthread_join(ipBerber, NULL);

    return EXIT_SUCCESS;
}

void* berber()
{
    while (!tamamlanmaDurumu)
    {
        /* Bir müşteri gelene kadar berber uyur */
        printf("Berber uyumakta.\n");
        sem_wait(&berberUykusu);

        if (!tamamlanmaDurumu)
        {
            /* Berber çalışmasının simule edilmesi */
            printf("Berber çalışıyor.\n");
            rastgeleBekle(3);
            printf("Berber işini bitirdi.\n");

            /* İşi biten müşterinin serbest bırakılması */
            sem_post(&musteriBekletici);
        }
        else
        {
            printf("Berber bugünkü işini bitirdi.\n");
        }
    }
}

void* musteri(void* sayi)
{
    int s = *(int*)sayi;

    /* Müşteri hareketlerinin simule edilmesi */
    rastgeleBekle(3);
    printf("%d numaralı müşteri dükkana geldi.\n", s);

    /* Bekleme odasında yer açılmasını bekle */
    sem_wait(&beklemeOdasi);
    printf("%d numaralı müşteri bekleme odasına girdi.\n", s);

    sem_wait(&berberKoltugu);   /* Berber koltuğu için bekle */
    sem_post(&beklemeOdasi);    /* Koltuk için bekleme odasından ayrıl */

    /* Berberi uyandır */
    printf("%d numaralı müşteri berberi uyandırdı.\n", s);
    sem_post(&berberUykusu);

    sem_wait(&musteriBekletici);/* Berberin işini bitirmesini bekle */
    sem_post(&berberKoltugu);   /* Berber koltuğundan ayrıl */

    printf("%d numaralı müşteri dükkandan ayrıldı.\n", s);
}

void* rastgeleBekle(int saniye)
{
    sleep(((rand() % 2) * saniye) + 1);
}
