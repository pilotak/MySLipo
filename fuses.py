Import('env')
env.Replace(FUSESCMD="avrdude $UPLOADERFLAGS -e -Ulock:w:0xFF:m -Uhfuse:w:0xDA:m -Uefuse:w:0x06:m -Ulfuse:w:0xE2:m")
