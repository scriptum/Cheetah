perl -pe 'if(/(.*)\\$/){ my $n = 79 - (tr/\t//); $f = "%-".$n."s\\\n"; printf($f, $1);$_="";}'