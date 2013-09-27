/*-
 * Copyright (c) 2013, by Mohamed Aslan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. The name of the developer may NOT be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <stdint.h>
#include <assert.h>

#include "syntax.h"

void print_buffer(buf, colors)
const char* buf;
uint8_t* colors;
{
	int count = 0;
	while(*buf) {
		if(colors[count]) 
        		attron(COLOR_PAIR(colors[count]));
		printw("%c", *buf++);
		if(colors[count])
		       	attroff(COLOR_PAIR(colors[count]));
		count++;
	}
	refresh();
}

int main(argc, argv)
int argc;
char **argv;
{
	FILE *srcfp, *syntaxfp;
	struct syntax_regex *syn;
	char *buf, *tmp;
	uint8_t *colors;
	int size;
	size_t rules_size;

	if(argc != 3) {
		printf("Wrong arguments!\n");
		exit(EXIT_FAILURE);
	}

	syntaxfp = fopen(argv[1], "r");
	if(syntaxfp == NULL) {
		printf("Can not open sytax file!\n");
		exit(EXIT_FAILURE);
	}
	syn = parse_syntax_file(syntaxfp, &rules_size);
	fclose(syntaxfp);
	
	srcfp = fopen(argv[2], "r");
	if(srcfp == NULL) {
		printf("Can not open source file!\n");
		exit(EXIT_FAILURE);
	}
	buf = tmp = (char *)malloc(10001);
	
	while(fscanf(srcfp, "%c", tmp++) != EOF);
	size = tmp - buf - 1; /* size = strlen(buf); */
	fclose(srcfp);


        initscr();                      /* Start curses mode */
	scrollok(stdscr, TRUE);
        if(has_colors() == FALSE) {
                endwin();
                printf("Colors are not supported!\n");
                exit(EXIT_FAILURE);
        }
        start_color();
        init_pair(0, COLOR_BLACK, COLOR_BLACK);
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        init_pair(4, COLOR_BLUE, COLOR_BLACK);
        init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(6, COLOR_CYAN, COLOR_BLACK);
        init_pair(7, COLOR_WHITE, COLOR_BLACK);

	colors = get_color_array(syn, rules_size, buf, size);
	print_buffer(buf, colors);

	getch();
        endwin();
	return 0;
}

