#!/usr/bin/perl

use strict;

my $in_config = 0;

print "/* Don't modify this file, this is generated from config.mk */\n\n";
print "#ifndef CONFIG_H\n";
print "#define CONFIG_H\n\n";
while(<>) {
    if(/^#Start configs/) {
	$in_config = 1;
    }
    if(/^#End configs/) {
	$in_config = 0;
    }
    next if ($in_config == 0);

    next if (/^#.*/);
    if(/^(\S+)\s*=\s*([yn])/) {
	if ($2 eq "y") {
	    print "#define $1\n";
	} else {
	    print "#undef  $1\n";
	}
    }
}
print "\n#endif\n";
