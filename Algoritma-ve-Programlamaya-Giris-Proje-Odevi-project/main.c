#include <stdio.h>

// Gecikmiş girişleri işlemek için fonksiyon
// gecikmiş girişlerde çalışan kişinin bilgilerini ve gecikme süresini hesaplar.
void gecGiris(int* sicil, int* dakikaToplam, int* gecDakika, int* gecSicil);

// Erken çıkışları işlemek için fonksiyon
// çalışan kişinin bilgilerini ve erken çıkış süresini hesaplar.
void erkenCıkıs(int* sicil, int* dakikaToplam, int* erkenDakika, int* erkenSicil);

int main(void)
{
    // Dosya pointer'larını tanımlıyoruz
    FILE *gunluk, *erken, *gec; // Dosyalar: "gunluk.dat", "erken.dat", "gec.dat"
    char veri[20];  // Dosyadan okunan her bir satırdaki veriyi geçici olarak tutacak dizi
    int sicil, saat, dakika;  // Sicil numarası, saat ve dakika bilgilerini tutan değişkenler
    char kontrol;  // Giriş (<) veya çıkış (>) kontrolünü tutacak karakter
    int gecSicil, gecDakika = 0;  // Gecikmiş giriş yapan kişinin sicil numarası ve gecikme süresi
    int erkenSicil, erkenDakika = 0;  // Erken çıkan kişinin sicil numarası ve erken çıkış süresi

    // "gunluk.dat" dosyasını sadece okuma amaçlı açıyoruz
    gunluk = fopen("gunluk.dat", "r");
    if (gunluk != NULL)  // Eğer dosya başarılı bir şekilde açıldıysa
    {
        printf("Dosya basariyla acildi\n");

        // Dosyanın her satırını satır satır okuyoruz
        while (fgets(veri, 20, gunluk))
        {
            // Okunan satırdaki verileri ilgili değişkenlere ayırıyoruz
            // Format: [Sicil Numarası] [Saat:Dakika] [Kontrol (< veya >)]
            sscanf(veri, "%d %d:%d %c", &sicil, &saat, &dakika, &kontrol);

            // Toplam dakika cinsinden zamanı hesaplıyoruz (örneğin 9:30 -> 570 dakika)
            int dakikaToplam = saat * 60 + dakika;

            // Kontrol karakterine göre işlem yapıyoruz
            switch (kontrol)
            {
            case '<':  // Giriş durumunda gecikmeyi kontrol eden fonksiyon çağrılır
                gecGiris(&sicil, &dakikaToplam, &gecDakika, &gecSicil);
                break;
            case '>':  // Çıkış durumunda erken çıkışı kontrol eden fonksiyon çağrılır
                erkenCıkıs(&sicil, &dakikaToplam, &erkenDakika, &erkenSicil);
                break;
            }
        }

        // "gunluk.dat" dosyasını kapatıyoruz
        fclose(gunluk);

        // "gec.dat" dosyasını yazma (output) amaçlı açıyoruz
        gec = fopen("gec.dat", "w");
        if (gec != NULL)  // Eğer dosya başarılı bir şekilde açıldıysa
        {
            printf("gec.dat dosyasi basariyla acildi\n");
            // En fazla geciken çalışanın bilgilerini dosyaya yazıyoruz
            fprintf(gec, "%d %d", gecSicil, gecDakika);
            printf("gec.dat dosyasina veriler basariyla yazdirildi\n\n");
        }
        else  // Eğer dosya açılamadıysa hata mesajı yazdırıyoruz
        {
            printf("gec.dat dosyasi acilamadi\n");
            return 1;  // Programı hata koduyla sonlandırıyoruz
        }

        // "erken.dat" dosyasını yazma (output) amaçlı açıyoruz
        erken = fopen("erken.dat", "w");
        if (erken != NULL)  // Eğer dosya başarılı bir şekilde açıldıysa
        {
            printf("erken.dat dosyasi basariyla acildi\n");
            // En erken çıkan çalışanın bilgilerini dosyaya yazıyoruz
            fprintf(erken, "%d %d", erkenSicil, erkenDakika);
            printf("erken.dat dosyasina veriler basariyla yazdirildi\n");
        }
        else  // Eğer dosya açılamadıysa hata mesajı yazdırıyoruz
        {
            printf("erken.dat dosyasi acilamadi\n");
            return 1;  // Programı hata koduyla sonlandırıyoruz
        }

        // Tüm dosyaları kapatıyoruz
        fclose(gec);
        fclose(erken);
    }
    else  // Eğer "gunluk.dat" dosyası açılamazsa hata mesajı yazdırıyoruz
    {
        printf("Dosya bulunamadi\n");
        return 1;  // Programı hata koduyla sonlandırıyoruz
    }

    return 0;  // Program başarıyla tamamlandı
}

// Gecikmiş girişleri işleyen fonksiyon
// Çalışanın giriş zamanı 9:00'dan (540 dakika) sonra ise gecikmeyi hesaplar ve kaydeder
void gecGiris(int* sicil, int* dakikaToplam, int* gecDakika, int* gecSicil)
{
    // Eğer toplam dakika 540'dan (9:00) sonra ise
    if (*dakikaToplam > 540)
    {
        int gecDk = *dakikaToplam - 540;  // Gecikme süresini hesaplıyoruz
        // Eğer bu gecikme önceki en büyük gecikmeden fazla ise, değerleri güncelliyoruz
        if (gecDk > *gecDakika)
        {
            *gecDakika = gecDk;
            *gecSicil = *sicil;
        }
    }
}

// Erken çıkışları işleyen fonksiyon
// Çalışanın çıkış zamanı 17:00'dan (1020 dakika) önce ise erken çıkışı hesaplar ve kaydeder
void erkenCıkıs(int* sicil, int* dakikaToplam, int* erkenDakika, int* erkenSicil)
{
    // Eğer toplam dakika 1020'den (17:00) önce ise
    if (*dakikaToplam < 1020)
    {
        int erkenDk = 1020 - *dakikaToplam;  // Erken çıkış süresini hesaplıyoruz
        // Eğer bu erken çıkış önceki en büyük erken çıkıştan fazla ise, değerleri güncelliyoruz
        if (erkenDk > *erkenDakika)
        {
            *erkenDakika = erkenDk;
            *erkenSicil = *sicil;
        }
    }
}

// Not: Fonksiyonlarda pointer kullanmamızın sebebi, değerleri doğrudan değiştirebilmektir.
// Eğer pointer kullanmasaydık, fonksiyonlar sadece kopyalar(by value) üzerinde çalışırdı
// ve gerçek değişkenlerin değerlerini değiştiremezdi.