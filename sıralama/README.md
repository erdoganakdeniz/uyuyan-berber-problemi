# 1. Çok *İş Parçacıklı* Sıralama Uygulaması

**Açıklama:**

İlk olarak 1000 elemanlı ve elemanları rastgele üretilmiş bir dizi oluşturulmalıdır. Elemanların değerleri benzersiz olmalıdır.
Program tarafından 3 thread yaratılacaktır. İlk thread dizinin ilk 500 elemanını sıralarken, ikinci thread diğer 500 elemanı sıralayacaktır.
Her iki thread sıralama işini bitirdikten sonra üçüncü thread ilk thread’in sıraladığı ve ikinci thread’in sıraladığı dizileri alarak birleştirme işlemini gerçekleştirecektir.
Birleştirme işlemi sonunda elimizde sıralanmış yeni bir dizi olmalıdır.
Son adımda listedeki elemanların programın çalıştığı dizinde oluşturulacak “son.txt” isimli dosyaya yazdırılması gerekmektedir.

**Durum:**
- [x] Rastgele üretilmiş 1000 benzersiz elemanlı bir dizi oluşturulması.
- [x] 3 iş parçasının oluşturulması.
- [x] Dizi elemanlarının ikiye bölünerek iki iş parçasında ayrı ayrı sıralanması.
- [x] Sıralanmış dizilerin son iş parçasında birleştirilip tekrar sıralanması.
- [x] Sıralanmış dizideki elemanların "son.txt" isimli dosyaya yazdırılması.

