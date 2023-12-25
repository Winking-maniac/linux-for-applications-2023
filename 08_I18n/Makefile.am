bin_PROGRAMS=guess
guess_SOURCES=guess.c
pkgdata_DATA=ru/LC_MESSAGES/guess.mo

guess.po: guess.pot
	msgmerge -U $@ $<

guess.pot: guess.c
	touch guess.pot
	xgettext -k_ -c guess.c -o guess.pot.new
	msgmerge -U guess.pot guess.pot.new
	rm guess.pot.new

ru/LC_MESSAGES/guess.mo: guess.po
	mkdir -p ru/LC_MESSAGES
	msgfmt guess.po -o ru/LC_MESSAGES/guess.mo

ru: guess
	LANGUAGE=ru_RU ./guess

en: guess
	LANGUAGE=en_US ./guess
