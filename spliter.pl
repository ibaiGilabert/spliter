use strict;
use warnings;

@ARGV or die "Input file required as command-line parameter\n";

my $out;

while (<>) {
  if ( /(\d+)\*+RAW$/ ) {
    open $out, '>', "$1.out" or die $!;
    select $out;
  }
  print $_ if $out;
}
