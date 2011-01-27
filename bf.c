/*
 * =====================================================================================
 *
 *       Filename:  bf.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011/01/27 19時17分36秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Naoya KANEKO (nk), enukane@skyperpc.net
 *        Company:  skyperpc.net
 *
 * =====================================================================================
 */

#include <u.h>
#include <libc.h>

#include "bf.h"

char	tape[MAXSIZE];
int	tape_pos = 0;
int	tape_inited = 0;

char	buffer[MAXSIZE];
int	buffer_pos = 0;
int	buffer_inited = 0;

void	init_tape(void);
int	get_tape_pos(void);
int	inc_tape_pos(void);
int	dec_tape_pos(void);

void	init_buffer(void);
void	push_buffer(char c);

char*	bf_get_data(void) 
{
	char*	data = strdup(buffer);

	init_buffer();
	init_tape();

	return data;
}

void 	bf_do_cmd(int size, char* cmd)
{
	int	stack = 0;
	int	pc = 0;

	if( 0 == tape_inited )
		init_tape();

	if( 0 == cmd )
		return;

	while( 1 ){
		if( pc >= size )
			break;

		switch( (char)cmd[pc] ){
			case '+':
				tape[get_tape_pos()]++;
				break;
			case '-':
				tape[get_tape_pos()]--;
				break;
			case '.':
				// save in buffer
				push_buffer( tape[get_tape_pos()] );
				break;
			case ',':
				tape[get_tape_pos()] = cmd[pc];
				break;
			case '>':
				inc_tape_pos();
				break;
			case '<':
				dec_tape_pos();
				break;
			case '[':
				if( 0 == tape[get_tape_pos()] ){
					pc++;
					while( 1 ){
						if( pc >= size ) break;
						if( stack <= 0 && cmd[pc] == ']' ) break;
						if( cmd[pc] == '[' ) stack++;
						if( cmd[pc] == ']' ) stack--;
						pc++;
					}
				}
				break;
			case ']':
				pc--;
				while( 1 ){
					if( pc < 0 ) break;
					if( stack <= 0 && cmd[pc] == '[' ) break;
					if( cmd[pc] == ']' ) stack++;
					if( cmd[pc] == '[' ) stack--;
					pc--;
				}
				pc--;
				break;
			case '*':
				// invoke syscall
				break;
			default:
				break;
		}
		pc++;
	}

	return;
}

void	init_tape(void)
{
	int i;

	for( i = 0; i < MAXSIZE; i++){
		tape[i] = 0;
	}

	tape_pos = 0;
}

int	get_tape_pos(void)
{
	return tape_pos;
}

int	inc_tape_pos(void)
{
	tape_pos++;

	if( tape_pos >= MAXSIZE )
		tape_pos = 0;

	return tape_pos;
}

int	dec_tape_pos(void)
{
	tape_pos--;

	if( tape_pos < 0 )
		tape_pos = MAXSIZE-1;

	return tape_pos;
}

void	init_buffer(void)
{
	int i;

	for( i = 0; i < MAXSIZE; i++ )
		buffer[i] = 0;
	buffer_pos = 0;

	return;
}

void	push_buffer(char c)
{
	if( 0 == buffer_inited ){
		init_buffer();
		buffer_inited = 1;
	}

	if( buffer_pos >= MAXSIZE )
		return;

	buffer[buffer_pos++] = c;

	return;
}

