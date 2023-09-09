
# CPU

Velkommen!

Før gruppa møtes for første gang `dag/dato` ønsker vi at alle leser gjennom denne guiden. Du trenger _ikke_ forstå alt, men bare det å lese gjennom den og la innholdet synke litt inn (ikke les den rett før møtet), vil hjelpe mye når vi begynner opplæringen!

Guiden inneholder følgende:
- Hva er en datamaskin?
- Hva er RAM?
- Hva er en CPU?
- Hva er et program?
- Én klokkesyklus

## Hva er en datamaskin?

I sin enkleste form består en datamaskin av to deler:
- RAM, eller _Random Access Memory_
- En CPU, eller _Central Processing Unit_

De fleste datamaskiner har også f.eks. nettverkskort, tastatur, skjerm, harddisker osv., men disse delene skal vi ikke fokusere på her.

## Hva er RAM?
`NB: Oppdater intro-avsnittet her: Har ikke snakket om CPU-en i særlig grad enda.`
RAM er minnet til datamaskinen. Instruksjonene (programmet) som CPU-en kjører, ligger lagret i RAM.

RAM heter "Random Access Memory" fordi vi kan få tilgang på dataen i en hvilken som helst rekkefølge.

Vi kan se på RAM som én kjempestor tabell. I venstre kolonne har vi _adresser_ i stigende rekkefølge. I høyre kolonne ser vi _dataene_ som er lagret på hver adresse. 

RAM støtter som regel to grunnleggende operasjoner:
- Du kan be om å få _lese_ et tall på en bestemt adresse. Det vil si at du gir RAM en adresse, og får tilbake dataene der.
- Du kan _skrive_ over et tall på en bestemt adresse. Det vil si at du gir RAM en adresse og litt data, og forteller at du vil skrive over hva enn som var der tidligere, med den nye dataen.

En slik RAM-tabell kan for eksempel se slik ut:

|   Adresse     |   Data
|:-------------:|:---------:
|   0           |   100
|   1           |   300
|   2           |   4
|   3           |   640
    
La oss si at vi nå spør RAM-brikken hva som ligger i adresse 2 (vi _leser_ adresse 2). Da svarer den 4. Hvis vi nå forteller RAM at den skal skrive 17 på adresse 3 (vi _skriver til_ adresse 3), ser tabellen slik ut etterpå:

|   Adresse     |   Data
|:-------------:|:---------:
|   0           |   100
|   1           |   300
|   2           |   4
|   3           |   17

## Hva er en CPU?

CPU-en kalles ofte "hjernen" til datamaskinen. Man kan godt se på det sånn, bortsett fra at mestparten av minnet altså ligger i RAM (se ovenfor). CPU-en kalles ofte hjernen til datamaskinen, fordi:
- Den utfører matematiske operasjoner, f.eks. addisjon
- Den bestemmer over resten av maskinen, og sender f.eks. signaler til RAM om skriving/lesing når det trengs

## Hva er et program?

Et _program_, som du for eksempel skriver i Python eller C, brukes for å kommunisere til datamaskinen hva du vil oppnå. Når du programmerer i f.eks. C, vil et annet program kalt en _kompilator_ (det er ikke så viktig hva det er akkurat nå) gjøre om programmet ditt til 0-ere og 1-ere kalt _maskinkode_. Maskinkoden forteller CPU-en nøyaktig hva den skal gjøre, og i hvilken rekkefølge.

Maskinkoden fra et program kan f.eks. se slik ut:  0010  1100  1110  0101



## Én klokkesyklus

