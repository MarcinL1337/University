import random
from datetime import datetime, timedelta

def generuj_logi(liczba_linii, nazwa_pliku, szansa_powtorzenia=0.15):
    statusy = [200, 404, 500]
    metody_http = ['GET', 'POST', 'PUT', 'DELETE']
    zasoby = ['/TheApplication/WebResource.axd', '/TheApplication/Page', '/AnotherApplication/Resource']

    adresy_ip = []
    
    with open(nazwa_pliku, 'w') as plik:
        for _ in range(liczba_linii):
            if random.random() < szansa_powtorzenia and adresy_ip:
                adres_ip = random.choice(adresy_ip)
            else:
                adres_ip = f'{random.randint(1, 255)}.{random.randint(1, 255)}.{random.randint(1, 255)}.{random.randint(1, 255)}'
                adresy_ip.append(adres_ip)

            czas = datetime.now() - timedelta(days=random.randint(1, 365), hours=random.randint(0, 23), minutes=random.randint(0, 59), seconds=random.randint(0, 59))
            metoda = random.choice(metody_http)
            zasob = random.choice(zasoby)
            status = random.choice(statusy)

            log_line = f'{czas.strftime("%H:%M:%S")} {adres_ip} {metoda} {zasob} {status}\n'
            plik.write(log_line)

if __name__ == '__main__':
    liczba_linii = 1000  # Możesz dostosować liczbę generowanych linii
    nazwa_pliku = 'logi_https.txt'  # Nazwa pliku do zapisania logów

    generuj_logi(liczba_linii, nazwa_pliku)
    print(f'Wygenerowano plik logów: {nazwa_pliku}')