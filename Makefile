#
#	Makefile - $Revision: 1.12 $
#
#	rev 2013.09.02 gls continue
#	rev 2013.08.23 gls
#
#	http://home.roadrunner.com/~jgglatt/tech/linuxapi.htm
#
#	http://www.tldp.org/HOWTO/MIDI-HOWTO-10.html
#
#	http://www.midi.org/techspecs/midi_chart-v2.pdf
#
#	http://alsa.opensrc.org/Quick_Install
#
#	http://fundamental-code.com/midi/
#


##midictrl: midictrl.cpp
##	g++ -o $@ $@.cpp -lasound

patMidi: patMidi.cpp
	g++ -o $@ $@.cpp -lasound

wrMidi: wrMidi.cpp
	g++ -o $@ $@.cpp -lasound

rdMidi: rdMidi.cpp
	g++ -o $@ $@.cpp -lasound

midi-listen: midi-listen.cpp
	g++ -o midi-listen midi-listen.cpp -lasound

CountCards: CountCards.cpp
	g++ -o CountCards CountCards.cpp -lasound

CardNames: CardNames.cpp
	g++ -o CardNames CardNames.cpp -lasound


install:
	sudo apt-get install libasound2 alsa-utils alsa-oss
	sudo apt-get install caps
	sudo apt-get install libasound2-dev

get:
	curl -O http://www.cs.fsu.edu/~baker/devices/lxr/source/2.6.25/xawtv-3.95/common/midictrl.c


# ////////////////////////////////////////////////////////////
# ////////////////////////////////////////////////////////////
# ////////////////////////////////////////////////////////////
	
