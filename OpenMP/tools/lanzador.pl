#!/usr/bin/perl
# Created by: John Corredor UAB-CAOS
# john@aopcjc.uab.es
# Julio 2008
#
# Lanza el binary juntos con los argumentos desde argumentos.txt
# y los eventos, desde el archivo Entrada_Experimentos


if (@ARGV[0]) {
	$FInput = "$ARGV[0]";
} else {
	usage();
}

use Switch;


$path0 = `pwd`;
chomp($path0);
$T = index($path0,"T")-1;
$Path = substr($path0,0,$T);


@Labels = ("Experiment","Binarys","Threads","Repetitions");
&Search(\@Labels);
$Experiment	= $Inputs[0];
@Binarys	= split(/,/,$Inputs[1]);
@Threads	= split(/,/,$Inputs[2]);
$Repet		= $Inputs[3];

@Separate = ("-",":");

#Capture SIZE MATRIZ in vector @Size
$Size  	= &Argument($Separate[1],1);   @Size	= split(/\n/,$arg);

#################################################################
# Main program: WARNING Binary x Threads x Size x Repetitions 	#
#################################################################

foreach $executable (@Binarys) {
	$CARP = $executable."-".$Experiment."-";
	chop($CARP);
	system ("mkdir $Path/Soluciones/$CARP");
	foreach $thread (@Threads) {
		foreach $size (@Size) {
			$file = "$Path/Soluciones/$CARP/$executable-".$size."-TH-".$thread.".txt";
			print "$executable $size $thread ... on $file\n";
		 	for ($k = 0; $k < $Repet; $k++) {
				system("$Path/BIN/$executable $size $thread 0 0  >> $file");
				#print "$Path/BIN/$executable $size $thread 0 0  \n";
			}	
			close($file);
		}
	}
}	


##################################################
# Functions					 #
##################################################
	   
sub Argument {
	$lab = $_[0];
	$pos = $_[1]; # pos: 1 2 3 4 5 
	$arg = `cat $FInput | grep $lab | cut -f2 -d$lab | cut -f$pos -d' '`;
	chomp($arg);
}

sub Search {
	my($label) = @_;
	foreach (@ {$label}) {
		$name = `grep $_ $FInput | cut -f1`;
		chomp($name);
		push(@Inputs, $name);
	}
}

sub Formato {
	$in = $_[0];
	if ($in < 1000) {
		$Id = $in;
	} elsif($in < 10000) {
		$ddk = $in/10000;
		$Id = $ddk."K";
	} elsif($in < 1000000){
		$ddk = $in/1000000;
		$Id = $ddk."M";
	}
}

sub usage() {
	print "\n test.pl: Error incorrect usage \n\n";
	print "\t <directory> Is directory start point to check \n\n\n";
	exit(1);
}

