
# CPU

Velkommen!

Guiden inneholder følgende:
- Hva er en datamaskin?
- Hva er RAM?
- Hva er en CPU?
- Hva er et program?
- CPU-ens _Fetch-Decode-Execute_-syklus
- Registre

## Hva er en datamaskin?

I sin enkleste form består en datamaskin av to deler:
- RAM, eller _Random Access Memory_
- En CPU, eller _Central Processing Unit_

De fleste datamaskiner har også f.eks. nettverkskort, tastatur, skjerm, harddisker osv., men disse delene skal vi ikke fokusere på her.

## Hva er RAM?
RAM er minnet til datamaskinen. Alt det CPU-en trenger å huske mens den kjører et program, ligger lagret i RAM.

Vi kan se på RAM som én kjempestor tabell. I venstre kolonne har vi _adresser_ i stigende rekkefølge. I høyre kolonne ser vi _dataene_ som er lagret på hver adresse. Grunnen til at RAM heter "Random Access Memory", er at vi kan få tilgang på dataen i en hvilken som helst rekkefølge.

RAM støtter som regel to grunnleggende operasjoner:
- Du kan be om å få _lese_ et tall på en bestemt adresse. Det vil si at du gir RAM en adresse, og får tilbake dataene der.
- Du kan _skrive_ over et tall på en bestemt adresse. Det vil si at du gir RAM en adresse og litt data, og forteller at du vil skrive over hva enn som var der tidligere, med den nye dataen.

En RAM-tabell kan for eksempel se slik ut (legg merke til at adressene ofte begynner på 0 i stedet for 1):

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

NB! Herfra begynner det å bli litt teknisk, så ta det i saaaaaaakte tempo :D

## Hva er et program?

Et _program_, som du for eksempel skriver i Python eller C, brukes for å kommunisere til datamaskinen hva du vil oppnå. Når du programmerer i f.eks. C, vil et annet program kalt en _kompilator_ (det er ikke så viktig hva det er akkurat nå) gjøre om programmet ditt til 0-ere og 1-ere kalt _maskinkode_. Maskinkoden forteller CPU-en nøyaktig hva den skal gjøre, og i hvilken rekkefølge.

Maskinkoden fra et program kan f.eks. se slik ut:  0010  1100  1110  0101  1101

Maskinkoden puttes alltid inn i RAM før programmet kjøres. Hvis vi skal kjøre programmet ovenfor, ser altså RAM slik ut:

|   Adresse (desimal)       |   Data (binær)
|:-------------------------:|:---------------------:
|   0                       |   0010
|   1                       |   1100
|   2                       |   1110
|   3                       |   0101
|   4                       |   1101

## CPU-ens _Fetch-Decode-Execute_-syklus

Anta at RAM nå ser ut som ovenfor, det vil si at vi har lastet inn et program som tar fem plasser i minnet, og at vi ønsker å _kjøre_ dette programmet. Hva betyr det å kjøre programmet? Det betyr at vi starter på begynnelsen av, og utfører instruksjonene i rekkefølge. For å kunne holde styr på hvor i programmet vi er, bruker CPU-en noe som kalles en _Program Counter_ (_PC_). Den starter på verdien 0, og økes for hver instruksjon vi utfører.

Under kjøring av programmet, gjentar CPU-en et veldig enkelt mønster om og om igjen:
1. Den _henter_ (fetch) neste instruksjon fra RAM
2. Den _dekoder_ (decode) instruksjonen, og finner ut hva den gjør
3. Den _utfører_ (execute) instruksjonen 

### (1) Fetch

Det første steget er alltid å finne ut hva som er neste instruksjon. Vi bruker Program Counteren til å vite hvor i minnet (RAM) neste instruksjon er. Hvis Program Counteren er 0, betyr det at neste instruksjon ligger i plass 0 i RAM. Da ber vi RAM om å få lese dataene på plass 0. Her ser vi at det er 0010. 

### (2) Decode

Verdien vi hentet ovenfor lagres i noe som kalles et _Instruction Register_. Bare tenk på det som en bitteliten lagringsplass inni CPU-en som holder styr på hva som er neste instruksjon.

Når vi har hentet instruksjonen, må vi finne ut hva den betyr. Det er opp til den som designer CPU-en å bestemme hva hver kombinasjon av 0-ere og 1-ere betyr. I dette tilfellet kan for eksempel 0010 bety "adder to tall".

### (3) Execute

Nå har vi funnet ut hva instruksjonen gjør. Da kan vi faktisk utføre den. Hvis instruksjonen for eksempel betyr "adder to tall", vil dette steget sørge for at tallene faktisk legges sammen. Akkurat hvilke tall som adderes, hvordan det foregår og hvor resultatet havner, kommer vi tilbake til.

Når dette steget er fullført, vil Program Counteren økes fra 0 til 1, og syklusen begynner på nytt igjen. Det betyr at CPU-en henter instruksjonen med adresse 1, det vil si 1100, og dekoder og utfører den. Denne prosessen skjer om og om igjen.

## Litt om registre

Til nå har vi nevnt to registre:
- Program Counter (PC), som holder styr på hvor i programmet vi er
- Instruction Register (IR), som holder styr på hvilken instruksjon vi utfører

CPU-er inneholder mange registre. Noen av disse er generelle og brukes til mange formål, mens andre er spesielle og brukes til noen få, helt konkrete oppgaver. Passende nok kalles de to kategoriene henholdsvis _general purpose_- og _special purpose_-registre. PC og IR er eksempler på _special purpose_-registre.

Mer generelle registre kalles ofte for eksempel r0, r1, r2, osv. Som vi snakket om tidligere, kan CPU-en addere tall. De to tallene som adderes, ligger lagret i slike registre internt i CPU-en. Resultatet av addisjonen skrives også til et register. Instruksjonen 0010 som vi nevnte ovenfor, kan for eksempel bety "adder tallene i r2 og r5, og skriv resultatet til r1". 

