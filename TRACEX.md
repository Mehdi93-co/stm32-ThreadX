



# dump file to schow:
- >dump binary value trace.trx trace_buffer


Kürzel	Bedeutung	Ausgelöst durch
S/P	Semaphore Put	tx_semaphore_put()
S/G	Semaphore Get	tx_semaphore_get()
I/S	Internal Suspend: Der Thread wird schlafen gelegt.	get auf einer Semaphore mit Zähler 0
I/R	Internal Resume: Der Thread wird wieder lauffähig.	put weckt einen wartenden Thread
S/C	Semaphore Create	tx_semaphore_create()
T/C	Thread Create	tx_thread_create()
R (grün)	Running: Ab hier läuft dieser Kontext (Kontextwechsel).	Scheduler