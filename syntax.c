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
#include <regex.h>
#include <stdint.h>
#include <assert.h>

#include "syntax.h"

const char *color_names[] = {
	"black",
	"red",
	"green",
	"yellow",
	"blue",
	"magenta",
	"cyan",
	"white"
};


int get_color_by_name(color)
const char *color;
{
	int i;
	for(i=0 ; i<8 ; i++)
		if(!strcmp(color_names[i], color))
			return i;
	return 0;
}

struct syntax_regex* parse_syntax_file(syntaxfp, syn_len)
FILE *syntaxfp;
size_t *syn_len;
{
	char *line = NULL, *linetmp;
	size_t len = 0;
	int args = 0;
	char regex1[200], regex2[200], syn_color[200];
	struct syntax_regex *syn = (struct syntax_regex *)malloc(sizeof(struct syntax_regex) * 200);
	int s = 0;
	assert(syntaxfp != NULL);
	while(getline(&line, &len, syntaxfp) != -1) {
		/* skip comments */
		if(*line == '#')
			continue;
		/* find the number of arguments per line */
		/* FIXME */
		for(linetmp=line, args=0 ; *linetmp ; linetmp++)
			if(*linetmp == ' ' || *linetmp == '\t' || *linetmp == '\n')
				args++;
		assert(args == 2 || args == 3);
		if(args == 2) {
			sscanf(line, "%s %s", regex1, syn_color);
			if(regcomp(&(syn[s].regex[0]), regex1, REG_EXTENDED | REG_NEWLINE)) {
				printf("ERROR\n");
				exit(EXIT_FAILURE);
			}
			syn[s].count = 1;
			syn[s++].color = get_color_by_name(syn_color);
		}
		else {
			sscanf(line, "%s %s %s", regex1, regex2, syn_color);
			if(regcomp(&(syn[s].regex[0]), regex1, REG_EXTENDED | REG_NEWLINE)) {
				printf("ERROR\n");
				exit(EXIT_FAILURE);
			}
			if(regcomp(&(syn[s].regex[1]), regex2, REG_EXTENDED | REG_NEWLINE)) {
				printf("ERROR\n");
				exit(EXIT_FAILURE);
			}
			syn[s].count = 2;
			syn[s++].color = get_color_by_name(syn_color);
		}
	}
	free(line);
	*syn_len = s;
	return syn;
}

uint8_t* get_color_array(syn, syn_len, txt, txt_len) 
struct syntax_regex *syn;
size_t syn_len;
const char *txt;
size_t txt_len;
{
	uint8_t *colors = NULL;
	regmatch_t match;
	regoff_t last_match;
	uint8_t current_color;
	int i, c;
	assert(syn != NULL && txt != NULL);
	/*
		allocate space for the color array
		it's the callee reponsibility to free it
	*/
	colors = (uint8_t *)malloc(sizeof(uint8_t) * txt_len);
	/* iterate over all the regex rules */
	for(i=0 ; i<syn_len ; i++) {
		last_match = 0;
		while(regexec(&(syn[i].regex[0]), txt + last_match, 1, &match, 0) == 0) {
			current_color = colors[match.rm_so + last_match];
			for(c=match.rm_so + last_match ; c < match.rm_eo + last_match && !current_color ; c++)
				colors[c] = syn[i].color;
			last_match += match.rm_so + 1;
			if(syn[i].count == 2) {
				if(regexec(&(syn[i].regex[1]), txt + last_match, 1, &match, 0) == 0) {
					for(; c <match.rm_eo + last_match && !current_color ; c++)
						colors[c] = syn[i].color;
				}
				else {
					fprintf(stderr, "Regex error!\n");
					exit(EXIT_FAILURE);
				}
				last_match += match.rm_so + 1;
			}
		}
		//regfree(&regex);
	}
	return colors;
}


