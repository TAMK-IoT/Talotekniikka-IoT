# Kaspos

Tämä ohjelma on osa Tampereen ammattikorkeakoulun talotekniikan koulutuksen IoT-projektia, jossa rakennetaan olosuhdelaboratoriota ilmanlaadun tarkkailuun erilaisissa olosuhteissa. 
Ohjelma toimii konfiguroitavana tilakoneena, joka yhdistää erilaisia sensoreita useilla viestintätavoilla yhden laitteen hallintaan ja lähettää mitatun tiedon pilvipalveluun.

Ohjelma on suunniteltu laajennettavaksi esimerkiksi tukemaan useampaa viestintäprotokollaa ja pilvipalvelua. Projektin vaatimuksena on tuki projektin puitteissa rakennetulle Arduino - MQTT viestiprotokollalle ja Wapicen IoT-Ticket pilvipalvelualustalle.

Tämä ohjelma on opinnäytetyöni ja lisensoitu: The MIT License (MIT) Copyright (c) 2017 Heikki Alho


### Järjestelmävaatimukset

Ohjelma on suunniteltu Linux -käyttöjärjestelmälle, erityisesti Raspberry Pi:n Rasbian -alustalle. Projekti käyttää CMake:a ohjelman rakentamiseen. 
Ohjelma ei sisällä seuraavia kirjastoja, joten ne pitää asentaa ennen ohjelman rakentamista esim. apt-get pakettimanagerin kanssa:
- libconfig++
- libmosquitto
- libmosquittopp
- pkg-config
- libmodbus
- libcurl


IOT_API kirjasto on lisensoitu: The MIT License (MIT) Copyright (c) 2015 Wapice Ltd. (https://github.com/IoT-Ticket/) Projektissa käytetään IoT-LinuxCppClient -kirjastoa ja se rakennetaan projektin rakentamisen yhteydessä.

(Listaus saattaa olla keskeneräinen. Ohjelmaa kääntäessä puuttuvat kirjastot tulevat ilmi)

### Asentaminen

1. Kopioi ohjelma GitHubista (git clone https://github.com/kromii/kaspos)
2. Rakenna projekti juurikansiosta Kaspos (cmake ./)
3. Käännä projekti juurikansiosta Kaspos (make)
4. Käynnistä ohjelma ./Kaspos - Parametrit -t "1" antavat debug-ikkunan

### Käyttö

##### Konfigurointi

Ohjelma on kyseisessä versiossaan tarkoitettu käytettäväksi projektin sensoripuolen Arduino-ohjelman kanssa ( INSERT LINK)
Muokkaamalla konfigurointitiedostoa on mahdollista liittää ja hallita em. ohjelman mukaisia Arduino-sensori -yhdistelmiä kokonaisuuteen ja siirtää sensoridataa pilvipalveluun. Tuetut viestiprotokollat:

- MQTT valmiiksi ohjelmoidulla viestikehyksellä
	- Konfigurointitiedoston vaatimukset : Name, Path, Type, Server, Modifier, Interval, Network (mqtt), messageFormat, deviceNumber, dataLocation
	- **Kyselytopic: "kysely"**
    - Viestiformaatti 01 | 001 | 01 | 01 (Lähetettävä pyyntö on 010010101)
    - Viestikehystunnus 01 (2 numeroa) , tilakoneen sensoritunnista 001 (3 numeroa), arduinon ID 01 (2 numeroa), arduinon sensorin ID 01 (2 numeroa).    
    - **Vastaustopic: "vastaus"**
    - Viestiformaatti 01 | 001 | 12345 (Lähetettävä vastaus on 0100112345)
    - Viestikehystunnus 01 (2 numeroa), tilakoneen sensoritunniste 001 (3 numeroa), sensorin AD-muunnettu lukema (1-10 numeroa)
- MODBUS RTU (Ei käytössä, ongelmia CRC-laskennan kanssa. Tarvitsee testausta, perusteet OK)
	- Konfigurointitiedoston vaatimukset : Name, Path, Type, Server, Modifier, Interval, Network (modbusRTU), deviceName (USB-kytkettävä CAN-adapteri, esim. /dev/ttyUSB1), deviceNumber (slaveAddress, esim 5), dataLocation (holdingRegister, esim. . 4032).
- MODBUS TCP (Ei käytössä, tarvitsee testausta. Perusteet OK)
	- Konfigurointitiedoston vaatimukset : Name, Path, Type, Server, Modifier, Interval, Network (modbusRTU), deviceName (IPAddress laitteelle, esim. 192.168.1.23), deviceNumber (slaveAddress, esim. 5), dataLocation (holdingRegister, esim. 4032).
    
##### Wapicen pilvialusta

TAMKilla on oma serveri Wapicen tarjoamalla IoT-ticket alustalla, joka on tällähetkellä ainoa tuettu pilvipalvelu. 
IoT-Ticketin konfigurointi tapahtuu lisäämällä käyttäjätiedot konfigurointitiedostoon. Ensimmäisellä ohjelman käynnistyskerralla tämän jälkeen laite rekisteröidään omalla IDllään IoT-Ticket palveluun, josta se voidaan tunnistaa ohjelman Dashboardissa.

# Jatkokehitys

Jos satut löytämään tämän ohjelman ja hyödyntämään sitä omissa projekteissasi, toivon että kertoisit aikaansaadoksistasi myös minulle.

    





