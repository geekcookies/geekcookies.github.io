
# create a commit hook to pass all the file who has this stuff :
# 1. find 'regex' , if any ->
# 2. sed 'regex'
# 3. git add , committ 'cleaned from google tracking links'

%s#\vhttps://www.google.com/url\?q\=(.{-})(\&sa.{-}\)){1}#\1)#g


# dirty links example
[Sito del progetto](https://www.google.com/url?q=http://www.iltucci.com/blog/la-domotica-fai-da-me/&sa=D&ust=1465242204038000&usg=AFQjCNG0Z2a4IJ25Hc3fwO41DVuL1PMRKQ) e [Sito del progetto](https://www.google.com/url?q=http://www.iltucci.com/blog/la-domotica-fai-da-me/&sa=D&ust=1465242204038000&usg=AFQjCNG0Z2a4IJ25Hc3fwO41DVuL1PMRKQ) 

# cleaned link 
[Sito del progetto](http://www.iltucci.com/blog/la-domotica-fai-da-me/)


