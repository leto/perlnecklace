


$string = '
	open(FDNUM,">>./testfile_NUM") or die $!;
	print FDNUM "test\n";
	';

for ( 1 .. 20 ){
	$tmp = $string;
	$tmp =~ s/NUM/$_/g;
	unless ( eval $tmp) {
		print "Open failed of $_\n";
	}
};

