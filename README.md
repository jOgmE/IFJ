# IFJ
IFJ hopefully 
![RNGesus](https://cdn.discordapp.com/emojis/616333658579533835.png)

Trello: https://trello.com/b/w0WCtTB6/ifj-projekt

Error dohoda (pokud narazíte na něco, co tím nejde řešit/nesedí to, okamžitě řvát, na tomto dost věcí stojí)
**ERROR HANDLING** pro parser a scanner

Na místě vzniku:
- vypsat chybové hlášení s co nejvíc infem na stderr
- zapsat do globa_error_code číslo chyby

V místě, kde k nám mohla přijít chyba:
- chyba 99 - free everything, return z dané fce
- ostatní chyby - neřešit

**symtable state**

What's working:
- go_in_global()
- go_in_local()
- clean_all_symtables()
- define_id_from_info(...)
- add_undef_id_from_info(...)
- add_undef_id_from_token(...)
- start_symtable_with_functions()

What needs to be done:
- get_id_type(...)
- define_id_from_token(...)

**BEWARE**
Lot of //TODOs, mostly error handling, needs to be dealt with.
But all should be callable.

KA skenneru:
![most of](https://cdn.discordapp.com/attachments/578249520551559169/640207678345576449/IMG_20191102_161633.jpg)
![operators](https://cdn.discordapp.com/attachments/578249520551559169/640215650945728522/IMG_20191102_164739.jpg)
![tokens summary](https://cdn.discordapp.com/attachments/578249520551559169/640215650085634070/IMG_20191102_164730.jpg)
