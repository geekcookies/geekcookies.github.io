
# create a commit hook to pass all the file who has this stuff :
# 1. find 'regex' , if any ->
# 2. sed 'regex'
# 3. git add , committ 'cleaned from google tracking links'

%s#\vhttps://www.google.com/url\?q\=(.+)(\&sa.+\)){0}#\1)#g

%s#\vhttps://www.google.com/url\?q\=#\1#g


# dirty links
[Sito del progetto](https://www.google.com/url?q=http://www.iltucci.com/blog/la-domotica-fai-da-me/&sa=D&ust=1465242204038000&usg=AFQjCNG0Z2a4IJ25Hc3fwO41DVuL1PMRKQ) e [Repository GitHub](https://www.google.com/url?q=https://github.com/cesco78/domotica-telegram&sa=D&ust=1465242204039000&usg=AFQjCNGbxIaZ-M5RBqSRjWxdy_CuVP7gAw)

