geonames: geonames.c
	gcc -o geonames -std=c11 -Wall -Wextra geonames.c

html:
	pandoc -s README.md -o README.html --metadata title="README.md" -c ./misc/github-pandoc.css
	pandoc -s sujet.md -o sujet.html --metadata title="sujet.md" -c ./misc/github-pandoc.css --from markdown-yaml_metadata_block

database:
	curl http://download.geonames.org/export/dump/countryInfo.txt --output countryInfo.txt
	curl http://download.geonames.org/export/dump/cities15000.zip --output cities15000.zip
	unzip cities15000.zip
	rm cities15000.zip

test:
	bats check.bats
clean:
	rm -f *.swp *.o *.html *.swp *.txt geonames
