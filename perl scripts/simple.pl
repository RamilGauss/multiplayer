use Fcntl;
# Только чтение
sysopen fGPL, "GPL.txt", O_RDONLY or die "Couldn't open!\n";
# читаем из файла
@str=<fGPL>;
close fGPL or die "Couldn't open!\n";
#print @str
# открыть на чтение и запись файл
sysopen fNext, "file.cpp", O_RDWD or die "Couldn't open!\n";
@strNext<fNext>;





