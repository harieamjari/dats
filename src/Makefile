lex : dats_parser.y dats_parser.l
	bison -d dats_parser.y
	flex dats_parser.l
	gcc -o $@ dats_parser.tab.c lex.yy.c -lfl -O3
