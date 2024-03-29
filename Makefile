compiler: clean lex.yy.c parser.tab.o main.cc
		g++ -g -w -o compiler parser.tab.o lex.yy.c main.cc symbolTable.cc semanticAnalyser.cc cfg.cc irNode.cc interpreter.cc linked_list.cc -std=c++14
parser.tab.o: parser.tab.cc
		g++ -g -w -c parser.tab.cc -std=c++14
parser.tab.cc: parser.yy
		bison parser.yy
lex.yy.c: lexer.flex parser.tab.cc
		flex lexer.flex
tree: 
		 dot -Tpdf tree.dot -o tree.pdf
		 dot -Tpdf irTree.dot -o irTree.pdf
		 dot -Tpdf cfgTree.dot -o cfgTree.pdf
clean:
		rm -f parser.tab.* lex.yy.c* compiler stack.hh tree.dot tree.pdf irTree.dot irTree.pdf cfgTree.dot cfgTree.pdf
		
