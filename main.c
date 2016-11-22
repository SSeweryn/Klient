#include<stdio.h> 				//printf
#include<string.h> 				//memset
#include<stdlib.h> 				//exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>


#define BUFLEN 64  				//maksymalna długość bufora
#define PORT 666    			//port, przez który wysyłamy dane

void die(char *s)
{
    printf(s);	//drukuje błąd
    exit(1);
}


int main(int argc, char **argv)
{
	char *server;

    struct sockaddr_in si_other;
    int s, slen=sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];

    server = argv[4]; // przypisanie do wskaznika czwartego parametru (adres serwera)

    //Wyswietlanie pobranych danych
    printf("\nWczytane parametry: \n");
    printf("\nZmienna A: %s" 		, argv[1]);
    printf("\nZmienna B: %s" 		, argv[2]);
    printf("\nZadanie  : %s"  	, argv[3]);
    printf("\nAdres SERWERA : %s \n" , argv[4]);

    //Umieszczenie zmiennych w payloadzie
    sprintf(message , "%s %s %s" , argv[1] , argv[2] , argv[3]);

    //Tworzenie gniazda
    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket"); //wywolanie funkcji die
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    if (inet_aton(server , &si_other.sin_addr) == 0)  //inet_aton konwertuje adres IP na binerne dane i laduje do struktury in_adr			//inte_aton konwertuje adres IP na binerne dane i laduje do struktury in_adr
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    //wysyłanie wiadomosci message
    if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
    {
        die("sendto()");
    }

    printf("\nOczekiwanie na odpowiedź serwera...");

    //odebranie odpowiedzi i wydrukowanie jej
    //czyszczenie bufora przez wypełnienie zerami (w razie gdyby zawierał poprzednio odebrane dane)
    memset(buf,'\0', BUFLEN);

    //próba odbioru danych, oczekiwanie na odpowiedź serwera
    if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1) // funkcja blokująca dopóki nie dostanie odpowiedzi z serwera
    {
        die("recvfrom()");
    }

printf("\nOdpowiedz: \n%s", buf);
    close(s);
    return 0;
}
