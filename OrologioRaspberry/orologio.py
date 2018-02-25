#!/usr/bin/env python
# -*- coding: utf-8 -*-

import datetime
import time
import locale
import requests # pip install requests

# libreria per il displaymicrodot
from microdotphat import write_string, set_decimal, clear, scroll, show, set_brightness, fill, set_pixel

# *** FINE IMPORT ***
#
# ** INIZIO FUNZIONI ***

# verifico se internet e' raggiungibile
# se OK ritorna 0
# se non OK ritorna 1
def connessione():
    try:
        req = requests.get("http://httpbin.org/ip")
        return 0
    except Exception, err:
        return 1

# un Esempio per disegnare il simbolo dell'Euro
# 00111
# 01000
# 11110
# 10000
# 11110
# 01000
# 00111
def char_euro():
    set_pixel(2,0,1)
    set_pixel(3,0,1)
    set_pixel(4,0,1)
    
    set_pixel(1,1,1)
    
    set_pixel(0,2,1)
    set_pixel(1,2,1)
    set_pixel(2,2,1)
    set_pixel(3,2,1)
    
    set_pixel(0,3,1)

    set_pixel(0,4,1)
    set_pixel(1,4,1)
    set_pixel(2,4,1)
    set_pixel(3,4,1)
    
    set_pixel(1,5,1)
    
    set_pixel(2,6,1)
    set_pixel(3,6,1)
    set_pixel(4,6,1)    

# legge da una pagina di testo un numero (es: le donazioni del mese)    
def leggehttp():
    if connessione() == 0:
        req = requests.get("http://www.sito.com/fileditestoconnumero.txt")       
        if req.status_code == 200:
            saldo = req.text
        else:
            return "ERR"
    else:
        return "ERR"
    
    return saldo


# dimmer notturno. Se l'orario e' compreso tra le 20 e le 8 la luminosita' e' dimezzata
def dimmer():
    notte = datetime.datetime.now()
    orario_notte = notte.time()
    if orario_notte >= datetime.time(20,00) or orario_notte <= datetime.time(8,00):
        set_brightness(0.5)
    else:
        set_brightness(1)


# visualizza sul display l'orologio        
def mostra_orologio():
    #verifico se c'e' connessione ad Internet
    connesso_internet = 0
    connesso_internet = connessione()
    
    tempo_orologio = 0
    while tempo_orologio < tempo_switch:
        clear()
        t = datetime.datetime.now()
        if t.second % 2 == 0:
            set_decimal(3, 1)
        else:
            set_decimal(3, 0)
        # se manca la connessione ad internet, durante la visualizzazione dell'ora viene anteposto un asterisco
        if (connesso_internet == 0):
            write_string(t.strftime(' %H%M'), kerning=False)
        else:
            write_string(t.strftime('*%H%M'), kerning=False)
        
        show()
        time.sleep(0.05)
        tempo_orologio = tempo_orologio + 1

# visualizza sul display la data        
def mostra_data():
    tempo_data = 0
    while tempo_data < tempo_switch:
        clear()
        t = datetime.datetime.now()
        if t.second % 2 == 0:
            set_decimal(3, 1)
        else:
            set_decimal(3, 0)        
        write_string(t.strftime('%a %d').upper(), kerning=False)
        show()
        time.sleep(0.05)
        tempo_data = tempo_data + 1
        
# visualizza sul display il saldo delle donazioni        
def mostra_saldo():
    tempo_saldo = 0
    saldo_euro = leggehttp()
    while tempo_saldo < tempo_switch:
        clear()
        char_euro()
        t = datetime.datetime.now()
        if t.second % 2 == 0:
            set_decimal(3, 1)
        else:
            set_decimal(3, 0)        
        write_string(saldo_euro, offset_x=8, kerning=False)
        show()
        time.sleep(0.05)
        tempo_saldo = tempo_saldo + 1 

# *** FINE FUNZIONI ***
#
# *** INIZIO MAIN ***

# *** avvio del sistema che eseguo un volta sola ***

# cancello il display
clear()

# messaggio di benvenuto
# kerning=false fa vedere ogni carattere in un display separato
write_string("Avvio!", kerning=False)
show()
time.sleep(3)

#accendo tutti i punti della matrice (cosi' vedo anche se c'e' un guasto)
fill(1)
show()
#faccio degradare la luminosita dal massimo al minimo a un millesimo per volta
luminosita = 1
while luminosita > 0.001:
    luminosita = luminosita - 0.001
    set_brightness(luminosita)

#pausa di mezzo secondo, se no non si vede che il display si e' spento
time.sleep(0.5)

# imposto il tempo di switch tra le verie informazioni
# il numero equivale alla q.ta di cicli con una pauda di 0,05'' per ogni ciclo 
tempo_switch = 50
clear()
show()

#imposto la luminosita al massimo
set_brightness(1)

#imposto i testi di data in Italiano
locale.setlocale(locale.LC_ALL, "it_IT.UTF-8")


# ciclo principale del programma
while True:
    dimmer()
    mostra_orologio()
    mostra_data()
    mostra_orologio()
    mostra_saldo()
