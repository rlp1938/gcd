/*
 * getoptions.c
 * Copyright 2015 Bob Parker <rlp1938@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
*/

#include "getoptions.h"

void process_options(int argc, char **argv)
{
	optstring = ":hq";
	qflag = 0;
	helpmsg =
  "\tUsage: gcd [option] latitude1,longitude1 latitude2,longitude2\n"

  "\n\tOptions:\n"
  "\t-h, --help\n\toutputs this help message.\n"
  "\t-q, --quiet\n"
  "\t Outputs distance as a bald number, not wrapped in text.\n"
  ;
	int opt;
	//int digit_optind = 0;
	// only need digit_optind if I process long opts without a short one
	while(1) {
		int option_index = 0;
		static struct option long_options[] = {
			{"help", 0, 0, 'h' },
			{"quiet", 0, 0, 'q' },
			{0, 0, 0, 0 }
		};

		opt = getopt_long(argc, argv, optstring, long_options,
							&option_index);
		if (opt == -1) break;

		switch(opt){
		case 0:	// needed only for --longopts not paired with short opt.
			break;
		case 'h':
			dohelp(0);
		break;
		case 'q':
			qflag = 1;
			break;
		case ':':
			fprintf(stderr, "Option %c requires an argument\n"
					,optopt);
			dohelp(1);
			break;
		case '?':
			fprintf(stderr, "Illegal option: %c\n",optopt);
			dohelp(1);
			break;
		} //switch()
	}//while()
}

void dohelp(int forced)
{
  fputs(helpmsg, stderr);
  exit(forced);
}
