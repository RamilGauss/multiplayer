use Fcntl;
# ������ ������
sysopen fGPL, "GPL.txt", O_RDONLY or die "Couldn't open!\n";
# ������ �� �����
@str=<fGPL>;
close fGPL or die "Couldn't open!\n";
#print @str
# ������� �� ������ � ������ ����
sysopen fNext, "file.cpp", O_RDWD or die "Couldn't open!\n";
@strNext<fNext>;





