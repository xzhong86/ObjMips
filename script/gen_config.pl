#!/usr/bin/perl

use strict;

print "/* Don't modify this file, this is generated from config.mk */\n\n";
print "#ifndef CONFIG_H\n";
print "#define CONFIG_H\n\n";
while(<>) {
    if(/^(\S+)\s*=\s*([yn])/) {
	if ($2 eq "y") {
	    print "#define $1\n";
	} else {
	    print "#undef  $1\n";
	}
    }
}
print "\n#endif\n";
