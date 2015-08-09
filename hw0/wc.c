#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

// total number of lines, words, chars in all the files
static int total_lines = 0, total_words = 0, total_chars = 0;

void wc( FILE *infile, char *inname );

int main (int argc, char *argv[]) {

  if( argc > 1 ){ // if we have command-line arguments

    // the name of the file we're processing
    char *inname = NULL;

    // now process files one by one
    while( *++argv != NULL ){ // while we have command-line arguments to process
      // open the file for reading
      FILE *infile = fopen( *argv, "r" );
      // check that everything went OK
      if( infile == NULL ){
	perror( *argv );
	exit( EXIT_FAILURE );
      }
    
      inname = *argv;

      wc( infile, inname );
      
      // close the file we were reading
      if( fclose(infile) != 0 ){
	perror( "fclose" );
	exit( EXIT_FAILURE );
      }
    
    } // while

    if( argc > 2 ) // if we had more than one input file
      // print the total
      printf("%7d %7d %7d   %s\n",  total_lines, total_words, total_chars, "total");
  }

  else // i.e., no input files, input comes from stdin
    wc( stdin, "");

    return 0;
} // main
  

void wc( FILE *infile, char *inname) {
  int lines = 0, words = 0, chars = 0;
  bool seen_space = false;
  int ch;
  while( (ch = fgetc( infile )) != EOF ){
    
    if( ch == '\n' ){
      ++ lines;
      ++ total_lines;
    }
    
    if( isspace(ch) && !seen_space ){
      ++ words;
      ++ total_words;
      seen_space = true;
    }

    if( !isspace(ch) && seen_space )
      seen_space = false;
    
    ++ chars;
    ++ total_chars;
  } // while
  
  printf("%7d %7d %7d   %s\n",  lines, words, chars, inname);

}// wc
