#! /usr/bin/perl

while (<>) {
  if ( /.*Jade.*/ ) {
    chomp();
    s/^[^0-9]*//;
print <<END;
#define JADE_VERSION SP_T("$_")
END
}
}
