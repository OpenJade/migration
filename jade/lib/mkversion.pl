#! /usr/bin/perl

while (<>) {
  if ( /.*SP.*/ ) {
    chomp();
    s/^[^0-9]*//;
print <<END;
#define SP_VERSION SP_T("$_")
END
}
}
